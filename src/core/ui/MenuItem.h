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
#include "MenuBar.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include <string>

namespace reflect {
class MenuItem : public TextComponent {
protected:
  ApplicationHandle winstance;
#ifdef _WIN32
  HMENU hmenu;
#endif

  ApplicationHandle parentInstance;
  // WindowHandle parentHWND;

  // Vector2 position;
  Vector2 size;

private:
  std::string text;
  std::string fontStr;
  int fontSize;

public:
  friend class Component;
  REFLECT_API void render(WindowHandle &parentHWND, WindowHandle &windowHWND);
  REFLECT_API void setVisible(bool flag);
  REFLECT_API void add(MenuItem &comp);
  REFLECT_API void setText(std::string contents);
  REFLECT_API MenuItem(std::string contents);
  REFLECT_API MenuItem();
  REFLECT_API void setVisible(int cmd);
  REFLECT_API void setFontSize(int size) override;
  REFLECT_API void setFont(std::string font) override;
  REFLECT_API std::string getText() override;
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);
  friend class MenuBar;
};
} // namespace reflect