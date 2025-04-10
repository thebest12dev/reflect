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
#include <windows.h> // or any other conflicting headers
#endif
#include "Definitions.h"
#include "TypeDefinitions.h"
#include <string>
#include <vector>

namespace CinnamonToast {
namespace Utilities {
CTOAST_API std::vector<std::string> cstrArrToVector(const char *arr[]);
CTOAST_API std::vector<std::string> splitString(std::string str,
                                                char delimiter);

#ifdef _WIN32
CTOAST_API HFONT getFont(std::string fontName = DEFAULT_FONT, int size = 16,
                         int fontWeight = 400, bool italic = false,
                         bool underline = false, bool strikeout = false);
CTOAST_API std::string getLastErrorAsString();
#endif
CTOAST_API std::string getOSPlatformAndVersion();
} // namespace Utilities
} // namespace CinnamonToast
