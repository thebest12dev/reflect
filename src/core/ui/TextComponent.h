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
#include "Colors.h"
#include "Component.h"
#include "Vector2.h"
#include <cstdint>
#include <string>

#include "TypeDefinitions.h"
namespace reflect {
class TextComponent : public Component {
protected:
  ApplicationHandle winstance;
  WindowHandle hwnd;

  ApplicationHandle parentInstance;
  // Window parentWindow;

  Vector2 position;
  Vector2 size;
  Color3 color;
  std::string text;
  std::string fontStr;
  int fontSize;

public:
  REFLECT_API TextComponent();
  REFLECT_API virtual void render(WindowHandle &parentHWND,
                                  WindowHandle &windowHWND) = 0;
  REFLECT_API void setVisible(bool flag);
  REFLECT_API void add(Component &comp);
  REFLECT_API void setVisible(int cmd);
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);
  REFLECT_API void setColor(Color3 color);
  REFLECT_API void setColor(Color3Array color);
  REFLECT_API virtual void setFontSize(int size);
  REFLECT_API virtual void setFont(std::string font);
  REFLECT_API void setText(std::string text);
  REFLECT_API virtual std::string getText();
  REFLECT_API ~TextComponent();
  friend class Label;
  friend class Button;
};
} // namespace reflect