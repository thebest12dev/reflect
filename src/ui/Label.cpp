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
#ifdef _WIN32
    #include "Label.h"
    #include "../TypeDefinitions.h"
    #include <windows.h>
    #include "../Utilities.h"
    #include "../Console.h"
    #include "../Definitions.h"
    #include <iostream>

    using namespace CinnamonToast::Console;
    using namespace CinnamonToast::Utilities;
    void ctoast Label::SetFont(string font) {
        fontStr = font;
        if (font == "default") {
            fontStr = DEFAULT_FONT;
        } 
    }
    void ctoast Label::SetFontSize(int size) {
        fontSize = size;
    }
    string ctoast Label::GetText() {
        return text;
    }
    void ctoast Label::Render(HWND& parentHWND, HWND& windowHWND) {
        if (!IsWindow(parentHWND)) {
            error("parent HWND is invalid!");
            std::exit(ERROR_WIN_PARENT_HWND_INVALID);
        }
        
        
        hwnd = CreateWindow(
            "STATIC",                   // Predefined class for a label
            text.c_str(),               // Label text
            WS_VISIBLE | WS_CHILD,      // Styles: visible and child window
            position.X, position.Y,     // Position (x, y)
            size.X, size.Y,             // Size (width, height)
            parentHWND,                 // Parent window handle
            NULL,                       // No menu or child ID
            winstance,                  // Instance handle
            NULL                        // Additional application data
        );
        if (size.X == 0 && size.Y == 0) {
            // Get the device context of the label
            HDC hdc = GetDC(hwnd);

            // Get the font used by the label
            HFONT hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
            if (hFont) {
                SelectObject(hdc, hFont); // Select the font into the device context
            }

            // Calculate the size of the text
            SIZE textSize;
            GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
            // // Get device context of the label
            // RECT parentRect;
            // GetClientRect(windowHWND, &parentRect); // Get the client area of the parent window

            // // Calculate the maximum available width for the label
            // int maxWidth = parentRect.right;

            // RECT labelRect;
            // GetWindowRect(hwnd, &labelRect);
            // MapWindowPoints(NULL, windowHWND, (LPPOINT)&labelRect, 2); // Convert to client coordinates
            // int labelX = labelRect.left;

            // int availableWidth = maxWidth - labelX; // Remaining width from the label's position

            // // Use DrawText to calculate the required dimensions
            // RECT textRect = { 0, 0, availableWidth, 0 }; // Limit width to the available space
            // DrawText(hdc, text.c_str(), -1, &textRect, DT_CALCRECT | DT_WORDBREAK);

            // // The textRect now contains the required width and height
            // int textWidth = textRect.right - textRect.left;
            // int textHeight = textRect.bottom - textRect.top;

            // // Retrieve font metrics for accurate line height
            // TEXTMETRIC textMetric;
            // GetTextMetrics(hdc, &textMetric);
            // int lineHeight = textMetric.tmHeight; // Height of a single line (ascent + descent)

            // // Calculate the number of lines (total height / line height)
            // int totalLines = textHeight / lineHeight;
            // Release the device context
            ReleaseDC(hwnd, hdc);

            SetWindowPos(
                hwnd,      // Handle to the label
                NULL,           // No z-order change
                position.X, position.Y,           // X and Y (ignored here)
                textSize.cx + (fontSize*2),    // New width
                fontSize,    // New height
                SWP_NOZORDER | SWP_NOMOVE // Don't change z-order or position
            );
        }
        if (!hwnd) {
            std::cout << GetLastErrorAsString();
            return;
        }

        // Create a custom font
        HFONT hFont = ctoast Utilities::GetFont(fontStr, fontSize);

        if (hFont) {
            // Set the font for the label
            SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
        } 
    }

    void ctoast Label::SetVisible(bool flag) {
    ShowWindow(this->hwnd, flag ? SW_SHOW : SW_HIDE);
    }
    ctoast Label::Label(string text, Vector2 pos) :  position(pos), size(Vector2(0,0)), text(text){
    }
#elif __linux__
    #include "Label.h"
    #include "../TypeDefinitions.h"
    #include "../Utilities.h"
    #include "../Console.h"
    #include "../Definitions.h"
    #include <iostream>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xft/Xft.h>

    //XftDrawStringUtf8(draw, &color, font, 50, 100, (const FcChar8 *)"Hello, X11 with Font!", 21);
    ctoast Label::Label(string text, Vector2 pos) :  position(pos), size(Vector2(0,0)), text(text){

    }
    void ctoast Label::SetVisible(bool flag) {
        this->visible = true;
    }
    void ctoast Label::SetFont(string font) {
        fontStr = font;
    }
    void ctoast Label::SetFontSize(int fontSize_) {
        fontSize = fontSize_;
    }
    ctoast Label::Label() : position(Vector2(0,0)), size(Vector2(0,0)), text(""){};
    string ctoast Label::GetText() {
        return text;
    }
    string ctoast Label::GetProperty(string property) {
        if (property == "Text") {
            return text;
        } else {
            return "";
        }
    }
#endif