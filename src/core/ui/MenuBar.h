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
#ifdef _WIN32
#include "Component.h"
#include "MenuItem.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include <string>

namespace reflect {
class MenuBar : public Component {
protected:
  HINSTANCE winstance;
  HMENU hmenu;

  HINSTANCE parentInstance;
  // HWND parentHWND;

  // Vector2 position;
  /*Vector2 size;*/

private:
public:
  friend class Component;
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  // REFLECT_API void SetVisible(bool flag);
  REFLECT_API void add(Component &comp);
  REFLECT_API MenuBar();
  // REFLECT_API void SetVisible(int cmd);
  // REFLECT_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
};
} // namespace reflect

#elif __linux__
#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>

#include "Component.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <string>
namespace reflect {
class MenuBar : public Component {
protected:
  Display *winstance;
  Menu hmenu;

  HINSTANCE parentInstance;
  // HWND parentHWND;

  // Vector2 position;
  /*Vector2 size;*/

private:
public:
  friend class Component;
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  // REFLECT_API void SetVisible(bool flag);
  REFLECT_API void add(Component &comp);
  REFLECT_API MenuBar();
  // REFLECT_API void SetVisible(int cmd);
  // REFLECT_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
};
} // namespace reflect

#endif