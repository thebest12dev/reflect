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
#undef byte
#include <GL/gl.h>

#include "Component.h"
#include "Notification.h"
#include "OpenGLContext.h"

#include "TypeDefinitions.h"

#include <cstdint>
#include <string>

#include "Vector2.h"
namespace CinnamonToast {
class Window : public Component {
protected:
  HINSTANCE winstance;
  HWND hwnd;

private:
  bool useGL;
  bool customPipeline;
  OpenGLContext *glCtx;

public:
  COMPONENT_DECL(Window);
  CTOAST_API Window(HINSTANCE instance);
  CTOAST_API Window(HINSTANCE instance, OpenGLContext ctx);
  CTOAST_API void setTitle(std::string title);
  // // void SetSize(Vector2 dim);
  CTOAST_API static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg,
                                                WPARAM wParam, LPARAM lParam);

  CTOAST_API void add(Component &comp, std::string id);
  CTOAST_API bool showNotification(CinnamonToast::Notification &notif);
  CTOAST_API int run(void (*func)(Window &win));
  CTOAST_API void close();
  CTOAST_API operator HWND() const;

  friend class Component;
  friend class Label;
  friend class OpenGLContext;
};
} // namespace CinnamonToast

#elif __linux__
#include "../TypeDefinitions.h"
#include "Component.h"
#include "Vector2.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <string>
typedef Window XWindow;
namespace CinnamonToast {
class Window : public Component {
protected:
  Display *winstance;
  XWindow window;
  int screen;
  XftFont *font;
  XftDraw *draw;

private:
  uint8_t bgColor[3];

public:
  CTOAST_API Window(Display *instance);
  CTOAST_API void SetTitle(std::string title);
  // // void SetSize(Vector2 dim);
  CTOAST_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
  CTOAST_API void SetVisible(bool flag);
  CTOAST_API void Add(Component *comp);
  CTOAST_API std::string GetProperty(std::string property);
  CTOAST_API void SetSize(Vector2 size);
  CTOAST_API void SetVisible(int cmd);
  CTOAST_API int Run();
  CTOAST_API ~Window();
  friend class Component;
  friend class Label;
};
} // namespace CinnamonToast
#endif
