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
#include "TextComponent.h"
#ifdef _WIN32
    #include <cstdint>
    #include <windows.h>
    #include "Component.h"

    #include "Vector2.h"
    #include "TypeDefinitions.h"
    #include <string>
    namespace CinnamonToast {
        class Button : public TextComponent {
            protected:
                HINSTANCE winstance;
                HWND hwnd;
                
                HINSTANCE parentInstance;
                // HWND parentHWND;

                Vector2 position;
                Vector2 size;
                
            private:
                string text;
                string fontStr;
                int fontSize;
            public:
                friend class Component;
                CTOAST_API void Render(HWND& parentHWND, HWND& windowHWND);
                CTOAST_API void SetVisible(bool flag);
                CTOAST_API void Add(Component comp);
                CTOAST_API Button(string contents, Vector2 pos);
                CTOAST_API void SetVisible(int cmd);
                CTOAST_API void SetFontSize(int size);
                CTOAST_API void SetFont(string font);
                CTOAST_API void SetText(string text);
                CTOAST_API string GetText();
                CTOAST_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
        };
    } 
#elif __linux__
    #include <cstdint>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>

     #include <string>
    #include "Vector2.h"
    #include "../TypeDefinitions.h"
    #include "Component.h"
    namespace CinnamonToast {
        class Button : public Component {
            protected:
                Display* winstance;
                Window window;
                
                Display* parentInstance;
                // Window parentWindow;

                Vector2 position;
                Vector2 size;
                
            private:
                string text;
                string fontStr;
                int fontSize;
            public:
                friend class Component;
                CTOAST_API void Render(Window& parentWindow, Window& windowWindow);
                CTOAST_API void SetVisible(bool flag);
                CTOAST_API void Add(Component comp);
                CTOAST_API Button(string contents, Vector2 pos);
                CTOAST_API void SetVisible(int cmd);
                CTOAST_API void SetFontSize(int size);
                CTOAST_API void SetFont(string font);
                CTOAST_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
        };
    } 

#endif


