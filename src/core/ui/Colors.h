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
#include <stdexcept>
namespace {
float value0 = 0.0f;
}
namespace reflect {
struct Color3 {
  unsigned char r, g, b;
  Color3() : r(0), g(0), b(0) {}
  Color3(unsigned char r, unsigned char g, unsigned char b)
      : r(r), g(g), b(b) {}
};

struct Color3Float {
  union {
    struct {
      float r, g, b;
    };
    float data[3]; // allows array-style access
  };
  Color3Float() : r(0.0f), g(0.0f), b(0.0f){};
  Color3Float(float r, float g, float b) : r(r), g(g), b(b){};
  operator Color3() {
    return Color3(static_cast<unsigned char>(r * 255),
                  static_cast<unsigned char>(g * 255),
                  static_cast<unsigned char>(b * 255));
  }
  Color3Float &operator=(const Color3 color) {
    r = static_cast<float>(color.r) / 255;
    g = static_cast<float>(color.g) / 255;
    b = static_cast<float>(color.b) / 255;
    return *this;
  }
  // Array-style access (read/write)

  float &operator[](size_t index) {
    if (index >= 3) {
      return value0;
    }
    return data[index];
  }

  // Array-style access (read-only version)
  const float &operator[](size_t index) const {
    if (index >= 3)
      return 0;
    return data[index];
  }
};
typedef unsigned char Color3Array[3];
} // namespace reflect