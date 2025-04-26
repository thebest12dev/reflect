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
#include "Label.h"
#include "../Utilities.h"
#include "Console.h"
#include "Definitions.h"
#include "TypeDefinitions.h"
#include <iostream>
#include <windows.h>

using namespace CinnamonToast::Console;
using namespace CinnamonToast::Utilities;
void ctoast Label::setFont(std::string font) {
  fontStr = font;
  if (font == "default") {
    fontStr = DEFAULT_FONT;
  }
}
void ctoast Label::setFontSize(int size) { fontSize = size; }
std::string ctoast Label::getText() { return text; }

void ctoast Label::render(HWND &parentHWND, HWND &windowHWND) {
  if (!IsWindow(parentHWND)) {
    ctoastError("parent HWND is invalid!");
    std::exit(CTOAST_ERROR_WIN_PARENT_HWND_INVALID);
  }

  // Create the label window
  hwnd = CreateWindow("STATIC",              // Predefined class for a label
                      text.c_str(),          // Label text
                      WS_VISIBLE | WS_CHILD, // Styles: visible and child window
                      position.x, position.y, // Position (x, y)
                      size.x, size.y,         // Size (width, height)
                      parentHWND,             // Parent window handle
                      NULL,                   // No menu or child ID
                      winstance,              // Instance handle
                      NULL                    // Additional application data
  );

  if (!hwnd) {
    std::cout << getLastErrorAsString();
    return;
  }

  // Create a custom font
  HFONT hFont = ctoast Utilities::getFont(fontStr, fontSize);

  if (hFont) {
    // Set the font for the label
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
  }

  // Resize the label based on the text and font size
  HDC hdc = GetDC(hwnd);
  if (hdc) {
    // Select the font into the device context
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // Calculate the size of the text
    SIZE textSize;
    GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);

    // Update the label's size
    SetWindowPos(hwnd,                         // Handle to the label
                 NULL,                         // No z-order change
                 position.x, position.y,       // X and Y (ignored here)
                 textSize.cx + (fontSize * 2), // New width (padding added)
                 textSize.cy,                  // New height
                 SWP_NOZORDER | SWP_NOMOVE // Don't change z-order or position
    );

    // Restore the old font and release the device context
    SelectObject(hdc, oldFont);
    ReleaseDC(hwnd, hdc);
  }
}

void ctoast Label::setVisible(bool flag) {
  ShowWindow(this->hwnd, flag ? SW_SHOW : SW_HIDE);
}
ctoast Label::Label(std::string text, Vector2 pos)
    : position(pos), size(Vector2(0, 0)), text(text) {}
#elif __linux__
#include "../Console.h"
#include "../Definitions.h"
#include "../TypeDefinitions.h"
#include "../Utilities.h"
#include "Label.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>

// XftDrawStringUtf8(draw, &color, font, 50, 100, (const FcChar8 *)"Hello, X11
// with Font!", 21);
ctoast Label::Label(std::string text, Vector2 pos)
    : position(pos), size(Vector2(0, 0)), text(text) {}
void ctoast Label::SetVisible(bool flag) { this->visible = true; }
void ctoast Label::SetFont(std::string font) { fontStr = font; }
void ctoast Label::SetFontSize(int fontSize_) { fontSize = fontSize_; }
ctoast Label::Label()
    : position(Vector2(0, 0)), size(Vector2(0, 0)), text("") {};
std::string ctoast Label::GetText() { return text; }
std::string ctoast Label::GetProperty(std::string property) {
  if (property == "Text") {
    return text;
  } else {
    return "";
  }
}
#endif