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
#include "Utilities.h"
#include "TypeDefinitions.h"
#include <sstream>
#include <string>
#ifdef _WIN32
#include <VersionHelpers.h>
#include <windows.h>
#endif
namespace reflect {

std::vector<std::string> utilities::cstrArrToVector(const char *arr[]) {
  std::vector<std::string> vec;

  // Iterate through the array until the end (null pointer)
  for (int i = 0; arr[i] != nullptr; ++i) {
    vec.push_back(std::string(arr[i])); // Convert each const char* to string
  }

  return vec;
}
std::vector<std::string> utilities::splitString(std::string str,
                                                char delimiter) {
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
HFONT utilities::getFont(std::string fontName, int size, int fontWeight,
                         bool italic, bool underline, bool strikeout) {

  HFONT hFont = CreateFont(size,                // Height of font
                           0,                   // Width of font (0 for default)
                           0,                   // Angle of escapement
                           0,                   // Orientation angle
                           fontWeight,          // Font weight
                           italic,              // Italic
                           underline,           // Underline
                           strikeout,           // Strikeout
                           ANSI_CHARSET,        // Character set
                           OUT_DEFAULT_PRECIS,  // Output precision
                           CLIP_DEFAULT_PRECIS, // Clipping precision
                           DEFAULT_QUALITY,     // Quality
                           DEFAULT_PITCH | FF_SWISS, // Pitch and family
                           fontName.c_str()          // Typeface name
  );

  return hFont;
}
std::string utilities::getLastErrorAsString() {
  DWORD reflectErrorMessageID =
      ::GetLastError(); // Get the last reflectError code
  if (reflectErrorMessageID == 0) {
    return "No error"; // No reflectError occurred
  }

  LPSTR messageBuffer = nullptr;

  // Format the reflectError message
  size_t size = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, reflectErrorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&messageBuffer, 0, NULL);

  std::string message(messageBuffer, size);

  // Free the buffer allocated by FormatMessage
  LocalFree(messageBuffer);

  return message;
}
#endif
#ifdef _WIN32
#include <sstream>
#include <windows.h>
std::string utilities::getOSPlatformAndVersion() {
  std::stringstream ss;
  if (IsWindows10OrGreater()) {
    ss << "Windows 10";
  } else if (IsWindows8OrGreater()) {
    ss << "Windows 8";
  } else if (IsWindows7OrGreater()) {
    ss << "Windows 7";
  } else if (IsWindowsVistaOrGreater()) {
    ss << "Windows Vista";
  } else if (IsWindowsXPOrGreater()) {
    ss << "Windows XP";
  } else {
    ss << "Older than Windows XP";
  }
  return ss.str();
}

#elif __linux__
#include <sstream>
#include <sys/utsname.h>

std::string reflect::utilities::GetOSPlatformAndVersion() {
  struct utsname buffer;
  std::stringstream ss;
  if (uname(&buffer) == 0) {
    ss << "Linux " << buffer.sysname << " " << buffer.release << " "
       << buffer.version << " " << buffer.machine << std::endl;
  }
  return ss.str();
}
#endif

} // namespace reflect
