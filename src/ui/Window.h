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
#pragma once


#ifdef _WIN32
    #include <windows.h>
    #include "Component.h"
    #include "../TypeDefinitions.h"
    #include <cstdint>
    #include <string>
    #include "Vector2.h"
    namespace CinnamonToast {
        class Window : public Component {
            protected:
                 HINSTANCE winstance;
                 HWND hwnd;
            private:
                float bgColor[3];
            public:
                shared Window(HINSTANCE instance);
                shared void SetTitle(string title);
                // // void SetSize(Vector2 dim);
                shared static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
                
                shared void SetColor(uint8_t r, uint8_t g, uint8_t b);
                shared void SetColor(Color3 color);
                shared void SetColor(Color3Array color);
                shared void SetVisible(bool flag);
                shared void Add(Component &comp);
                shared void Add(Component& comp, string id);
                shared void SetSize(Vector2 size);
                shared void Render(HWND& parentHWND, HWND& windowHWND);
                shared void SetVisible(int cmd);
                shared int Run();
                
                shared ~Window();
                friend class Component;
                friend class Label;
        };
    }

#elif __linux__
    #include <cstdint>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xft/Xft.h>
    #include <string>
    #include "Vector2.h"
    #include "../TypeDefinitions.h"
    #include "Component.h"
    typedef Window XWindow;
    namespace CinnamonToast {
        class Window : public Component {
            protected:
                 Display* winstance;
                 XWindow window;
                 int screen;
                 XftFont* font;
                 XftDraw* draw;
            private:
                uint8_t bgColor[3];
            public:
                shared Window(Display* instance);
                shared void SetTitle(string title);
                // // void SetSize(Vector2 dim);
                shared void SetColor(uint8_t r, uint8_t g, uint8_t b);
                shared void SetVisible(bool flag);
                shared void Add(Component* comp);
                shared string GetProperty(string property);
                shared void SetSize(Vector2 size);
                shared void SetVisible(int cmd);
                shared int Run();
                shared ~Window();
                friend class Component;
                friend class Label;
        };
    }
#endif
