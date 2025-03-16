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
#pragma once
#define default_font "Segoe UI"

/**
 * External app name to be shown to the end user.
*/
#define app_name "Project CinnamonToast"

/**
 * Internal app version for compatibility checks.
 */
#define app_internal_version 0000001L

/**
 * External app version to be shown to the end user.
 */
#define app_version "1.0.0"

/**
 * Error code specification
 * 
 * These error codes should be used to indicate a specific error. Some errors
 * are platform-specific, and this is indicated by the second leftmost digit of the
 * error code. The valid values are:
 *      0x0 - Cross-platform
 *      0x1 - Windows
 *      0x2 - macOS
 *      0x3 - Linux
 * 
 * Examples are: 0x01000000 (an "error" code for a successful execution on Windows, though this is only
 * for demonstation purposes and SHOULD NOT be used in code), 0x03f00ba4 (Linux), etc.
 * 
 * 
 * There are some reserved return codes like 0x0000002 which is ALWAYS the "file not found" error. 0x00000000
 * is also another one of these codes. It's not necessarily an "error" code, though it is a return code for success.
 */

/**
 * Comment/uncomment this to toggle Win32 errors.
 */
#ifdef _WIN32
    #define win32_error_codes
#endif

// Internal error codes and other things
#define error_no_files_specified 0x000003f0L
#define error_generic_xml_error 0x000003f1L
#define error_xml_no_root 0x000003f2L
#define error_xml_no_window 0x000003f3L
#define error_file_not_found 0x0000002L
#define error_hex_color_malformed 0x000003f4L

// Win32 specific errors
#if defined(_WIN32) && defined(win32_error_codes)
    #define error_win_parent_hwnd_invalid 0x010003f5L
    // more errors go here
#endif