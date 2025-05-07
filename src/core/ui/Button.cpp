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

#ifdef _WIN32
#include "Button.h"
#include "../Utilities.h"
#include "Console.h"
#include "Definitions.h"
#include "TypeDefinitions.h"
#include "Window.h"
#include <commctrl.h>
#include <iostream>
#include <windows.h>

using namespace cinnamontoast::console;
using namespace cinnamontoast::utilities;

/**
 * @brief Sets the font of the button.
 *
 * @param font The font name to set. If "default", the default font is used.
 */
void ctoast Button::setFont(std::string font) {
  fontStr = font;
  if (font == "default") {
    fontStr = DEFAULT_FONT;
  }
}

/**
 * @brief Sets the font size of the button.
 *
 * @param size The font size to set.
 */
void ctoast Button::setFontSize(int size) { fontSize = size; }

WNDPROC originalBtnProc = nullptr;
void ctoast Button::onClick(void (*callback)(Button &)) {
  this->clickCallback = callback;
}
long long CALLBACK ctoast Button::buttonProc(HWND hwnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam,
                                             UINT_PTR uIdSubclass,
                                             DWORD_PTR dwRefData) {
  Button *pThis = nullptr;
  pThis =
      reinterpret_cast<ctoast Button *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (uMsg) {
  case WM_LBUTTONUP: {
    if (pThis->clickCallback) {
      pThis->clickCallback(*pThis);
    }
  }
  }
  return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}
/**
 * @brief Renders the button on the parent window.
 *
 * @param parentHWND The handle to the parent window.
 * @param windowHWND The handle to the window.
 */
void ctoast Button::render(HWND &parentHWND, HWND &windowHWND) {
  if (!IsWindow(parentHWND)) {
    ctoastError("parent HWND is invalid!");
    std::exit(CTOAST_ERROR_WIN_PARENT_HWND_INVALID);
  }
  ctoast Window *window = reinterpret_cast<ctoast Window *>(
      GetWindowLongPtr(windowHWND, GWLP_USERDATA));
  hwnd = CreateWindow(
      "BUTTON",                        // Predefined class for a Button
      text.c_str(),                    // Button text
      WS_VISIBLE | WS_CHILD | BS_FLAT, // Styles: visible and child window
      position.x,
      position.y +
          window->getProperty<int>("customTitleBarSize"), // Position (x, y)
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
  SetWindowSubclass(hwnd, buttonProc, 0, 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  HFONT hFont = ctoast utilities::getFont(fontStr, fontSize);

  if (hFont) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
  }

  HDC hdc = GetDC(hwnd);
  if (hdc) {
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    SIZE textSize;
    GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);

    SetWindowPos(hwnd, NULL, position.x, position.y,
                 textSize.cx + (fontSize * 2), textSize.cy + (fontSize / 2),
                 SWP_NOZORDER | SWP_NOMOVE);

    SelectObject(hdc, oldFont);
    ReleaseDC(hwnd, hdc);
  }
};

/**
 * @brief Sets the visibility of the button.
 *
 * @param flag True to show the button, false to hide it.
 */
void ctoast Button::setVisible(bool flag) {
  ShowWindow(this->hwnd, flag ? SW_SHOW : SW_HIDE);
}

/**
 * @brief Gets the text of the button.
 *
 * @return The text of the button.
 */
std::string ctoast Button::getText() { return text; }

/**
 * @brief Constructs a Button object with the specified text and position.
 *
 * @param text The text to display on the button.
 * @param pos The position of the button.
 */
ctoast Button::Button(std::string text, Vector2 pos)
    : position(pos), size(Vector2(0, 0)), text(text) {
  initializeObject(CTOAST_OBJECT_BUTTON, CTOAST_OBJECT_TEXTCOMPONENT);
}
#elif __linux__
#include "../Utilities.h"
#include "Button.h"
#include "Console.h"
#include "Definitions.h"
#include "TypeDefinitions.h"
#include <commctrl.h>
#include <iostream>
#include <windows.h>

using namespace cinnamontoast::console;
using namespace cinnamontoast::utilities;

/**
 * @brief Sets the font of the button.
 *
 * @param font The font name to set. If "default", the default font is used.
 */
void ctoast Button::setFont(std::string font) {
  fontStr = font;
  if (font == "default") {
    fontStr = DEFAULT_FONT;
  }
}

/**
 * @brief Sets the font size of the button.
 *
 * @param size The font size to set.
 */
void ctoast Button::setFontSize(int size) { fontSize = size; }

void ctoast Button::onClick(void (*callback)(Button &)) {
  this->clickCallback = callback;
}

/**
 * @brief Renders the button on the parent window.
 *
 * @param parentWindow The handle to the parent window.
 * @param windowWindow The handle to the window.
 */
void ctoast Button::render(XWindow &parentWindow, XWindow &windowWindow) {
  // stub!
};

/**
 * @brief Sets the visibility of the button.
 *
 * @param flag True to show the button, false to hide it.
 */
void ctoast Button::setVisible(bool flag) {}

/**
 * @brief Gets the text of the button.
 *
 * @return The text of the button.
 */
std::string ctoast Button::getText() { return text; }

/**
 * @brief Constructs a Button object with the specified text and position.
 *
 * @param text The text to display on the button.
 * @param pos The position of the button.
 */
ctoast Button::Button(std::string text, Vector2 pos)
    : position(pos), size(Vector2(0, 0)), text(text) {
  initializeObject(CTOAST_OBJECT_BUTTON, CTOAST_OBJECT_TEXTCOMPONENT);
}
#endif