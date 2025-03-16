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
#include "TypeDefinitions.h"
#include "Utilities.h"

vector<string> ctoast Utilities::CstrArrToVector(const char* arr[]) {
    vector<string> vec;

    // Iterate through the array until the end (null pointer)
    for (int i = 0; arr[i] != nullptr; ++i) {
        vec.push_back(string(arr[i])); // Convert each const char* to string
    }

    return vec;
}
vector<string> ctoast Utilities::SplitString(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    for (char ch : str) {
        if (ch == delimiter) {
            if (!token.empty()) {
                result.push_back(token); // Add token to the result
            }
            token.clear(); // Reset the token
        } else {
            token += ch; // Append current character to the token
        }
    }
    if (!token.empty()) {
        result.push_back(token); // Add the last token, if any
    }
    return result;
}

#ifdef _WIN32
    HFONT ctoast Utilities::GetFont(string fontName, int size, int fontWeight, bool italic, bool underline, bool strikeout) {
            
        HFONT hFont = CreateFont(
            size,                     // Height of font
            0,                         // Width of font (0 for default)
            0,                         // Angle of escapement
            0,                         // Orientation angle
            fontWeight,                 // Font weight
            italic,                     // Italic
            underline,                     // Underline
            strikeout,                     // Strikeout
            ANSI_CHARSET,              // Character set
            OUT_DEFAULT_PRECIS,        // Output precision
            CLIP_DEFAULT_PRECIS,       // Clipping precision
            DEFAULT_QUALITY,           // Quality
            DEFAULT_PITCH | FF_SWISS,  // Pitch and family
            fontName.c_str()                   // Typeface name
        );

        return hFont;
    }
    string ctoast Utilities::GetLastErrorAsString() {
        DWORD errorMessageID = ::GetLastError(); // Get the last error code
        if (errorMessageID == 0) {
            return "No error"; // No error occurred
        }

        LPSTR messageBuffer = nullptr;

        // Format the error message
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorMessageID,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&messageBuffer,
            0,
            NULL
        );

        std::string message(messageBuffer, size);

        // Free the buffer allocated by FormatMessage
        LocalFree(messageBuffer);

        return message;
    }
#endif
#ifdef _WIN32
    #include <windows.h>
    #include <sstream>
    string ctoast Utilities::GetOSPlatformAndVersion() {
        OSVERSIONINFO osvi;
        stringstream ss;
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (GetVersionEx(&osvi)) {
            ss << "Windows " << osvi.dwMajorVersion << "." 
            << osvi.dwMinorVersion << " " 
            << osvi.szCSDVersion << std::endl;
        }
        return ss.str();
    }

#elif __linux__
    #include <sys/utsname.h>
    #include <sstream>

    string ctoast Utilities::GetOSPlatformAndVersion() {
        struct utsname buffer;
        stringstream ss;
        if (uname(&buffer) == 0) {
            ss << "Linux " << buffer.sysname << " "
            << buffer.release << " "
            << buffer.version << " "
            << buffer.machine << std::endl;
        }
        return ss.str();
    }
#endif
