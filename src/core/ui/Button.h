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
#pragma once
#include "TextComponent.h"
#ifdef _WIN32
#include "Component.h"
#include <cstdint>
#include <windows.h>

#include "TypeDefinitions.h"
#include "Vector2.h"
#include <string>
namespace CinnamonToast {
class Button : public TextComponent {
protected:
  HINSTANCE winstance;
  HWND hwnd;

  HINSTANCE parentInstance;
  // HWND parentHWND;

  Vector2 position;
  Vector2 size;
  void (*clickCallback)(Button &);

private:
  std::string text;
  std::string fontStr;
  int fontSize;
  CTOAST_API static LRESULT CALLBACK buttonProc(HWND hwnd, UINT uMsg,
                                                WPARAM wParam, LPARAM lParam,
                                                UINT_PTR uIdSubclass,
                                                DWORD_PTR dwRefData);

public:
  friend class Component;
  CTOAST_API void render(HWND &parentHWND, HWND &windowHWND);
  CTOAST_API void setVisible(bool flag);
  CTOAST_API void add(Component comp);
  CTOAST_API Button(std::string contents, Vector2 pos);
  CTOAST_API void setVisible(int cmd);
  CTOAST_API void setFontSize(int size);
  CTOAST_API void setFont(std::string font);
  CTOAST_API void setText(std::string text);
  CTOAST_API std::string getText();
  CTOAST_API void onClick(void (*callback)(Button &));
  CTOAST_API void setColor(uint8_t r, uint8_t g, uint8_t b);
};
} // namespace CinnamonToast
#elif __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>

#include "../TypeDefinitions.h"
#include "Component.h"
#include "Vector2.h"
#include <string>
namespace CinnamonToast {
class Button : public Component {
protected:
  Display *winstance;
  Window window;

  Display *parentInstance;
  // Window parentWindow;

  Vector2 position;
  Vector2 size;

private:
  std::string text;
  std::string fontStr;
  int fontSize;

public:
  friend class Component;
  CTOAST_API void render(Window &parentWindow, Window &windowWindow);
  CTOAST_API void setVisible(bool flag);
  CTOAST_API void add(Component comp);
  CTOAST_API Button(std::string contents, Vector2 pos);
  CTOAST_API void setVisible(int cmd);
  CTOAST_API void setFontSize(int size);
  CTOAST_API void setFont(std::string font);
  CTOAST_API void setColor(uint8_t r, uint8_t g, uint8_t b);
};
} // namespace CinnamonToast

#endif
