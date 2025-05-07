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

#include "ConsoleHelper.h"

#include "../core/logging/LogInstance.h"
namespace {
// Indicates whether debug logging is enabled.
bool debugEnabled = false;
} // namespace

namespace cinnamontoast {

/**
 * @brief Enables or disables debug logging.
 *
 * @param enabled A boolean indicating whether debug logging should be enabled.
 */
void console::setDebugEnabled(bool enabled) { debugEnabled = enabled; }

/**
 * @brief Retrieves the current debug logging state.
 *
 * @return True if debug logging is enabled, false otherwise.
 */
bool console::getDebugEnabled() { return debugEnabled; }
inline void console::setLogInstance(LogInstance inst) {
  std::cout.rdbuf(&(std::streambuf &)inst);
  std::cerr.rdbuf(&(std::streambuf &)inst);
};
} // namespace cinnamontoast