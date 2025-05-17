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
#include <Windows.h>
#include <d2d1.h>
#undef byte
#endif
#include "Component.h"
#include "Notification.h"
#include "OpenGLContext.h"
#include "TypeDefinitions.h"
#include <condition_variable>

#include <mutex>
#include <thread>

#include <cstdint>
#include <string>

#include "Vector2.h"
#include <any>
#include <functional>
#include <unordered_map>
namespace reflect {
enum WindowStyle { STYLE_DARK_TITLE_BAR };
struct WindowCreateInfo {
  bool customTitleBar = false;
};
class Window : public Component {
private:
  bool useGL;
  bool customPipeline;
  OpenGLContext *glCtx;
  bool closeHovering;
  bool customTitleBar;
  void (*renderLoop)(Window &);
  void (*beforeRenderLoop)(Window &);

protected:
  ApplicationHandle winstance;
  WindowHandle hwnd;
  // std::map<char, bool> pressedKeys;
  std::thread *renderThread = nullptr;
  std::condition_variable renderCondition;
  std::mutex renderMutex;

  std::unordered_map<std::string, std::function<std::any()>> propertyMap = {
    {"customTitleBarSize",
     [this]() -> std::any {
       if (customTitleBar) {
         return 40;
       } else {
         return 0;
       }
     }},
#ifdef _WIN32
    {"direct2DRenderTarget",
     [this]() -> std::any {
       return pRenderTarget; // ID2D1HwndRenderTarget
     }},
    {"direct2DFactory", [this]() -> std::any {
       return pFactory; // ID2D1Factory
     }};
#endif
};
bool renderRunning;
bool callInit;
#ifdef _WIN32
ID2D1HwndRenderTarget *pRenderTarget;
ID2D1Factory *pFactory;
static LRESULT CALLBACK windowProc(WindowHandle hwnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam);
void initializeDirect2D();
#endif

public:
COMPONENT_DECL(Window);
REFLECT_API Window(ApplicationHandle instance, std::string id,
                   WindowCreateInfo *info = nullptr);
REFLECT_API Window(ApplicationHandle instance, OpenGLContext ctx,
                   std::string id);
REFLECT_API void setTitle(std::string title);
REFLECT_API void addStyle(WindowStyle style);
// // void SetSize(Vector2 dim);

template <typename T>

REFLECT_API T getProperty(std::string property) {
  std::any anyType = propertyMap[property]();
  return std::any_cast<T>(anyType);
};
REFLECT_API void setBeforeRenderLoop(void (*callback)(Window &));
REFLECT_API void swapBuffers();
REFLECT_API bool isKeyPressed(char key);
REFLECT_API void add(Component &comp, std::string id);
REFLECT_API bool showNotification(reflect::Notification &notif);
REFLECT_API int run(void (*func)(Window &win));
REFLECT_API void setRenderLoop(void (*loop)(Window &));
REFLECT_API void close();
REFLECT_API operator WindowHandle() const;

friend class Component;
friend class Label;
friend class OpenGLContext;
}; // namespace reflect
}
; // namespace reflect
