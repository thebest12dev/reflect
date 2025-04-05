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
#include "Component.h"
#include "Colors.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#include <string>
ctoast Component::Component()
    : position(Vector2(0, 0)), size(Vector2(0, 0)),
      color(Color3(255, 255, 255)) {}
// ctoast Component::~Component()  {
//
// }
ctoast Color3 ctoast Component::getColor() { return color; }
void ctoast Component::add(Component &comp) {
  // Do nothing
}
void ctoast Component::setVisible(int cmd) {
  // Do nothing
}
void ctoast Component::setVisible(bool cmd) {
  // Do nothing
}

void ctoast Component::setSize(Vector2 size) { this->size = size; }
void ctoast Component::setColor(Color3 color) { this->color = color; }
void ctoast Component::setColor(Color3Array color) {
  this->color.r = color[0];
  this->color.g = color[1];
  this->color.b = color[2];
}
void ctoast Component::setColor(uint8_t r, uint8_t g, uint8_t b) {
  this->color.r = r;
  this->color.g = g;
  this->color.b = b;
}
ctoast Vector2 ctoast Component::getPosition() { return position; };
ctoast Vector2 ctoast Component::getSize() { return size; };
#ifdef __linux__

// std::string ctoast Component::GetProperty(std::string property) {
//     return "";
// }
#endif