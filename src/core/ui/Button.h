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
#include "Component.h"
#include "TextComponent.h"
#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <string>
namespace reflect {
class Button : public TextComponent {
protected:
  ApplicationHandle winstance;
  WindowHandle hwnd;

  ApplicationHandle parentInstance;
  // WindowHandle parentHWND;

  Vector2 position;
  Vector2 size;
  void (*clickCallback)(Button &);

private:
  std::string text;
  std::string fontStr;
  int fontSize;
#ifdef _WIN32
  static LRESULT CALLBACK buttonProc(WindowHandle hwnd, UINT uMsg,
                                     WPARAM wParam, LPARAM lParam,
                                     UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
#endif

public:
  friend class Component;
  REFLECT_API void render(WindowHandle &parentHWND, WindowHandle &windowHWND);
  REFLECT_API void setVisible(bool flag);
  REFLECT_API void add(Component comp);
  REFLECT_API Button(std::string contents, Vector2 pos);
  REFLECT_API void setVisible(int cmd);
  REFLECT_API void setFontSize(int size);
  REFLECT_API void setFont(std::string font);
  REFLECT_API void setText(std::string text);
  REFLECT_API std::string getText();
  REFLECT_API void onClick(void (*callback)(Button &));
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);
};
} // namespace reflect