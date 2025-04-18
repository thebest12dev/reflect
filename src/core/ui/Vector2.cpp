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
#include "Vector2.h"
#include "TypeDefinitions.h"

namespace CinnamonToast {

Vector2::Vector2() = default;

Vector2::Vector2(int x, int y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2 &vec) const noexcept {
  return Vector2(this->x + vec.x, this->y + vec.y);
}

Vector2 Vector2::operator-(const Vector2 &vec) const noexcept {
  return Vector2(this->x - vec.x, this->y - vec.y);
}

Vector2 Vector2::operator*(const Vector2 &vec) const noexcept {
  return Vector2(this->x * vec.x, this->y * vec.y);
}

Vector2 Vector2::operator/(const Vector2 &vec) const noexcept {
  return Vector2(this->x / vec.x, this->y / vec.y);
}

Vector2 &Vector2::operator+=(const Vector2 &vec) noexcept {
  this->x += vec.x;
  this->y += vec.y;
  return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &vec) noexcept {
  this->x -= vec.x;
  this->y -= vec.y;
  return *this;
}

Vector2 &Vector2::operator*=(const Vector2 &vec) noexcept {
  this->x *= vec.x;
  this->y *= vec.y;
  return *this;
}

Vector2 &Vector2::operator/=(const Vector2 &vec) noexcept {
  this->x /= vec.x;
  this->y /= vec.y;
  return *this;
}

Vector2 Vector2::operator<<(int shift) const noexcept {
  return Vector2(this->x << shift, this->y << shift);
}

Vector2 Vector2::operator>>(int shift) const noexcept {
  return Vector2(this->x >> shift, this->y >> shift);
}

Vector2 Vector2::operator&(const Vector2 &vec) const noexcept {
  return Vector2(this->x & vec.x, this->y & vec.y);
}

Vector2 Vector2::operator|(const Vector2 &vec) const noexcept {
  return Vector2(this->x | vec.x, this->y | vec.y);
}
Vector2::operator std::vector<int>() const noexcept {
  return std::vector<int>{x, y};
}
} // namespace CinnamonToast