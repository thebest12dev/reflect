/**
 * Copyright (c) 2025 thebest12lines
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

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

/**
 * @brief Default font for the Reflect framework.
 */
#define DEFAULT_FONT "Segoe UI"

/**
 * @brief External application name to be shown to the end user.
 */
#define APP_NAME "Reflect"

/**
 * @brief Internal application version for compatibility checks.
 */
#define APP_INTERNAL_VERSION 0x0000000CL

/**
 * @brief External application version to be shown to the end user.
 */
#define APP_VERSION "1.9.0"

#define REFLECT_VERSION_MAJOR 1
#define REFLECT_VERSION_MINOR 9
#define REFLECT_VERSION_PATCH 0

/**
 * @brief Error codes used in the Reflect framework.
 *
 * These error codes indicate specific issues, with platform-specific codes
 * indicated by the second leftmost digit:
 * - 0x0: Cross-platform
 * - 0x1: Windows
 * - 0x2: macOS
 * - 0x3: Linux
 */
#define REFLECT_ERROR_NO_FILES_SPECIFIED 0x000003f0L
#define REFLECT_ERROR_GENERIC_XML_ERROR 0x000003f1L
#define REFLECT_ERROR_XML_NO_ROOT 0x000003f2L
#define REFLECT_ERROR_XML_NO_WINDOW 0x000003f3L
#define REFLECT_ERROR_FILE_NOT_FOUND 0x0000002L
#define REFLECT_ERROR_FONT_NOT_FOUND 0x0000003L
#define REFLECT_ERROR_HEX_COLOR_MALFORMED 0x000003f4L
#define REFLECT_ERROR_LINUX_X11_NOT_INITIALIZED 0x030003f5L
#define REFLECT_ERROR_CANNOT_LOAD_SHARED_LIBRARY 0x000003f9L
#define REFLECT_ERROR_CANNOT_LOAD_LIBRARY_FUNCTION 0x000003faL
#define REFLECT_ERROR_XML_NOT_COMPATIBLE 0x000003fbL
#define REFLECT_ERROR_UNHANDLED_EXCEPTION 0x000003fcL

#if defined(_WIN32)
/**
 * @brief Windows-specific error codes.
 */
#define REFLECT_ERROR_WIN_PARENT_HWND_INVALID 0x010003f5L
#endif

#define REFLECT_OBJECT_OBJECT 0
#define REFLECT_OBJECT_COMPONENT 1
#define REFLECT_OBJECT_TEXTCOMPONENT 2
#define REFLECT_OBJECT_LABEL 3
#define REFLECT_OBJECT_BUTTON 4
#define REFLECT_OBJECT_WINDOW 5
#define REFLECT_OBJECT_MENUBAR 6
#define REFLECT_OBJECT_MENUITEM 7
#define REFLECT_OBJECT_TEXTFIELD 8
#define REFLECT_OBJECT_NOTIFICATION 9
#define REFLECT_OBJECT_PROGRESSBAR 10
#define REFLECT_OBJECT_IMAGE 11
#define REFLECT_OBJECT_CONTAINER 12
#define REFLECT_OBJECT_CANVAS 13