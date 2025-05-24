/**
 * Copyright (c) 2025 thebest12lines
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "Label.h"
#include "../Utilities.h"
#include "Console.h"
#include "Definitions.h"
#include "TypeDefinitions.h"
#include "Window.h"
#include <dwrite.h>
#include <iostream>
#pragma comment(lib, "dwrite.lib")

#ifdef _WIN32
#include <windows.h>
#endif
using namespace reflect::console;
using namespace reflect::utilities;
void reflect::Label::setFont(std::string font) {
  fontStr = font;
  if (font == "default") {
    fontStr = DEFAULT_FONT;
  }
}
void reflect::Label::registerClass() {
  WNDCLASS wc = {};
  wc.lpszClassName = "D2D1Label";
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpfnWndProc = labelProc;
  RegisterClass(&wc);
}
void reflect::Label::createResources(ID2D1RenderTarget *rt,
                                     IDWriteFactory *dwrite,
                                     reflect::Window *win) {

  if (pTextFormat) {
    pTextFormat->Release();
    pTextFormat = nullptr;
  }
  if (pBgBrush) {
    pBgBrush->Release();
    pBgBrush = nullptr;
  }
  if (pTextBrush) {
    pTextBrush->Release();
    pTextBrush = nullptr;
  }

  std::wstring wfont(fontStr.begin(), fontStr.end());
  dwrite->CreateTextFormat(wfont.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
                           DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
                           fontSize, L"en-us", &pTextFormat);

  rt->CreateSolidColorBrush(D2D1::ColorF(win->getColor().r / 255.0f,
                                         win->getColor().g / 255.0f,
                                         win->getColor().b / 255.0f),
                            &pBgBrush);
  rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pTextBrush);
}
LRESULT CALLBACK reflect::Label::labelProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                           LPARAM lParam) {
  reflect::Label *pThis = nullptr;

  if (uMsg == WM_CREATE) {
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<reflect::Label *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    reflect::Window *win = reinterpret_cast<reflect::Window *>(
        GetWindowLongPtr(GetAncestor(hwnd, GA_ROOT), GWLP_USERDATA));

    RECT rc;

    GetClientRect(hwnd, &rc);
    // Initialize Direct2D
    ID2D1Factory *pFactory =
        win->getProperty<ID2D1Factory *>("direct2DFactory");
    IDWriteFactory *pDWriteFactory = win->getProperty<IDWriteFactory *>(
        "directWriteFactory"); // get from parent window
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
        D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

    HRESULT hr = pFactory->CreateHwndRenderTarget(
        rtProps, hwndRTProps,
        &(pThis->childRenderTarget)); // <--- store it on pThis
    // std::wstring wtextFont =
    //     std::wstring(pThis->fontStr.begin(), pThis->fontStr.end());
    //// In WM_CREATE block after creating childRenderTarget:
    // hr = pDWriteFactory->CreateTextFormat(
    //     wtextFont.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
    //     DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
    //     pThis->fontSize, L"en-us", &pThis->pTextFormat);

    if (SUCCEEDED(hr)) {
      pThis->createResources(pThis->childRenderTarget, pDWriteFactory, win);
    }

  } else {
    pThis = reinterpret_cast<reflect::Label *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }
  reflect::Window *win = reinterpret_cast<reflect::Window *>(
      GetWindowLongPtr(GetAncestor(hwnd, GA_ROOT), GWLP_USERDATA));
  // Example: In Label::render or Label::paint

  // Create text format (font, size, etc.)
  IDWriteFactory *pDWriteFactory = win->getProperty<IDWriteFactory *>(
      "directWriteFactory"); // get from parent window
  ID2D1Factory *pFactory = win->getProperty<ID2D1Factory *>("direct2DFactory");
  if (pThis) {

    // Delegate the handling of messages to the instance

    switch (uMsg) {
    case WM_SIZE: {

      RECT rc;
      GetClientRect(hwnd, &rc);
      if (pThis->childRenderTarget) {
        D2D1_SIZE_U newSize =
            D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
        pThis->childRenderTarget->Resize(newSize);
      }

      pThis->createResources(pThis->childRenderTarget, pDWriteFactory, win);
      InvalidateRect(hwnd, nullptr, FALSE); // Request redraw
      break;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      pThis->childRenderTarget->BeginDraw();

      D2D1_RECT_F rect = {0, 0, pThis->size.x, pThis->size.y};

      pThis->childRenderTarget->FillRectangle(rect, pThis->pBgBrush);
      std::wstring wtext = std::wstring(pThis->text.begin(), pThis->text.end());

      pThis->childRenderTarget->DrawText(wtext.c_str(), (UINT32)wtext.length(),
                                         pThis->pTextFormat, rect,
                                         pThis->pTextBrush);

      HRESULT hr = pThis->childRenderTarget->EndDraw();
      if (hr == D2DERR_RECREATE_TARGET) {
        RECT rc;

        GetClientRect(hwnd, &rc);
        pThis->childRenderTarget->Release();
        pThis->childRenderTarget = nullptr;
        pThis->createResources(pThis->childRenderTarget, pDWriteFactory, win);
        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
        D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
            D2D1::HwndRenderTargetProperties(
                hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

        HRESULT hr = pFactory->CreateHwndRenderTarget(
            rtProps, hwndRTProps,
            &(pThis->childRenderTarget)); // <--- store it on pThis
        InvalidateRect(hwnd, nullptr, FALSE);
      }

      EndPaint(hwnd, &ps);
      return 0;
    }
    case WM_ERASEBKGND: {
      // Prevent background erase; Direct2D will handle it.
      return 1;
    }
    }
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void reflect::Label::setFontSize(int size) { fontSize = size; }
void reflect::Label::setColor(uint8_t r, uint8_t g, uint8_t b) {
  color = {r, g, b};
}
reflect::Color3 reflect::Label::getColor() { return color; }
void reflect::Label::render(HWND &parentHWND, HWND &windowHWND) {
#ifdef _WIN32
  if (!IsWindow(parentHWND)) {
    reflectError("parent HWND is invalid!");
    std::exit(REFLECT_ERROR_WIN_PARENT_HWND_INVALID);
  }
  reflect::Window *window = reinterpret_cast<reflect::Window *>(
      GetWindowLongPtr(windowHWND, GWLP_USERDATA));
  registerClass();
  // Create the label window
  hwnd = CreateWindow("D2D1Label",  // Predefined class for a label
                      text.c_str(), // Label text
                      WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN |
                          WS_CLIPSIBLINGS, // Styles: visible and child window
                      position.x,
                      position.y + window->getProperty<int>(
                                       "customTitleBarSize"), // Position (x, y)
                      size.x, size.y, // Size (width, height)
                      parentHWND,     // Parent window handle
                      NULL,           // No menu or child ID
                      winstance,      // Instance handle
                      this            // Additional application data
  );

  if (!hwnd) {
    std::cout << getLastErrorAsString();
    return;
  }

  // Create a custom font

  // Resize the label based on the text and font size
  // --- DirectWrite text measurement for accurate sizing ---
  IDWriteFactory *pDWriteFactory =
      window->getProperty<IDWriteFactory *>("directWriteFactory");
  if (pDWriteFactory) {
    std::wstring wtext = std::wstring(text.begin(), text.end());
    std::wstring wfont = std::wstring(fontStr.begin(), fontStr.end());
    IDWriteTextFormat *pTextFormat = nullptr;
    HRESULT hr = pDWriteFactory->CreateTextFormat(
        wfont.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, (FLOAT)fontSize,
        L"en-us", &pTextFormat);

    if (SUCCEEDED(hr)) {
      // Enable word wrapping
      pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);

      // Use the top-level window's client width as the layout width for
      // wrapping
      RECT parentRc;
      GetClientRect(parentHWND, &parentRc);
      float layoutWidth = (float)(parentRc.right - parentRc.left);
      if (layoutWidth <= 0)
        layoutWidth = 200.0f; // fallback

      float layoutHeight = 4096.0f; // large height to allow for wrapping

      IDWriteTextLayout *pTextLayout = nullptr;
      hr = pDWriteFactory->CreateTextLayout(
          wtext.c_str(), (UINT32)wtext.length(), pTextFormat, layoutWidth,
          layoutHeight, &pTextLayout);

      if (SUCCEEDED(hr)) {
        DWRITE_TEXT_METRICS metrics;
        hr = pTextLayout->GetMetrics(&metrics);
        if (SUCCEEDED(hr)) {
          // Only auto-size vertically to fit the wrapped text
          size.x = (int)layoutWidth;
          size.y = static_cast<int>(metrics.height + fontSize);

          // Update the label's size
          SetWindowPos(hwnd, NULL, position.x, position.y, size.x, size.y,
                       SWP_NOZORDER | SWP_NOMOVE);
        }
        pTextLayout->Release();
      }
      pTextFormat->Release();
    }
  }
#elif __linux__
  // do nothing, as you cannot create a label in linux
#endif
}

void reflect::Label::setVisible(bool flag) {
#ifdef _WIN32
  ShowWindow(this->hwnd, flag ? SW_SHOW : SW_HIDE);

#elif __linux__

#endif
}
reflect::Label::Label(std::string text, Vector2 pos)
    : childRenderTarget(nullptr) {
  this->text = text;
  position = pos;
  initializeObject(REFLECT_OBJECT_LABEL, REFLECT_OBJECT_TEXTCOMPONENT);
}