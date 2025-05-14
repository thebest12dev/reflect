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

namespace reflect {
/**
 * @brief A namespace primarily for utility functions.
 */
namespace utilities {
/**
 * @brief Converts a C-style string array to a vector of strings.
 *
 * @param arr The C-style string array to convert.
 * @return A vector of strings.
 */
REFLECT_API std::vector<std::string> cstrArrToVector(const char *arr[]);

/**
 * @brief Splits a string into a vector of strings based on a delimiter.
 *
 * @param str The string to split.
 * @param delimiter The character to split the string by.
 * @return A vector of strings.
 */
REFLECT_API std::vector<std::string> splitString(std::string str,
                                                 char delimiter);

#ifdef _WIN32
/**
 * @brief Creates a Win32 font with the specified properties.
 *
 * @param fontName The name of the font. Defaults to Segoe UI.
 * @param size The size of the font. Defaults to 16.
 * @param fontWeight The weight of the font. Defaults to 400 (normal).
 * @param italic Whether the font is italic. Defaults to false.
 * @param underline Whether the font is underlined. Defaults to false.
 * @param strikeout Whether the font is struck out. Defaults to false.
 * @return A handle to the created font.
 */
REFLECT_API HFONT getFont(std::string fontName = DEFAULT_FONT, int size = 16,
                          int fontWeight = 400, bool italic = false,
                          bool underline = false, bool strikeout = false);
/**
 * @brief Retrieves the last error message as a string.
 */
REFLECT_API std::string getLastErrorAsString();
#endif
/**
 * @brief Retrieves the current operating system platform and version.
 */
REFLECT_API std::string getOSPlatformAndVersion();
} // namespace utilities
} // namespace reflect
