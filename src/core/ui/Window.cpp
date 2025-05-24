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
#include "Window.h"
#include "../Utilities.h"
#include "Component.h"
#include "Components.h"

#include "Console.h"
#include "Image.h"
#include "Label.h"
#include "MenuBar.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <iostream>
#include <string>
#include <wincodec.h>
#ifdef _WIN32
#include "Notification.h"
#include <cstdio>
#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <windowsx.h>
#pragma comment(lib, "dwrite.lib")

#pragma comment(lib, "dwmapi.lib")
using namespace reflect::console;
// Direct2D-specific members
// Due to floating point operations, may not produce exact color
void reflect::Window::addStyle(WindowStyle style) {
  switch (style) {
  case STYLE_DARK_TITLE_BAR:
    reflectDebug("enabling dark title bar...")
        // Enable dark mode title bar
        BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark,
                          sizeof(dark));

    // Enable rounded corners
    DWM_WINDOW_CORNER_PREFERENCE cornerPreference = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
                          &cornerPreference, sizeof(cornerPreference));

    // Enable Mica background
    DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_TABBEDWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                          sizeof(backdropType));
  }
}
void reflect::Window::initializeDirectWrite() {
  if (!this->pDWriteFactory) {
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown **>(&this->pDWriteFactory));
    if (FAILED(hr)) {
      std::wcerr << L"Failed to create DirectWrite factory." << std::endl;
      this->pDWriteFactory = nullptr;
    }
  }
}

void reflect::Window::setColor(uint8_t r, uint8_t g, uint8_t b) {
  this->bgColor[0] = r / 255.0f;
  this->bgColor[1] = g / 255.0f;
  this->bgColor[2] = b / 255.0f;
}
reflect::Vector2 reflect::Window::getSize() { return size; }
// Due to floating point operations, may not produce exact color
void reflect::Window::setColor(Color3 color) {
  this->bgColor.r = color.r / 255.0f;
  this->bgColor.g = color.g / 255.0f;
  this->bgColor.b = color.b / 255.0f;
}
reflect::Color3 reflect::Window::getColor() {
  return {static_cast<unsigned char>(bgColor[0] * 255),
          static_cast<unsigned char>(bgColor[1] * 255),
          static_cast<unsigned char>(bgColor[2] * 255)};
}

// Due to floating point operations, may not produce exact color
void reflect::Window::setColor(Color3Array color) {
  this->bgColor[0] = color[0] / 255.0f;
  this->bgColor[1] = color[1] / 255.0f;
  this->bgColor[2] = color[2] / 255.0f;
}
void reflect::Window::setRenderLoop(void (*loop)(Window &)) {
  renderLoop = loop;
}
void reflect::Window::initializeDirect2D() {
  if (!this->pFactory) {

    reflectDebug("initializing Direct2D...");
    // Create the Direct2D factory

    reflectDebug("creating d2d1 factory...");
    HRESULT hr =
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pFactory);

    int i = 0;
  }
  if (!this->pRenderTarget) {
    RECT rc;
    GetClientRect(hwnd, &rc);

    // Create the render target
    reflectDebug("creating renderer target...");
    this->pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
        &this->pRenderTarget);
  }
};

namespace {
float getDPIScaleForWindow(HWND hwnd) {
  HDC hdc = GetDC(hwnd);
  float dpi = GetDeviceCaps(hdc, LOGPIXELSX); // or LOGPIXELSY
  ReleaseDC(hwnd, hdc);
  return dpi / 96.0f; // 96 is the default DPI baseline
}

void safeResize(ID2D1HwndRenderTarget *pRenderTarget, D2D1_SIZE_U size) {
  __try {
    HRESULT hr = pRenderTarget->Resize(size);
    if (FAILED(hr)) {
      // Handle HRESULT errors if needed
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    DWORD exceptionCode = GetExceptionCode();
    // Use printf instead of std::cout for SEH restrictions
    std::printf("[WARN] [safeResize]: Exception occurred while resizing "
                "Direct2D render target: 0x%08X\n",
                exceptionCode);
  }
}
} // namespace
bool openglRendering = false, direct2dRendering = false, firstUpdate = true;
HGLRC currentContext = nullptr;
HDC glHdc = nullptr;
reflect::Image closeIcon;

LRESULT CALLBACK reflect::Window::windowProc(HWND hwnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam) {
  reflect::Window *pThis = nullptr;
  if (uMsg == WM_CREATE) {
    reflectDebug("message type: WM_CREATE");
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<reflect::Window *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->hwnd = hwnd;
    // Initialize Direct2D
    if (!pThis->useGL) {
      reflectDebug("calling initializeDirect2D...");
      pThis->initializeDirect2D();
      reflectDebug("calling initializeDirectWrite...");
      pThis->initializeDirectWrite();
    }
    closeIcon.setImageLocation("resources://reflectClose");
    closeIcon.setImageLocation("resources://reflectCloseHover", "hover");
    closeIcon.setSize({10, 10});
    Color3Float flt = {0.9f, 0.9f, 0.9f};
    closeIcon.setColor(flt);

  } else {
    pThis = reinterpret_cast<reflect::Window *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
  };
  if (pThis && !pThis->pRenderTarget) {
    pThis->initializeDirect2D();
  }
  if (pThis) {

    // Delegate the handling of messages to the instance

    switch (uMsg) {
    case WM_DESTROY:
      reflectDebug("message type: WM_DESTROY");
      PostQuitMessage(0);
      reflectInfo("Exiting...");
      return 0;
    // case WM_KEYDOWN: {
    //   UINT scanCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
    //   char ch = (char)scanCode;
    //   std::cout << ch << std::endl;
    //   // wParam contains the virtual key code of the key that was pressed

    //  pThis->pressedKeys[ch] = true;
    //  return 0;
    //}
    // case WM_KEYUP: {

    //  UINT scanCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
    //  char ch = (char)scanCode;

    //  // wParam contains the virtual key code of the key that was pressed

    //  pThis->pressedKeys[ch] = false;
    //  return 0;
    //}
    case WM_LBUTTONUP: {
      if (pThis->customTitleBar) {
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left; // The width of the client area

        RECT areaRect = {width - 50, 0, width,
                         40}; // Example area: x=100, y=100 to x=200, y=200
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        POINT clickPoint = {xPos, yPos};

        if (PtInRect(&areaRect, clickPoint)) {
          exit(0);
          // MessageBox(hwnd, L"Clicked inside the area!", L"Click Detected",
          // MB_OK);
        } else {
          //  MessageBox(hwnd, L"Clicked outside the area!", L"Click Detected",
          //  MB_OK);
        }
      }
      break;
    }
    // case WM_CTLCOLORSTATIC: {
    //   HDC hdcStatic = (HDC)wParam;                 // Get device context
    //   SetTextColor(hdcStatic, RGB(255, 255, 255)); // Set text color (Red)
    //   SetBkColor(hdcStatic,
    //              RGB(pThis->bgColor.r * 255, pThis->bgColor.b * 255,
    //                  pThis->bgColor.b * 255)); // Set background color
    //                  (Green)

    //  static HBRUSH hbrBkgnd = CreateSolidBrush(
    //      RGB(pThis->bgColor.r * 255, pThis->bgColor.b * 255,
    //          pThis->bgColor.b * 255)); // Create background brush
    //  return (INT_PTR)hbrBkgnd;         // Return brush handle
    //}
    case WM_SIZE: {

      if (pThis->pRenderTarget) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = {static_cast<UINT32>(rc.right - rc.left),
                            static_cast<UINT32>(rc.bottom - rc.top)};
        ::safeResize(pThis->pRenderTarget, size);
      }
      // Set the clear color

      InvalidateRect(hwnd, NULL,
                     TRUE); // Mark the entire window as needing a repaint
      break;
    }
    case WM_NCACTIVATE: {

      if (pThis && pThis->customTitleBar) {
        InvalidateRect(hwnd, nullptr, false);
        return TRUE;
      }
      break;
    }
    case WM_ACTIVATE: {

      if (pThis->customTitleBar) {
        RECT rc;
        InvalidateRect(hwnd, nullptr, false);
        GetWindowRect(hwnd, &rc);
        closeIcon.setPosition(
            {rc.right - rc.left - (10 + (40 / 2)), ((10 + (40 / 2)) / 2)});
      }
      break;
    }
    case WM_MOVE:
      if (pThis->customTitleBar) {
        InvalidateRect(hwnd, NULL, true);
      }

      break;
    case WM_SIZING: {
      if (pThis->beforeRenderLoop)
        pThis->callInit = true;
      if (pThis->customTitleBar) {
        RECT rc;

        GetWindowRect(hwnd, &rc);
        closeIcon.setPosition(
            {rc.right - rc.left - (10 + (40 / 2)), ((10 + (40 / 2)) / 2)});
      }
      break;
    }
    case WM_NCHITTEST: {
      if (pThis->customTitleBar) {

        LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);

        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ScreenToClient(hwnd, &pt);

        RECT rc;
        GetClientRect(hwnd, &rc);

        int borderSize = 8; // Resize border thickness

        // Check if mouse is in title bar region (y < 30)

        // Resize hit-test logic
        if (pt.y < borderSize) {
          if (pt.x < borderSize)
            return HTTOPLEFT;
          if (pt.x > rc.right - borderSize)
            return HTTOPRIGHT;
          return HTTOP;
        }
        if (pt.y > rc.bottom - borderSize) {
          if (pt.x < borderSize)
            return HTBOTTOMLEFT;
          if (pt.x > rc.right - borderSize)
            return HTBOTTOMRIGHT;
          return HTBOTTOM;
        }
        if (pt.x < borderSize)
          return HTLEFT;
        if (pt.x > rc.right - borderSize)
          return HTRIGHT;
        if (pt.y >= 0 && pt.y < 30 && pt.x < rc.right - 50) {
          return HTCAPTION; // Allows dragging like a standard title bar
        }
        return hit; // Default behavior
      }
      break;
    }
    case WM_MOUSEMOVE: {
      if (pThis->customTitleBar) {
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left; // The width of the client area

        RECT areaRect = {width - 40, 0, width,
                         40}; // Example area: x=100, y=100 to x=200, y=200
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        POINT clickPoint = {xPos, yPos};

        if (PtInRect(&areaRect, clickPoint)) {
          if (!pThis->closeHovering) {
            pThis->closeHovering = true;

            InvalidateRect(hwnd, nullptr, false);

            // MessageBox(hwnd, L"Clicked inside the area!", L"Click Detected",
            // MB_OK);
          }
        } else {
          if (pThis->closeHovering) {
            pThis->closeHovering = false;

            InvalidateRect(hwnd, nullptr, false);
          }
          //  MessageBox(hwnd, L"Clicked outside the area!", L"Click Detected",
          //  MB_OK);
        }
      }
      break;
    }
    case WM_NCMOUSEMOVE: {
      if (pThis->customTitleBar) {
        RECT rc;
        GetWindowRect(hwnd, &rc);
        int width = rc.right - rc.left; // The width of the client area

        RECT areaRect = {width - 40, 0, width,
                         40}; // Example area: x=100, y=100 to x=200, y=200
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        POINT clickPoint = {xPos, yPos};

        if (PtInRect(&areaRect, clickPoint)) {
          if (!pThis->closeHovering) {
            pThis->closeHovering = true;

            InvalidateRect(hwnd, nullptr, false);
          }

          // MessageBox(hwnd, L"Clicked inside the area!", L"Click Detected",
          // MB_OK);
        } else {
          if (pThis->closeHovering) {
            pThis->closeHovering = false;

            InvalidateRect(hwnd, nullptr, false);
          }
          //  MessageBox(hwnd, L"Clicked outside the area!", L"Click Detected",
          //  MB_OK);
        }
      }
      break;
    }
    case WM_NCCALCSIZE: {
      if (pThis->customTitleBar) {
        if (wParam == TRUE) {
          NCCALCSIZE_PARAMS *pParams = (NCCALCSIZE_PARAMS *)lParam;
          pParams->rgrc[0].top += 1; // Shrink the non-client area slightly
          return 0;
        }
      }
      break;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      if (!openglRendering && firstUpdate) {
        glHdc = GetDC(hwnd);
        if (pThis->useGL) {
          pThis->glCtx->initializeContext(*pThis, glHdc);
        }
        currentContext = wglGetCurrentContext();
        reflectDebug("checking if OpenGL is initialized...");
        if (currentContext) {
          reflectDebug("context found, rendering with OpenGL...");
          openglRendering = true;
          wglDeleteContext(currentContext);
          pThis->callInit = true;
        } else {
          reflectDebug("OpenGL context not found, falling back to Direct2D...");
          direct2dRendering = true;
        }

      } /*else {
        currentContext = wglGetCurrentContext();
      }*/

      if (currentContext != nullptr && openglRendering) {

      } else if (!openglRendering && direct2dRendering) {

        if (pThis->pRenderTarget == nullptr) {
          pThis->initializeDirect2D();
        }
        if (firstUpdate && pThis->customTitleBar) {
          pThis->add(closeIcon);
        }

        // Create a clipping region for the child controls

        // Start Direct2D rendering

        pThis->pRenderTarget->BeginDraw();

        // Set background color (this will not cover child control areas due
        // to ExcludeClipRect)
        pThis->pRenderTarget->Clear(D2D1::ColorF(
            pThis->bgColor[0], pThis->bgColor[1], pThis->bgColor[2]));
        RECT rc;

        // Clear the window
        if (pThis->customTitleBar) {
          GetWindowRect(hwnd, &rc);
          int width = rc.right - rc.left; // The width of the client area

          D2D1_RECT_F rect;
          rect.left = 0.0f;
          rect.top = 0.0f;
          rect.right = width;
          rect.bottom = 40.0f;
          ID2D1SolidColorBrush *pBrush = nullptr;
          HRESULT hr = pThis->pRenderTarget->CreateSolidColorBrush(
              D2D1::ColorF(0.9f, 0.9f, 0.9f), &pBrush);
          pThis->pRenderTarget->FillRectangle(rect, pBrush);
          if (FAILED(hr)) {
            // Handle brush creation error
            std::wcerr << L"Failed to create solid color brush." << std::endl;
            return 0;
          }
          if (pThis->closeHovering) {
            D2D1_RECT_F rect;
            rect.left = width;
            rect.top = 0.0f;
            rect.right = width - 50;
            rect.bottom = 40.0f;
            ID2D1SolidColorBrush *pBrush = nullptr;
            HRESULT hr = pThis->pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(1.0f, 0.0f, 0.0f), &pBrush);
            pThis->pRenderTarget->FillRectangle(rect, pBrush);
            if (closeIcon.getActiveImage() != "hover") {
              Color3Float flt = {1.0f, 0.0f, 0.0f};
              closeIcon.setColor(flt);
              closeIcon.setActiveImage("hover");
              closeIcon.paint();
            }
          } else {
            D2D1_RECT_F rect;
            rect.left = width;
            rect.top = 0.0f;
            rect.right = width - 50;
            rect.bottom = 40.0f;
            ID2D1SolidColorBrush *pBrush = nullptr;
            HRESULT hr = pThis->pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(0.9f, 0.9f, 0.9f), &pBrush);

            pThis->pRenderTarget->FillRectangle(rect, pBrush);
            if (closeIcon.getActiveImage() != "default") {
              Color3Float flt = {0.9f, 0.9f, 0.9f};
              closeIcon.setColor(flt);
              closeIcon.setActiveImage("default");
              closeIcon.paint();
            }
          }
        }

        //  pThis->pRenderTarget->DrawBitmap(bitmap, {100, 500, 110, 510});

        HRESULT hr = pThis->pRenderTarget->EndDraw();
        // if (hr == D2DERR_RECREATE_TARGET) {
        //   // Handle device loss
        //   pThis->pRenderTarget->Release();
        //   pThis->pRenderTarget = nullptr;
        //   pThis->initializeDirect2D();
        // }
        //   Clean up
      }

      EndPaint(hwnd, &ps);
      firstUpdate = false;
      break;
    }
    }
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
bool renderContextInitialized = false;
bool reflect::Window::isKeyPressed(char key) {
  return GetAsyncKeyState(key) & 0x8000;
};
bool reflect::Window::showNotification(Notification &notif) {
  if (!IsWindow(hwnd)) {
    reflectError("Window handle is invalid!");
    return false;
  }
  NOTIFYICONDATA nid = {};
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 32067;
  nid.uFlags = NIF_INFO | NIF_MESSAGE | NIF_ICON | NIF_TIP;
  nid.uCallbackMessage =
      WM_USER + 1; // Custom message ID for notification events

  // Load an icon for the notification
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);

  strcpy_s(nid.szInfo, notif.text.c_str());
  strcpy_s(nid.szTip, "Tooltip");
  strcpy_s(nid.szInfoTitle, notif.title.c_str());
  // wcsncpy_s(nid.szInfoTitle, wTitle.c_str(), ARRAYSIZE(nid.szInfoTitle) -
  // 1); wcsncpy_s(nid.szInfo, wText.c_str(), ARRAYSIZE(nid.szInfo) - 1);

  // std::wstring tooltip = L"Notification Tooltip";
  /*wcsncpy_s(nid.szTip, tooltip.c_str(), ARRAYSIZE(nid.szTip) - 1);*/

  nid.dwInfoFlags = NIIF_INFO;
  if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
    reflectError("Failed to add notification icon! (" +
                 std::to_string(GetLastError()) + ")");
    return false;
  }

  // Modify the notification icon to show the balloon tip
  /*if (!Shell_NotifyIcon(NIM_MODIFY, &nid)) {
    error("Failed to modify notification icon! (" +
              std::to_string(GetLastError()) + ")",
          "showNotification");
  }*/
  return true;
}

// void reflect::Window::ShowNotification(Notification&  notif) {
//     NOTIFYICONDATA nid = {};
//     nid.cbSize = sizeof(NOTIFYICONDATA);
//     nid.hWnd = hwnd;
//     nid.uID = 32067;
//     nid.uFlags = NIF_INFO;
//     // Convert std::string to std::wstring
//     std::wstring wTitle = std::wstring(notif.title.begin(),
//     notif.title.end()); std::wstring wText =
//     std::wstring(notif.text.begin(), notif.text.end());
//     // Convert std::wstring to std::string
//     int titleSize = WideCharToMultiByte(CP_ACP, 0, wTitle.c_str(), -1,
//     NULL, 0, NULL, NULL); int textSize = WideCharToMultiByte(CP_ACP, 0,
//     wText.c_str(), -1, NULL, 0, NULL, NULL); std::string
//     narrowTitle(titleSize, 0); std::string narrowText(textSize, 0);
//     WideCharToMultiByte(CP_ACP, 0, wTitle.c_str(), -1, &narrowTitle[0],
//     titleSize, NULL, NULL); WideCharToMultiByte(CP_ACP, 0, wText.c_str(),
//     -1, &narrowText[0], textSize, NULL, NULL); strncpy_s(nid.szInfoTitle,
//     narrowTitle.c_str(), ARRAYSIZE(nid.szInfoTitle) - 1);
//     strncpy_s(nid.szInfo, narrowText.c_str(), ARRAYSIZE(nid.szInfo) - 1);
//     nid.dwInfoFlags = NIIF_INFO;
//     bool success = Shell_NotifyIcon(NIM_MODIFY, &nid);
//     if (!success) {
//         warn("Notification failed to show! "+GetLastError(),
//         "ShowNotification");
//     }
// }

void reflect::Window::add(reflect::Component &comp) {
  if (customPipeline) {
    reflectWarn("Since a custom pipeline is used, the component will not be "
                "rendered.");
    return;
  }
  reflectDebug("added new component");
  comp.winstance = this->winstance;
  comp.render(this->hwnd, this->hwnd);
}
void reflect::Window::add(reflect::Component &comp, std::string id) {
  if (customPipeline) {
    reflectWarn("Since a custom pipeline is used, the component will not be "
                "rendered.");
    return;
  }
  reflectDebug("added new component");
  if (Components::gchildren[id] == nullptr) {
    Components::gchildren[id] = &comp;

    comp.winstance = this->winstance;
    comp.render(this->hwnd, this->hwnd);
  }
}
void reflect::Window::setSize(Vector2 size_) {
  reflectDebug("resized window");
  size = size_;
  SetWindowPos(hwnd,    // Handle to the window
               NULL,    // Z-order (NULL if not changing the order)
               100,     // New X position
               100,     // New Y position
               size_.x, // New width
               size_.y, // New height
               SWP_NOZORDER | SWP_NOACTIVATE // Flags
  );
}
reflect::Window::Window(HINSTANCE instance, std::string id,
                        WindowCreateInfo *info)
    : winstance(instance), useGL(false), glCtx(nullptr), customPipeline(false),
      beforeRenderLoop(nullptr), callInit(false), renderLoop(nullptr),
      renderRunning(false) {
  initializeObject(REFLECT_OBJECT_WINDOW, REFLECT_OBJECT_COMPONENT);
  reflectDebug("initializing win32 parameters...");
  WNDCLASS wc = {};
  wc.lpfnWndProc = windowProc; // Window procedure
  wc.hInstance = instance;
  wc.lpszClassName = "WindowClass";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClass(&wc);

  // Create window
  reflectDebug("calling CreateWindowEx...");
  if (info && info->customTitleBar) {
    hwnd = CreateWindowEx(0, "WindowClass", wc.lpszClassName,
                          WS_POPUP | WS_THICKFRAME | WS_CLIPCHILDREN |
                              WS_CLIPSIBLINGS,
                          CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, nullptr,
                          nullptr, instance, this);

    customTitleBar = true;
  } else {
    hwnd = CreateWindowEx(
        0, "WindowClass", wc.lpszClassName,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT,
        CW_USEDEFAULT, 100, 100, nullptr, nullptr, instance, this);
    customTitleBar = false;
  }
  RECT rc;

  GetWindowRect(hwnd, &rc);

  // Create a clipping region for the child controls
  // HRGN hrgn = CreateRectRgn(0, 0, rc.right - rc.left, rc.bottom - rc.top);
  HRGN hrgnA = nullptr;
  int regionType = GetWindowRgn(
      hwnd, hrgnA); // Get the window's region (valid or reflectError)
  if (regionType == ERROR) {
    // SetWindowRgn(hwnd, hrgn, TRUE);
  }
  if (hwnd == nullptr) {
    reflectDebug("whoops, hwnd is nullptr");
    MessageBox(nullptr, "Window creation failed!", "Error",
               MB_OK | MB_ICONERROR);
    exit(1);
  }
  reflect::Components::gchildren[id] = this;
}
reflect::Window::Window(HINSTANCE instance, OpenGLContext ctx, std::string id)
    : winstance(instance), useGL(true), glCtx(&ctx), customPipeline(true),
      beforeRenderLoop(nullptr) {
  initializeObject(REFLECT_OBJECT_WINDOW, REFLECT_OBJECT_COMPONENT);
  reflectDebug("initializing win32 parameters...");
  WNDCLASS wc = {};
  wc.lpfnWndProc = windowProc; // Window procedure
  wc.hInstance = instance;
  wc.lpszClassName = "WindowClass";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClass(&wc);

  // Create window
  reflectDebug("calling CreateWindowEx...");
  hwnd = CreateWindowEx(0, "WindowClass", wc.lpszClassName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, nullptr,
                        nullptr, instance, this);

  if (hwnd == nullptr) {
    reflectDebug("whoops, hwnd is nullptr");
    MessageBox(nullptr, "Window creation failed!", "Error",
               MB_OK | MB_ICONERROR);
    exit(1);
  }
  reflect::Components::gchildren[id] = this;
}
reflect::Window::~Window() {
  reflectDebug("releasing memory...");
  if (pRenderTarget) {
    pRenderTarget->Release();
    pRenderTarget = nullptr;
  }
  if (pFactory) {
    pFactory->Release();
    pFactory = nullptr;
  }
  // set to nullptr to avoid dangling pointers
}
void reflect::Window::setTitle(std::string title) {
  reflectDebug("window title set");
  SetWindowText(this->hwnd, title.c_str());
}

void reflect::Window::setVisible(bool flag) {
  reflectDebug("window visible = " + (std::string(flag ? "true" : "false")));
  ShowWindow(hwnd, flag ? SW_SHOW : SW_HIDE);
}
bool reflect::Window::getVisible() { return IsWindowVisible(hwnd); }
reflect::Window::operator HWND() const { return this->hwnd; }
void reflect::Window::setVisible(int cmd) { ShowWindow(hwnd, cmd); }
void reflect::Window::render(HWND &parentHWND, HWND &windowHWND) {
  reflectWarn("Window::render called, the method is intentionally empty "
              "because it is "
              "not a child component!",
              "render");
  // do nothing
}
void reflect::Window::close() { PostMessage(hwnd, WM_DESTROY, 0, 0); };
HDC renderHdc = nullptr;
int reflect::Window::run(void (*func)(Window &win)) {
  reflectInfo("Running window...");
  MSG msg;
  bool isExecuted = false;
  while (true) {
    // Peek for messages, doesn't block
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        break; // Exit the loop when receiving WM_QUIT
      }
      /*  glClearColor(1.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(glHdc);*/
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      // if (openglRendering && currentContext != nullptr && customPipeline &&
      //    renderLoop) {
      //  renderRunning = true;
      //}
    }

    if (customPipeline && openglRendering && !renderThread)
      renderThread = new std::thread([this, func, msg]() {
        while (msg.message != WM_QUIT) {
          if (!renderHdc) {
            renderHdc = GetDC(hwnd);
            this->glCtx->initializeContext(hwnd, renderHdc);
          }
          // Check if the render loop should be executed
          if (callInit) {
            beforeRenderLoop(*this);
            callInit = false;
          }
          //   if (renderRunning) {
          // Perform the rendering task
          if (openglRendering && wglGetCurrentContext() != nullptr &&
              customPipeline && renderLoop) {
            renderLoop(*this);
          }

          // Reset render flag
          //    renderRunning = false;
          // }
        }
      });
    /*if (openglRendering && currentContext != nullptr && customPipeline &&
        renderLoop) {
      renderLoop(*this);
    }*/

    // Perform other tasks here while the message loop is running
    // Example: process background tasks, update UI, etc.
    // This ensures the UI is responsive and other tasks can run concurrently
    if (!isExecuted) {

      func(*this);
      isExecuted = true;
    }
  }
  return static_cast<int>(msg.wParam);
}
void reflect::Window::setBeforeRenderLoop(void (*callback)(Window &)) {
  this->beforeRenderLoop = callback;
}
void reflect::Window::swapBuffers() {
  if (renderHdc)
    SwapBuffers(renderHdc);
}
#elif __linux__
#include "Label.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <string>
reflect::Window::Window(Display *instance) {
  // Initialize the X11 winstance
  winstance = instance;
  if (!winstance) {
    std::cerr << "Cannot open winstance." << std::endl;
    exit(reflectError_linux_x11_not_initialized);
  }

  // Create a window
  screen = DefaultScreen(winstance);
  window = XCreateSimpleWindow(winstance, RootWindow(winstance, screen), 100,
                               100, 400, 200, 1, BlackPixel(winstance, screen),
                               WhitePixel(winstance, screen));

  // Set window title
  XStoreName(winstance, window);

  // Select input events
  XSelectInput(winstance, window, ExposureMask | KeyPressMask);

  // Map (show) the window
  // XMapWindow(winstance, window);

  // Load the font using Xft
  font = XftFontOpenName(winstance, screen, "Ubuntu-20");
  if (!font) {
    std::cerr << "Font not found!" << std::endl;
    exit(reflectError_font_not_found);
  }

  // Create XftDraw for drawing text
  draw = XftDrawCreate(winstance, window, DefaultVisual(winstance, screen),
                       DefaultColormap(winstance, screen));

  // Set color for text (black)

  // Event loop to handle drawing text

  // Cleanup
}
void reflect::Window::SetVisible(bool visible) {
  visible ? XMapWindow(winstance, window)
          : XWithdrawWindow(winstance, window, screen);
}

int reflect::Window::Run() {
  XMapWindow(winstance, window);
  XEvent event;
  while (true) {
    XNextEvent(winstance, &event);

    if (event.xexpose.count == 0) {
      // Clear the window (optional)
      XClearWindow(winstance, window);

      for (Component *child_ : children) {
        Label *child = dynamic_cast<Label *>(child_);
        if (child != nullptr) {
          std::string text = child->GetText();

          const FcChar8 *str = (const FcChar8 *)text.c_str();
          XftColor color;
          XftColorAllocName(winstance, DefaultVisual(winstance, screen),
                            DefaultColormap(winstance, screen), "black",
                            &color);
          XftDrawStringUtf8(draw, &color, font, child->position.X,
                            child->position.Y + 20, (const FcChar8 *)str,
                            text.length());
        }
        /* code */
      }

      // Draw text on the window
    }

    // Exit if the user closes the window
    if (event.xany.type == ClientMessage) {
      break;
    }
  }
  XftDrawDestroy(draw);
  XftFontClose(winstance, font);
  XCloseDisplay(winstance);
  return 0;
}
void reflect::Window::Add(Component *comp) { children.push_back(comp); }
void reflect::Window::SetSize(Vector2 size) {}
void reflect::Window::SetTitle(std::string title) {}
void reflect::Window::SetColor(uint8_t r, uint8_t g, uint8_t b) {}
std::string reflect::Window::GetProperty(std::string property) {
  if (property == "Window") {
    return "true";
  }
  return "";
}
reflect::Window::~Window() {}
#endif