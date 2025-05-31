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
#include "Component.h"
#include "../memory/HeapPool.h"
#include "Canvas.h"
#include "Colors.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#include <string>
reflect::Component::~Component() = default;
reflect::Component::Component()
    : position(Vector2(0, 0)), size(Vector2(0, 0)),
      bgColor(Color3Float(1, 1, 1)), hwnd(nullptr), winstance(nullptr) {
  initializeObject(REFLECT_OBJECT_COMPONENT);
}
// reflect::Component::~Component()  {
//
// }
void reflect::Component::setPosition(Vector2 pos) {
  if (!hwnd) {
    this->position = pos;
  } else {
    this->position = pos;
    SetWindowPos(hwnd, HWND_TOP, pos.x, pos.y, size.x, size.y, SWP_SHOWWINDOW);
  }
}
void reflect::Component::paint() { InvalidateRect(hwnd, nullptr, true); }
void reflect::Component::setClassName(const std::string &className) {
  this->className = "reflect_" + className;
}
void reflect::Component::setWindowName(const std::string &windowName) {
  this->windowName = windowName;
}
void reflect::Component::setStyles(unsigned int styles,
                                   unsigned int extendedStyles) {
  this->styles = styles;
  this->extendedStyles = extendedStyles;
}
reflect::Canvas &reflect::Component::getCanvas() { return *canvas.get(); }
void reflect::Component::onPaint() {};
void reflect::Component::onCreate() {};
HWND &reflect::Component::getParentWindow() { return parentHWND; }
HWND &reflect::Component::getRootWindow() { return windowHWND; }
LRESULT CALLBACK reflect::Component::componentProc(HWND hwnd, UINT uMsg,
                                                   WPARAM wParam,
                                                   LPARAM lParam) {
  Component *pThis =
      reinterpret_cast<Component *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if (uMsg == WM_CREATE) {
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<Component *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->hwnd = hwnd;
    pThis->canvas = std::make_unique<Canvas>(pThis);
    pThis->canvas->render(hwnd, pThis->windowHWND);

    pThis->onCreate();
  }
  if (pThis) {
    switch (uMsg) {

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);

      pThis->onPaint();

      EndPaint(hwnd, &ps);
      return 0;
    }
    }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
void reflect::Component::render(HWND &parentHWND, HWND &windowHWND) {
  this->parentHWND = parentHWND;
  this->windowHWND = windowHWND;
  WNDCLASS wc = {};
  wc.lpszClassName = className.c_str();
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpfnWndProc = componentProc;
  RegisterClass(&wc);
  hwnd = CreateWindowEx(extendedStyles, className.c_str(), windowName.c_str(),
                        styles, position.x, position.y, size.x, size.y,
                        parentHWND, nullptr, GetModuleHandle(nullptr), this);
};

reflect::Color3 reflect::Component::getColor() { return bgColor; }
void reflect::Component::add(Component &comp) {
  // Do nothing
}
void reflect::Component::setVisible(int cmd) {
  // Do nothing
}
void reflect::Component::setVisible(bool cmd) {
  // Do nothing
}
bool reflect::Component::getVisible() { return IsWindowVisible(hwnd); }
void reflect::Component::setSize(Vector2 size) {
  if (!hwnd) {
    this->size = size;
  } else {
    this->size = size;
    SetWindowPos(hwnd, HWND_TOP, position.x, position.y, size.x, size.y,
                 SWP_SHOWWINDOW);
  }
}
void reflect::Component::setColor(Color3 color) { this->bgColor = color; }
void reflect::Component::setColor(Color3Array color) {
  this->bgColor.r = color[0] / 255.0f;
  this->bgColor.g = color[1] / 255.0f;
  this->bgColor.b = color[2] / 255.0f;
}
void reflect::Component::setColor(uint8_t r, uint8_t g, uint8_t b) {
  this->bgColor.r = r;
  this->bgColor.g = g;
  this->bgColor.b = b;
}
reflect::Vector2 reflect::Component::getPosition() { return position; };
reflect::Vector2 reflect::Component::getSize() { return size; };
namespace reflect {
void *Component::operator new(std::size_t size) {
  if (!getHeapPool()) {
    initializeHeapPool(2 * 1024 * 1024);
  }
  void *ptr = getHeapPool()->allocate(size);
  if (!ptr) {
    throw std::bad_alloc(); // Handle allocation failure
  }
  return ptr;
}

void Component::operator delete(void *ptr) {
  if (!getHeapPool()) {
    initializeHeapPool(2 * 1024 * 1024);
  }
  getHeapPool()->deallocate(ptr, sizeof(ptr));
}

void *Component::operator new[](std::size_t size) {
  if (!getHeapPool()) {
    initializeHeapPool(2 * 1024 * 1024);
  }
  void *ptr = getHeapPool()->allocate(size);
  return ptr;
}

void Component::operator delete[](void *ptr) {
  if (!getHeapPool()) {
    initializeHeapPool(2 * 1024 * 1024);
  }
  getHeapPool()->deallocate(ptr, sizeof(ptr));
}
} // namespace reflect
#ifdef __linux__

// std::string reflect::Component::GetProperty(std::string property) {
//     return "";
// }
#endif