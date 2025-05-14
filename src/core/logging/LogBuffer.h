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
#include "TypeDefinitions.h"
#include <fstream>
#include <streambuf>
namespace reflect {
// Class to redirect std::cout to a file and console
class LogBuffer : public std::streambuf {
public:
  /**
   * @brief Constructs a LogBuffer object and initializes the file stream.
   *
   * Redirects `std::cout` to both the console and a log file.
   */
  REFLECT_API LogBuffer();

  REFLECT_API void setOutputFile(std::string file);

protected:
  /**
   * @brief Handles overflow when writing to the buffer.
   *
   * @param c The character to write.
   * @return The written character, or EOF on failure.
   */
  int overflow(int c) override;
  /**
   * @brief Synchronizes the buffer with the output streams.
   *
   * @return 0 on success, -1 on failure.
   */
  int sync() override;

private:
  std::streambuf *sb1;
  std::streambuf *sb2;
  std::ofstream fileStream;
};
} // namespace reflect