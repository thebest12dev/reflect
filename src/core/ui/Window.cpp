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
#include "Window.h"
#include "../Utilities.h"
#include "Component.h"
#include "Components.h"

#include "Console.h"
#include "Label.h"
#include "MenuBar.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <iostream>
#include <string>
#ifdef _WIN32
#include "Notification.h"
#include <cstdio>
#include <d2d1.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
using namespace cinnamontoast::console;
// Direct2D-specific members
ID2D1Factory *pFactory = nullptr;
ID2D1HwndRenderTarget *pRenderTarget = nullptr;
// Due to floating point operations, may not produce exact color
void ctoast Window::addStyle(WindowStyle style) {
  switch (style) {
  case STYLE_DARK_TITLE_BAR:
    ctoastDebug("enabling dark title bar...")
        // Enable dark mode title bar
        BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark,
                          sizeof(dark));

    // Enable rounded corners
    DWM_WINDOW_CORNER_PREFERENCE cornerPreference = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
                          &cornerPreference, sizeof(cornerPreference));

    // Enable Mica background
    DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType,
                          sizeof(backdropType));
  }
}
void ctoast Window::setColor(uint8_t r, uint8_t g, uint8_t b) {
  this->bgColor[0] = r / 255.0f;
  this->bgColor[1] = g / 255.0f;
  this->bgColor[2] = b / 255.0f;
}
cinnamontoast::Vector2 ctoast Window::getSize() { return size; }
// Due to floating point operations, may not produce exact color
void ctoast Window::setColor(Color3 color) {
  this->bgColor.r = color.r / 255.0f;
  this->bgColor.g = color.g / 255.0f;
  this->bgColor.b = color.b / 255.0f;
}
ctoast Color3 ctoast Window::getColor() {
  return {static_cast<unsigned char>(bgColor[0] * 255),
          static_cast<unsigned char>(bgColor[1] * 255),
          static_cast<unsigned char>(bgColor[2] * 255)};
}

// Due to floating point operations, may not produce exact color
void ctoast Window::setColor(Color3Array color) {
  this->bgColor[0] = color[0] / 255.0f;
  this->bgColor[1] = color[1] / 255.0f;
  this->bgColor[2] = color[2] / 255.0f;
}
void ctoast Window::setRenderLoop(void (*loop)(Window &)) { renderLoop = loop; }
void initializeDirect2D(HWND hwnd) {
  if (!pFactory && !pRenderTarget) {

    ctoastDebug("initializing Direct2D...");
    // Create the Direct2D factory
    if (!pFactory) {
      ctoastDebug("creating d2d1 factory...");
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
    }

    // Get window dimensions
    RECT rc;
    GetClientRect(hwnd, &rc);

    // Create the render target
    ctoastDebug("creating renderer target...");
    if (!pRenderTarget) {
      pFactory->CreateHwndRenderTarget(
          D2D1::RenderTargetProperties(),
          D2D1::HwndRenderTargetProperties(
              hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
          &pRenderTarget);
    }
  }
};

namespace {
float getDPIScaleForWindow(HWND hwnd) {
  HDC hdc = GetDC(hwnd);
  float dpi = GetDeviceCaps(hdc, LOGPIXELSX); // or LOGPIXELSY
  ReleaseDC(hwnd, hdc);
  return dpi / 96.0f; // 96 is the default DPI baseline
}

void safeResize(ID2D1HwndRenderTarget *pRenderTarget, D2D1_SIZE_U size) {
  __try {
    HRESULT hr = pRenderTarget->Resize(size);
    if (FAILED(hr)) {
      // Handle HRESULT errors if needed
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    DWORD exceptionCode = GetExceptionCode();
    // Use printf instead of std::cout for SEH restrictions
    std::printf("[WARN] [safeResize]: Exception occurred while resizing "
                "Direct2D render target: 0x%08X\n",
                exceptionCode);
  }
}
} // namespace
bool openglRendering = false, direct2dRendering = false, firstUpdate = true;
HGLRC currentContext = nullptr;
HDC glHdc = nullptr;
LRESULT CALLBACK ctoast Window::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                           LPARAM lParam) {
  cinnamontoast::Window *pThis = nullptr;

  if (uMsg == WM_CREATE) {
    ctoastDebug("message type: WM_CREATE");
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<ctoast Window *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

    // Initialize Direct2D
    if (!pThis->useGL) {
      ctoastDebug("calling initializeDirect2D...");
      initializeDirect2D(hwnd);
    }

  } else {
    pThis = reinterpret_cast<ctoast Window *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }

  if (pThis) {

    // Delegate the handling of messages to the instance

    switch (uMsg) {
    case WM_DESTROY:
      ctoastDebug("message type: WM_DESTROY");
      PostQuitMessage(0);
      ctoastInfo("Exiting...");
      return 0;
    // case WM_KEYDOWN: {
    //   UINT scanCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
    //   char ch = (char)scanCode;
    //   std::cout << ch << std::endl;
    //   // wParam contains the virtual key code of the key that was pressed

    //  pThis->pressedKeys[ch] = true;
    //  return 0;
    //}
    // case WM_KEYUP: {

    //  UINT scanCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
    //  char ch = (char)scanCode;

    //  // wParam contains the virtual key code of the key that was pressed

    //  pThis->pressedKeys[ch] = false;
    //  return 0;
    //}
    case WM_SIZE: {

      if (pRenderTarget) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = {static_cast<UINT32>(rc.right - rc.left),
                            static_cast<UINT32>(rc.bottom - rc.top)};
        ::safeResize(pRenderTarget, size);
      }
      // Set the clear color

      InvalidateRect(hwnd, NULL,
                     TRUE); // Mark the entire window as needing a repaint
      break;
    }
    case WM_MOVE:
      InvalidateRect(hwnd, NULL,
                     TRUE); // Mark the entire window as needing a repaint
      break;
    case WM_SIZING: {
      if (pThis->beforeRenderLoop)
        pThis->callInit = true;
      break;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      if (!openglRendering && firstUpdate) {
        glHdc = GetDC(hwnd);
        if (pThis->useGL) {
          pThis->glCtx->initializeContext(*pThis, glHdc);
        }
        currentContext = wglGetCurrentContext();
        ctoastDebug("checking if OpenGL is initialized...");
        if (currentContext) {
          ctoastDebug("context found, rendering with OpenGL...");
          openglRendering = true;
          wglDeleteContext(currentContext);
          pThis->callInit = true;
        } else {
          ctoastDebug("OpenGL context not found, falling back to Direct2D...");
          direct2dRendering = true;
        }
        firstUpdate = false;
      } /*else {
        currentContext = wglGetCurrentContext();
      }*/

      if (currentContext != nullptr && openglRendering) {

      } else if (!openglRendering && direct2dRendering) {

        if (!pRenderTarget) {
          initializeDirect2D(hwnd);
        }

        // Create a clipping region for the child controls
        HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
        int regionType = GetWindowRgn(
            hwnd, hrgn); // Get the window's region (valid or ctoastError)
        if (regionType != ERROR) {
          RECT controlRect;
          HWND hChild = GetWindow(hwnd, GW_CHILD); // Get the first child window

          while (hChild != NULL) {
            GetWindowRect(hChild, &controlRect);
            MapWindowPoints(NULL, hwnd, (LPPOINT)&controlRect,
                            2); // Convert to client coordinates

            // Exclude the child control region from the painting area
            ExcludeClipRect(hdc, controlRect.left, controlRect.top,
                            controlRect.right, controlRect.bottom);

            hChild =
                GetNextWindow(hChild, GW_HWNDNEXT); // Get the next child window
          }
        }

        // Start Direct2D rendering
        pRenderTarget->BeginDraw();

        // Set background color (this will not cover child control areas due to
        // ExcludeClipRect)
        pRenderTarget->Clear(D2D1::ColorF(pThis->bgColor[0], pThis->bgColor[1],
                                          pThis->bgColor[2]));

        // End drawing
        HRESULT hr = pRenderTarget->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET) {
          // Handle device loss
          pRenderTarget->Release();
          pRenderTarget = nullptr;
          initializeDirect2D(hwnd);
        }

        // Clean up
        DeleteObject(hrgn);
      }

      EndPaint(hwnd, &ps);

      break;
    }
    }
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
bool renderContextInitialized = false;
bool ctoast Window::isKeyPressed(char key) {
  return GetAsyncKeyState(key) & 0x8000;
};
bool ctoast Window::showNotification(Notification &notif) {
  if (!IsWindow(hwnd)) {
    ctoastError("Window handle is invalid!");
    return false;
  }
  NOTIFYICONDATA nid = {};
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 32067;
  nid.uFlags = NIF_INFO | NIF_MESSAGE | NIF_ICON | NIF_TIP;
  nid.uCallbackMessage =
      WM_USER + 1; // Custom message ID for notification events

  // Load an icon for the notification
  nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);

  strcpy_s(nid.szInfo, notif.text.c_str());
  strcpy_s(nid.szTip, "Tooltip");
  strcpy_s(nid.szInfoTitle, notif.title.c_str());
  // wcsncpy_s(nid.szInfoTitle, wTitle.c_str(), ARRAYSIZE(nid.szInfoTitle) - 1);
  // wcsncpy_s(nid.szInfo, wText.c_str(), ARRAYSIZE(nid.szInfo) - 1);

  // std::wstring tooltip = L"Notification Tooltip";
  /*wcsncpy_s(nid.szTip, tooltip.c_str(), ARRAYSIZE(nid.szTip) - 1);*/

  nid.dwInfoFlags = NIIF_INFO;
  if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
    ctoastError("Failed to add notification icon! (" +
                std::to_string(GetLastError()) + ")");
    return false;
  }

  // Modify the notification icon to show the balloon tip
  /*if (!Shell_NotifyIcon(NIM_MODIFY, &nid)) {
    error("Failed to modify notification icon! (" +
              std::to_string(GetLastError()) + ")",
          "showNotification");
  }*/
  return true;
}

// void ctoast Window::ShowNotification(Notification&  notif) {
//     NOTIFYICONDATA nid = {};
//     nid.cbSize = sizeof(NOTIFYICONDATA);
//     nid.hWnd = hwnd;
//     nid.uID = 32067;
//     nid.uFlags = NIF_INFO;
//     // Convert std::string to std::wstring
//     std::wstring wTitle = std::wstring(notif.title.begin(),
//     notif.title.end()); std::wstring wText = std::wstring(notif.text.begin(),
//     notif.text.end());
//     // Convert std::wstring to std::string
//     int titleSize = WideCharToMultiByte(CP_ACP, 0, wTitle.c_str(), -1, NULL,
//     0, NULL, NULL); int textSize = WideCharToMultiByte(CP_ACP, 0,
//     wText.c_str(), -1, NULL, 0, NULL, NULL); std::string
//     narrowTitle(titleSize, 0); std::string narrowText(textSize, 0);
//     WideCharToMultiByte(CP_ACP, 0, wTitle.c_str(), -1, &narrowTitle[0],
//     titleSize, NULL, NULL); WideCharToMultiByte(CP_ACP, 0, wText.c_str(), -1,
//     &narrowText[0], textSize, NULL, NULL); strncpy_s(nid.szInfoTitle,
//     narrowTitle.c_str(), ARRAYSIZE(nid.szInfoTitle) - 1);
//     strncpy_s(nid.szInfo, narrowText.c_str(), ARRAYSIZE(nid.szInfo) - 1);
//     nid.dwInfoFlags = NIIF_INFO;
//     bool success = Shell_NotifyIcon(NIM_MODIFY, &nid);
//     if (!success) {
//         warn("Notification failed to show! "+GetLastError(),
//         "ShowNotification");
//     }
// }
void ctoast Window::add(ctoast Component &comp) {
  if (customPipeline) {
    ctoastWarn("Since a custom pipeline is used, the component will not be "
               "rendered.");
    return;
  }
  ctoastDebug("added new component");
  comp.winstance = this->winstance;
  comp.render(this->hwnd, this->hwnd);
}
void ctoast Window::add(ctoast Component &comp, std::string id) {
  if (customPipeline) {
    ctoastWarn("Since a custom pipeline is used, the component will not be "
               "rendered.");
    return;
  }
  ctoastDebug("added new component");
  if (Components::gchildren[id] == nullptr) {
    Components::gchildren[id] = &comp;

    comp.winstance = this->winstance;
    comp.render(this->hwnd, this->hwnd);
  }
}
void ctoast Window::setSize(Vector2 size_) {
  ctoastDebug("resized window");
  size = size_;
  SetWindowPos(hwnd,    // Handle to the window
               NULL,    // Z-order (NULL if not changing the order)
               100,     // New X position
               100,     // New Y position
               size_.x, // New width
               size_.y, // New height
               SWP_NOZORDER | SWP_NOACTIVATE // Flags
  );
}
ctoast Window::Window(HINSTANCE instance, std::string id)
    : winstance(instance), useGL(false), glCtx(nullptr), customPipeline(false),
      beforeRenderLoop(nullptr), callInit(false), renderLoop(nullptr),
      renderRunning(false) {
  initializeObject(CTOAST_OBJECT_WINDOW, CTOAST_OBJECT_COMPONENT);
  ctoastDebug("initializing win32 parameters...");
  WNDCLASS wc = {};
  wc.lpfnWndProc = windowProc; // Window procedure
  wc.hInstance = instance;
  wc.lpszClassName = "WindowClass";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClass(&wc);

  // Create window
  ctoastDebug("calling CreateWindowEx...");
  hwnd = CreateWindowEx(0, "WindowClass", wc.lpszClassName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, nullptr,
                        nullptr, instance, this);

  if (hwnd == nullptr) {
    ctoastDebug("whoops, hwnd is nullptr");
    MessageBox(nullptr, "Window creation failed!", "Error",
               MB_OK | MB_ICONERROR);
    exit(1);
  }
  cinnamontoast::Components::gchildren[id] = this;
}
ctoast Window::Window(HINSTANCE instance, OpenGLContext ctx, std::string id)
    : winstance(instance), useGL(true), glCtx(&ctx), customPipeline(true),
      beforeRenderLoop(nullptr) {
  initializeObject(CTOAST_OBJECT_WINDOW, CTOAST_OBJECT_COMPONENT);
  ctoastDebug("initializing win32 parameters...");
  WNDCLASS wc = {};
  wc.lpfnWndProc = windowProc; // Window procedure
  wc.hInstance = instance;
  wc.lpszClassName = "WindowClass";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  RegisterClass(&wc);

  // Create window
  ctoastDebug("calling CreateWindowEx...");
  hwnd = CreateWindowEx(0, "WindowClass", wc.lpszClassName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, nullptr,
                        nullptr, instance, this);

  if (hwnd == nullptr) {
    ctoastDebug("whoops, hwnd is nullptr");
    MessageBox(nullptr, "Window creation failed!", "Error",
               MB_OK | MB_ICONERROR);
    exit(1);
  }
  cinnamontoast::Components::gchildren[id] = this;
}
ctoast Window::~Window() {
  ctoastDebug("releasing memory...");
  if (pRenderTarget) {
    pRenderTarget->Release();
  }
  if (pFactory) {
    pFactory->Release();
  }
  // set to nullptr to avoid dangling pointers
  pRenderTarget = nullptr;
  pFactory = nullptr;
}

void ctoast Window::setTitle(std::string title) {
  ctoastDebug("window title set");
  SetWindowText(this->hwnd, title.c_str());
}

void ctoast Window::setVisible(bool flag) {
  ctoastDebug("window visible = " + (std::string(flag ? "true" : "false")));
  ShowWindow(hwnd, flag ? SW_SHOW : SW_HIDE);
}
bool ctoast Window::getVisible() { return IsWindowVisible(hwnd); }
ctoast Window::operator WindowHandle() const { return this->hwnd; }
void ctoast Window::setVisible(int cmd) { ShowWindow(hwnd, cmd); }
void ctoast Window::render(HWND &parentHWND, HWND &windowHWND) {
  ctoastWarn(
      "Window::render called, the method is intentionally empty because it is "
      "not a child component!",
      "render");
  // do nothing
}
void ctoast Window::close() { PostMessage(hwnd, WM_DESTROY, 0, 0); };
HDC renderHdc = nullptr;
int ctoast Window::run(void (*func)(Window &win)) {
  ctoastInfo("Running window...");
  MSG msg;
  bool isExecuted = false;
  while (true) {
    // Peek for messages, doesn't block
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        break; // Exit the loop when receiving WM_QUIT
      }
      /*  glClearColor(1.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(glHdc);*/
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      // if (openglRendering && currentContext != nullptr && customPipeline &&
      //    renderLoop) {
      //  renderRunning = true;
      //}
    }

    if (customPipeline && openglRendering && !renderThread)
      renderThread = new std::thread([this, func, msg]() {
        while (msg.message != WM_QUIT) {
          if (!renderHdc) {
            renderHdc = GetDC(hwnd);
            this->glCtx->initializeContext(hwnd, renderHdc);
          }
          // Check if the render loop should be executed
          if (callInit) {
            beforeRenderLoop(*this);
            callInit = false;
          }
          //   if (renderRunning) {
          // Perform the rendering task
          if (openglRendering && wglGetCurrentContext() != nullptr &&
              customPipeline && renderLoop) {
            renderLoop(*this);
          }

          // Reset render flag
          //    renderRunning = false;
          // }
        }
      });
    /*if (openglRendering && currentContext != nullptr && customPipeline &&
        renderLoop) {
      renderLoop(*this);
    }*/

    // Perform other tasks here while the message loop is running
    // Example: process background tasks, update UI, etc.
    // This ensures the UI is responsive and other tasks can run concurrently
    if (!isExecuted) {

      func(*this);
      isExecuted = true;
    }
  }
  return static_cast<int>(msg.wParam);
}
void ctoast Window::setBeforeRenderLoop(void (*callback)(Window &)) {
  this->beforeRenderLoop = callback;
}
void ctoast Window::swapBuffers() {
  if (renderHdc)
    SwapBuffers(renderHdc);
}
#elif __linux__
#include "Label.h"
#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <string>
ctoast Window::Window(Display *instance) {
  // Initialize the X11 winstance
  winstance = instance;
  if (!winstance) {
    std::cerr << "Cannot open winstance." << std::endl;
    exit(ctoastError_linux_x11_not_initialized);
  }

  // Create a window
  screen = DefaultScreen(winstance);
  window = XCreateSimpleWindow(winstance, RootWindow(winstance, screen), 100,
                               100, 400, 200, 1, BlackPixel(winstance, screen),
                               WhitePixel(winstance, screen));

  // Set window title
  XStoreName(winstance, window);

  // Select input events
  XSelectInput(winstance, window, ExposureMask | KeyPressMask);

  // Map (show) the window
  // XMapWindow(winstance, window);

  // Load the font using Xft
  font = XftFontOpenName(winstance, screen, "Ubuntu-20");
  if (!font) {
    std::cerr << "Font not found!" << std::endl;
    exit(ctoastError_font_not_found);
  }

  // Create XftDraw for drawing text
  draw = XftDrawCreate(winstance, window, DefaultVisual(winstance, screen),
                       DefaultColormap(winstance, screen));

  // Set color for text (black)

  // Event loop to handle drawing text

  // Cleanup
}
void ctoast Window::SetVisible(bool visible) {
  visible ? XMapWindow(winstance, window)
          : XWithdrawWindow(winstance, window, screen);
}

int ctoast Window::Run() {
  XMapWindow(winstance, window);
  XEvent event;
  while (true) {
    XNextEvent(winstance, &event);

    if (event.xexpose.count == 0) {
      // Clear the window (optional)
      XClearWindow(winstance, window);

      for (Component *child_ : children) {
        Label *child = dynamic_cast<Label *>(child_);
        if (child != nullptr) {
          std::string text = child->GetText();

          const FcChar8 *str = (const FcChar8 *)text.c_str();
          XftColor color;
          XftColorAllocName(winstance, DefaultVisual(winstance, screen),
                            DefaultColormap(winstance, screen), "black",
                            &color);
          XftDrawStringUtf8(draw, &color, font, child->position.X,
                            child->position.Y + 20, (const FcChar8 *)str,
                            text.length());
        }
        /* code */
      }

      // Draw text on the window
    }

    // Exit if the user closes the window
    if (event.xany.type == ClientMessage) {
      break;
    }
  }
  XftDrawDestroy(draw);
  XftFontClose(winstance, font);
  XCloseDisplay(winstance);
  return 0;
}
void ctoast Window::Add(Component *comp) { children.push_back(comp); }
void ctoast Window::SetSize(Vector2 size) {}
void ctoast Window::SetTitle(std::string title) {}
void ctoast Window::SetColor(uint8_t r, uint8_t g, uint8_t b) {}
std::string ctoast Window::GetProperty(std::string property) {
  if (property == "Window") {
    return "true";
  }
  return "";
}
ctoast Window::~Window() {}
#endif