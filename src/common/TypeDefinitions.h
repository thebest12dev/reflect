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

// Check if CTOAST_API is already defined
#if !defined(CTOAST_API) && !defined(CTOAST_BUILDING) &&                       \
    !defined(CTOAST_STATIC_LIBRARY)
#define CTOAST_API __declspec(dllimport)
#define CTOAST_IMPORT __declspec(dllimport)
#ifdef __linux__
#undef CTOAST_IMPORT
#define CTOAST_IMPORT extern
#endif
#endif

#ifdef CTOAST_BUNDLED
#undef CTOAST_IMPORT
#define CTOAST_IMPORT
#endif

#ifdef __linux__
#include <X11/X.h>
/**
 * @brief Alias for an X11 window identifier.
 */
typedef XID XWindow;
#endif

#ifdef CTOAST_STATIC_LIBRARY
#undef CTOAST_API
#define CTOAST_API
#undef CTOAST_IMPORT
#define CTOAST_IMPORT

#endif
/*
Alias for CinnamonToast
*/
#define ctoast cinnamontoast::
#define LIBC_namespace cinnamontoast

/**
 * @brief Namespace for the CinnamonToast framework.
 */
namespace cinnamontoast {}

// if defined
#undef CTOAST_SHARED_LIBRARY
#define CTOAST_SHARED_LIBRARY
#if defined(CTOAST_SHARED_LIBRARY) && !defined(CTOAST_EXTERNAL)
#ifdef _WIN32
#undef CTOAST_API
#define CTOAST_API __declspec(dllexport)
#elif __linux__
#undef CTOAST_API
#define CTOAST_API __attribute__((visibility("default")))
#endif
#endif
