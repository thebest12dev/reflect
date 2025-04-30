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
// Include helper header
#include "ConsoleHelper.h"

#include "../core/logging/LogInstance.h"
#include "TypeDefinitions.h"
#include <iostream>
#include <string>

/**
 * @brief A shorthand macro for printing to the console. Handles function names
 * automatically.
 *
 * @param obj The object to print.
 */
#define ctoastPrint(obj) CinnamonToast::Console::print(obj);

/**
 * @brief A shorthand macro for printing with newline to the console. Handles
 * function names automatically.
 *
 * @param obj The object to print.
 */
#define ctoastPrintln(obj) CinnamonToast::Console::println(obj);

/**
 * @brief A shorthand macro for logging errors to the console. Handles function
 * names automatically.
 *
 * @param obj The error message or object to log.
 */
#define ctoastError(obj) CinnamonToast::Console::error(obj, __func__);

/**
 * @brief A shorthand macro for logging informational messages to the console.
 * Handles function names automatically.
 *
 * @param obj The informational message or object to log.
 */
#define ctoastInfo(obj) CinnamonToast::Console::info(obj, __func__);

/**
 * @brief A shorthand macro for logging warnings to the console. Handles
 * function names automatically.
 *
 * @param obj The warning message or object to log.
 */
#define ctoastWarn(obj) CinnamonToast::Console::warn(obj, __func__);

/**
 * @brief A shorthand macro for logging debug messages to the console. Handles
 * function names automatically.
 *
 * @param obj The debug message or object to log.
 */
#define ctoastDebug(obj) CinnamonToast::Console::debug(obj, __func__);

/**
 * @brief A shorthand macro for enabling or disabling debug logging.
 */
#define ctoastDebugEnabled CinnamonToast::Console::setDebugEnabled

namespace CinnamonToast {

/**
 * @brief A namespace for console logging utilities.
 */
namespace Console {

/**
 * @brief Prints an object to the console.
 *
 * @tparam T The type of the object to print.
 * @param obj The object to print.
 * @exception std::exception If an error occurs during printing.
 */
template <typename T> CTOAST_API inline void print(const T &obj) noexcept {
  try {
    std::cout << obj;
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "]: " << e.what() << std::endl;
  }
};

/**
 * @brief Prints an object to the console followed by a newline.
 *
 * @tparam T The type of the object to print.
 * @param obj The object to print.
 * @exception std::exception If an error occurs during printing, however will be
 * caught.
 */
template <typename T> CTOAST_API inline void println(const T &obj) noexcept {
  try {
    std::cout << obj << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "]: " << e.what() << std::endl;
  }
};

/**
 * @brief Logs an error message to the console.
 *
 * @tparam T The type of the error message or object.
 * @param obj The error message or object to log.
 * @param ctx The context or function name where the error occurred.
 * @exception std::exception If an error occurs during logging, however will be
 * caught.
 */
template <typename T>
CTOAST_API inline void error(const T &obj,
                             std::string ctx = "default") noexcept {
  try {
    std::cerr << "[ERROR] " << "[" << ctx << "]: " << obj << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
  }
};

/**
 * @brief Logs an informational message to the console.
 *
 * @tparam T The type of the informational message or object.
 * @param obj The informational message or object to log.
 * @param ctx The context or function name where the message is logged.
 * @exception std::exception If an error occurs during logging, however will be
 * caught.
 */
template <typename T>
CTOAST_API inline void info(const T &obj,
                            std::string ctx = "default") noexcept {
  try {
    std::cout << "[INFO] " << "[" << ctx << "]: " << obj << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
  }
};

/**
 * @brief Logs a warning message to the console.
 *
 * @tparam T The type of the warning message or object.
 * @param obj The warning message or object to log.
 * @param ctx The context or function name where the warning is logged.
 * @exception std::exception If an error occurs during logging, however will be
 * caught.
 */
template <typename T>
CTOAST_API inline void warn(const T &obj,
                            std::string ctx = "default") noexcept {
  try {
    std::cout << "[WARN] " << "[" << ctx << "]: " << obj << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
  }
};

/**
 * @brief Logs a debug message to the console if debug logging is enabled.
 *
 * @tparam T The type of the debug message or object.
 * @param obj The debug message or object to log.
 * @param ctx The context or function name where the debug message is logged.
 * @exception std::exception If an error occurs during logging, however will be
 * caught.
 */
template <typename T>
CTOAST_API inline void debug(const T &obj,
                             std::string ctx = "default") noexcept {
  try {
    if (getDebugEnabled())
      std::cout << "[DEBUG] " << "[" << ctx << "]: " << obj << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
  }
};

} // namespace Console
} // namespace CinnamonToast
