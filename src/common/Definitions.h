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

/*
Default font for CinnamonToast to use.
*/
#define DEFAULT_FONT "Segoe UI"

/**
 * External app name to be shown to the end user.
 */
#define APP_NAME "Project CinnamonToast"

/**
 * Internal app version for compatibility checks.
 */
#define APP_INTERNAL_VERSION 0000006L

/**
 * External app version to be shown to the end user.
 */
#define APP_VERSION "1.4.0"

/**
 * Error code specification
 *
 * These error codes should be used to indicate a specific error. Some errors
 * are platform-specific, and this is indicated by the second leftmost digit of
 * the error code. The valid values are: 0x0 - Cross-platform 0x1 - Windows 0x2
 * - macOS 0x3 - Linux
 *
 * Examples are: 0x01000000 (an "error" code for a successful execution on
 * Windows, though this is only for demonstation purposes and SHOULD NOT be used
 * in code), 0x03badf00 (Linux), etc.
 *
 *
 * There are some reserved return codes like 0x0000002 which is ALWAYS the "file
 * not found" error. 0x00000000 is also another one of these codes. It's not
 * necessarily an "error" code, though it is a return code for success.
 */

/**
 * Comment/uncomment this to toggle Win32 errors.
 */
#ifdef _WIN32
// Define Win32 error codes
#define WIN32_ERROR_CODES
#endif

// Internal error codes and other things

// Error for if there isn't a file specified.
#define CTOAST_ERROR_NO_FILES_SPECIFIED 0x000003f0L

// Error for an XML error that does not originate from the program.
#define CTOAST_ERROR_GENERIC_XML_ERROR 0x000003f1L

// Error for if the XML file does not have a root element.
#define CTOAST_ERROR_XML_NO_ROOT 0x000003f2L

// Error for if the XML file does not have a window element inside the root
// element.
#define CTOAST_ERROR_XML_NO_WINDOW 0x000003f3L

// Error for if a file is not found.
#define CTOAST_ERROR_FILE_NOT_FOUND 0x0000002L

// Error for if a font is not found.
#define CTOAST_ERROR_FONT_NOT_FOUND 0x0000003L

// Error if a hex color (like #ffffff) is malformed.
#define CTOAST_ERROR_HEX_COLOR_MALFORMED 0x000003f4L

// A Linux-specific error that X11 isn't initialized.
#define CTOAST_ERROR_LINUX_X11_NOT_INITIALIZED 0x030003f5L

// Error if the shared library (.dll, .so, etc.) can't be loaded primarily due
// to filesystem errors or other reasons.
#define CTOAST_ERROR_CANNOT_LOAD_SHARED_LIBRARY 0x000003f9L

// Error if the shared library's `CToastMain` function does not exist or there
// is another problem.
#define CTOAST_ERROR_CANNOT_LOAD_LIBRARY_FUNCTION 0x000003faL

// Error if the XML document's app version is above the current app version.
#define CTOAST_ERROR_XML_NOT_COMPATIBLE 0x000003fbL

// Error if an unhandled exception (std::exception) occurs.
#define CTOAST_ERROR_UNHANDLED_EXCEPTION 0x000003fcL

// Win32 specific errors
#if defined(_WIN32) && defined(WIN32_ERROR_CODES)

// Error if the parent HWND (window handle) is invalid in Windows.
#define CTOAST_ERROR_WIN_PARENT_HWND_INVALID 0x010003f5L

// more errors go here
#endif