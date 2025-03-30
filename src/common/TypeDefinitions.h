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
typedef unsigned char byte;
#include "Definitions.h"

/*
The macro to specify whether or not CinnamonToast should export or import
functions.
*/
#define CTOAST_API
#undef CTOAST_API

// Check if CTOAST_API is already defined
#if !defined(CTOAST_API) && !defined(CTOAST_BUILDING)
#define CTOAST_API __declspec(dllimport)
#endif

/*
Alias for CinnamonToast
*/
#define ctoast CinnamonToast::
#define LIBC_NAMESPACE CinnamonToast

/*
The namespace which holds everything related to CinnamonToast as well as
additional types and objects.
*/
namespace CinnamonToast {};

// if defined
#undef CTOAST_SHARED_LIBRARY
#define CTOAST_SHARED_LIBRARY

#ifdef CTOAST_SHARED_LIBRARY
#ifdef _WIN32
#undef CTOAST_API // Undefine CTOAST_API if it was previously defined
#define CTOAST_API __declspec(dllexport)
#elif __linux__
#undef CTOAST_API // Undefine CTOAST_API if it was previously defined
#define CTOAST_API __attribute__((visibility("default")))
#endif
#endif