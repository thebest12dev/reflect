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

#include "TypeDefinitions.h"
#include <vector>

namespace reflect {

struct Vector2 {
public:
  REFLECT_API Vector2();
  REFLECT_API Vector2(int x, int y);

  // Arithmetic operators
  REFLECT_API Vector2 operator+(const Vector2 &vec) const noexcept;
  REFLECT_API Vector2 operator-(const Vector2 &vec) const noexcept;
  REFLECT_API Vector2 operator*(const Vector2 &vec) const noexcept;
  REFLECT_API Vector2 operator/(const Vector2 &vec) const noexcept;

  // Compound assignment
  REFLECT_API Vector2 &operator+=(const Vector2 &vec) noexcept;
  REFLECT_API Vector2 &operator-=(const Vector2 &vec) noexcept;
  REFLECT_API Vector2 &operator*=(const Vector2 &vec) noexcept;
  REFLECT_API Vector2 &operator/=(const Vector2 &vec) noexcept;

  // Bitwise shift
  REFLECT_API Vector2 operator<<(int shift) const noexcept;
  REFLECT_API Vector2 operator>>(int shift) const noexcept;

  // Bitwise logic
  REFLECT_API Vector2 operator&(const Vector2 &vec) const noexcept;
  REFLECT_API Vector2 operator|(const Vector2 &vec) const noexcept;
  REFLECT_API explicit operator std::vector<int>() const noexcept;
  int x, y;
};

} // namespace reflect
