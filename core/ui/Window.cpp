/**
* Copyright (c) 2025 thebest12lines
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Component.h"
#include "TextComponent.h"
#include <string>
#include "Vector2.h"
#include <iostream>
#include "Window.h"
#include "Label.h"
#include "MenuBar.h"
#include "Components.h"
#include "Console.h"
#include "TypeDefinitions.h"
#ifdef _WIN32
    #include <windows.h>
    #include <d2d1.h>


    using namespace CinnamonToast::Console;
    // Direct2D-specific members
    ID2D1Factory* pFactory = nullptr;
    ID2D1HwndRenderTarget* pRenderTarget = nullptr;
    // Due to floating point operations, may not produce exact color
    void ctoast Window::SetColor(uint8_t r, uint8_t g, uint8_t b) {
        this->bgColor[0] = r / 255.0f;
        this->bgColor[1] = g / 255.0f;
        this->bgColor[2] = b / 255.0f;
    }
    // Due to floating point operations, may not produce exact color
    void ctoast Window::SetColor(Color3 color) {
        this->bgColor[0] = color.r / 255.0f;
        this->bgColor[1] = color.g / 255.0f;
        this->bgColor[2] = color.b / 255.0f;
    }
    // Due to floating point operations, may not produce exact color
    void ctoast Window::SetColor(Color3Array color) {
        this->bgColor[0] = color[0] / 255.0f;
        this->bgColor[1] = color[1] / 255.0f;
        this->bgColor[2] = color[2] / 255.0f;
    }

    void InitializeDirect2D(HWND hwnd) {
        debug("initializing direct2d...","InitializeDirect2D");
        // Create the Direct2D factory
        if (!pFactory) {
            debug("creating d2d1 factory...","InitializeDirect2D");
            D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
        }
    
        // Get window dimensions
        RECT rc;
        GetClientRect(hwnd, &rc);
    
        // Create the render target
        debug("creating renderer target...","InitializeDirect2d");
        if (!pRenderTarget) {
            pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(
                    hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
                &pRenderTarget);
        }
    }
  
    LRESULT CALLBACK ctoast Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        CinnamonToast::Window* pThis = nullptr;

        if (uMsg == WM_CREATE) {
            debug("message type: WM_CREATE", "WindowProc");
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<ctoast Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            // Initialize Direct2D
            debug("calling InitializeDirect2D...", "WindowProc");
            InitializeDirect2D(hwnd);
        }
        else {
            pThis = reinterpret_cast<ctoast Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis) {
            // Delegate the handling of messages to the instance
            switch (uMsg) {
            case WM_DESTROY:
                debug("message type: WM_DESTROY", "WindowProc");
                PostQuitMessage(0);
                info("Exiting...", "WindowProc");
                return 0;
            case WM_SIZE:
                if (pRenderTarget) {
                    RECT rc;
                    GetClientRect(hwnd, &rc);
                    pRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
                }
                InvalidateRect(hwnd, NULL, TRUE);  // Mark the entire window as needing a repaint
                break;
            case WM_MOVE:
                InvalidateRect(hwnd, NULL, TRUE);  // Mark the entire window as needing a repaint
                break;
            case WM_PAINT: {
                if (!pRenderTarget) {
                    InitializeDirect2D(hwnd);
                }

                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // Create a clipping region for the child controls
                HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
                int regionType = GetWindowRgn(hwnd, hrgn); // Get the window's region (valid or error)
                if (regionType != ERROR) {
                    RECT controlRect;
                    HWND hChild = GetWindow(hwnd, GW_CHILD); // Get the first child window

                    while (hChild != NULL) {
                        GetWindowRect(hChild, &controlRect);
                        MapWindowPoints(NULL, hwnd, (LPPOINT)&controlRect, 2); // Convert to client coordinates

                        // Exclude the child control region from the painting area
                        ExcludeClipRect(hdc, controlRect.left, controlRect.top, controlRect.right, controlRect.bottom);

                        hChild = GetNextWindow(hChild, GW_HWNDNEXT); // Get the next child window
                    }
                }

                // Start Direct2D rendering
                pRenderTarget->BeginDraw();

                // Set background color (this will not cover child control areas due to ExcludeClipRect)
                pRenderTarget->Clear(D2D1::ColorF(pThis->bgColor[0], pThis->bgColor[1], pThis->bgColor[2]));

                // End drawing
                HRESULT hr = pRenderTarget->EndDraw();
                if (hr == D2DERR_RECREATE_TARGET) {
                    // Handle device loss
                    pRenderTarget->Release();
                    pRenderTarget = nullptr;
                    InitializeDirect2D(hwnd);
                }

                // Clean up
                DeleteObject(hrgn);
                EndPaint(hwnd, &ps);
                break;
            }
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    void ctoast Window::Add(ctoast Component& comp) {
        debug("added new component","WindowProc");
        comp.winstance = this->parentInstance;
        comp.Render(this->hwnd, this->hwnd);
    }
    void ctoast Window::Add(ctoast Component& comp, string id) {
        debug("added new component", "WindowProc");
        if (Components::gchildren[id] == nullptr) {
            Components::gchildren[id] = &comp;

            comp.winstance = this->parentInstance;
            comp.Render(this->hwnd, this->hwnd);  
        }
    }
    void ctoast Window::SetSize(Vector2 size) {
        debug("resized window","WindowProc");
        SetWindowPos(
            hwnd,                // Handle to the window
            NULL,                // Z-order (NULL if not changing the order)
            100,                 // New X position
            100,                 // New Y position
            size.X,                 // New width
            size.Y,                 // New height
            SWP_NOZORDER | SWP_NOACTIVATE // Flags
        );
        
    }
    ctoast Window::Window(HINSTANCE instance) : winstance(instance) {
        debug("initializing win32 parameters...","Window");
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc; // Window procedure
        wc.hInstance = instance;
        wc.lpszClassName = "WindowClass";
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        RegisterClass(&wc);

        // Create window
        debug("calling CreateWindowEx...","Window");
        hwnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            "Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 100, 100,
            nullptr, nullptr, instance, this);

        if (hwnd == nullptr) {
            debug("whoops, hwnd is nullptr","Window");
            MessageBox(nullptr, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
            exit(1);
        }
    }

    ctoast Window::~Window() {
        debug("releasing memory...","~Window");
        if (pRenderTarget) {
            pRenderTarget->Release();
        }
        if (pFactory) {
            pFactory->Release();
        }
    }

    void ctoast Window::SetTitle(std::string title) {
        debug("window title set","SetTitle");
        SetWindowText(this->hwnd, title.c_str());
    }

    void ctoast Window::SetVisible(bool flag) {
        debug("window visible = "+flag ? "true" : "false","SetVisible");
        ShowWindow(hwnd, flag ? SW_SHOW : SW_HIDE);
    }

    void ctoast Window::SetVisible(int cmd) {
        ShowWindow(hwnd, cmd);
    }
    void ctoast Window::Render(HWND& parentHWND, HWND& windowHWND) {
        warn("Window::Render called, the method is intentionally empty because it is not a child component!","Render");
        // do nothing
    }

    int ctoast Window::Run() {
        info("Running window...","Run");
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
    }

#elif __linux__
    #include "Label.h"
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <string>
    #include <X11/Xft/Xft.h>
#include <string.h>
    ctoast Window::Window(Display* instance) {
        // Initialize the X11 winstance
        winstance = instance;
        if (!winstance) {
            std::cerr << "Cannot open winstance." << std::endl;
            exit(error_linux_x11_not_initialized);
        }

        // Create a window
        screen = DefaultScreen(winstance);
        window = XCreateSimpleWindow(winstance, RootWindow(winstance, screen), 100, 100, 400, 200, 1, BlackPixel(winstance, screen), WhitePixel(winstance, screen));

        // Set window title
        XStoreName(winstance, window, "Window");

        // Select input events
        XSelectInput(winstance, window, ExposureMask | KeyPressMask);

        // Map (show) the window
        // XMapWindow(winstance, window);

        // Load the font using Xft
        font = XftFontOpenName(winstance, screen, "Ubuntu-20");
        if (!font) {
            std::cerr << "Font not found!" << std::endl;
            exit(error_font_not_found);
        }

        // Create XftDraw for drawing text
        draw = XftDrawCreate(winstance, window, DefaultVisual(winstance, screen), DefaultColormap(winstance, screen));

        // Set color for text (black)
        

        // Event loop to handle drawing text
        

        // Cleanup
       
    }
    void ctoast Window::SetVisible(bool visible) {
        visible ? XMapWindow(winstance, window) : XWithdrawWindow(winstance, window, screen);
    }
    
    int ctoast Window::Run() {
        XMapWindow(winstance, window);
        XEvent event;
        while (true) {
            XNextEvent(winstance, &event);

            if (event.xexpose.count == 0) {
                // Clear the window (optional)
                XClearWindow(winstance, window);
                
                for (Component* child_: children) {
                    Label* child = dynamic_cast<Label*>(child_);
                    if (child != nullptr) {
                        string text = child->GetText();

                        const FcChar8* str = (const FcChar8*) text.c_str();
                        XftColor color;
                        XftColorAllocName(winstance, DefaultVisual(winstance, screen), DefaultColormap(winstance, screen), "black", &color);
                        XftDrawStringUtf8(draw, &color, font, child->position.X , child->position.Y+20 , (const FcChar8*) str, text.length());
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
    void ctoast Window::Add(Component* comp) {
        children.push_back(comp);
    }
    void ctoast Window::SetSize(Vector2 size) {}
    void ctoast Window::SetTitle(string title) {}
    void ctoast Window::SetColor(uint8_t r, uint8_t g, uint8_t b ) {}
    string ctoast Window::GetProperty(string property) {
        if (property == "Window") {
            return "true";
        }
        return "";
    }
    ctoast Window::~Window() {
        
    }
#endif