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
#include <iostream>
#include <map>
/**
 * @brief Alias for an unsigned byte.
 */
typedef unsigned char byte;

#include "Definitions.h"

// Check if REFLECT_API is already defined
#if !defined(REFLECT_API) && !defined(REFLECT_BUILDING) &&                     \
    !defined(REFLECT_STATIC_LIBRARY)
#define REFLECT_API __declspec(dllimport)
#define REFLECT_IMPORT __declspec(dllimport)
#ifdef __linux__
#undef REFLECT_IMPORT
#define REFLECT_IMPORT extern
#endif
#endif

#ifdef REFLECT_BUNDLED
#undef REFLECT_IMPORT
#define REFLECT_IMPORT
#endif

#ifdef __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
/**
 * @brief Alias for an X11 window identifier.
 */
typedef XID XWindow;
#endif

#ifdef REFLECT_STATIC_LIBRARY
#undef REFLECT_API
#define REFLECT_API
#undef REFLECT_IMPORT
#define REFLECT_IMPORT

#endif

#define LIBC_NAMESPACE reflect

/**
 * @brief Namespace for the Reflect framework.
 */
namespace reflect {
#ifdef __linux__
using WindowHandle = XWindow;
using ApplicationHandle = Display *;
#elif _WIN32
using WindowHandle = HWND;
using ApplicationHandle = ApplicationHandle;
#endif
} // namespace reflect

// if defined
#undef REFLECT_SHARED_LIBRARY
#define REFLECT_SHARED_LIBRARY
#if defined(REFLECT_SHARED_LIBRARY) && !defined(REFLECT_EXTERNAL)
#ifdef _WIN32
#undef REFLECT_API
#define REFLECT_API __declspec(dllexport)
#elif __linux__
#undef REFLECT_API
#define REFLECT_API __attribute__((visibility("default")))
#endif
#endif


