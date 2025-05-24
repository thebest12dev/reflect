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
#include "TypeDefinitions.h"
#include <d2d1.h>
#include <dwrite.h>
#include <string>

namespace reflect {
class Label : public TextComponent {
protected:
  HINSTANCE winstance;
  HWND hwnd;

  HINSTANCE parentInstance;
  // HWND parentHWND;
  void registerClass();
  static LRESULT CALLBACK labelProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
  void createResources(ID2D1RenderTarget *rt, IDWriteFactory *dwrite,
                       reflect::Window *win);
  ID2D1HwndRenderTarget *childRenderTarget;
  IDWriteTextFormat *pTextFormat = nullptr;
  ID2D1SolidColorBrush *pBgBrush = nullptr;
  ID2D1SolidColorBrush *pTextBrush = nullptr;

public:
  friend class Component;
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  REFLECT_API void setVisible(bool flag);
  REFLECT_API void add(Component comp);
  REFLECT_API Label(std::string contents, Vector2 pos);
  REFLECT_API void setVisible(int cmd);
  REFLECT_API void setFontSize(int size) override;
  REFLECT_API void setFont(std::string font) override;
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);
  REFLECT_API Color3 getColor();
};
} // namespace reflect