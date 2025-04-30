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
#include "LogBuffer.h"
#include <windows.h>

namespace CinnamonToast {

/**
 * @brief Constructs a LogBuffer object and initializes the file stream.
 *
 * Redirects `std::cout` to both the console and a log file.
 */
LogBuffer::LogBuffer() : fileStream("logs/current.log") {
  sb1 = std::cout.rdbuf();
  sb2 = fileStream.rdbuf();
}

/**
 * @brief Handles overflow when writing to the buffer.
 *
 * @param c The character to write.
 * @return The written character, or EOF on failure.
 */
int LogBuffer::overflow(int c) {
  __try {
    if (sb1 == nullptr || sb2 == nullptr)
      return EOF;
    if (c == EOF)
      return !EOF;
    if (sb1->sputc(c) == EOF)
      return EOF;
    if (sb2->sputc(c) == EOF)
      return EOF;
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    // Handle exception

    return EOF;
  }

  return c;
}

/**
 * @brief Synchronizes the buffer with the output streams.
 *
 * @return 0 on success, -1 on failure.
 */
int LogBuffer::sync() {
  __try {
    if (sb1 == nullptr || sb2 == nullptr)
      return -1;
    return (sb1->pubsync() == 0 && sb2->pubsync() == 0) ? 0 : -1;
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    // Handle exception
    return -1;
  }
}
void LogBuffer::setOutputFile(std::string file) {
  fileStream = std::ofstream(file);
  sb2 = fileStream.rdbuf();
}
} // namespace CinnamonToast