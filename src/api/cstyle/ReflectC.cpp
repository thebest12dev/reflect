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

// Includes
#include "ReflectC.h"
#include "../../core/ReflectAPI.h"

// why do you need this really?
using namespace reflect;

// literally whatever
REFLECT_API ReflectComponent Reflect_getComponentById(ReflectString id) {
  return {(uint8_t)external::getComponentById(id)}; // do not segfault!
};
REFLECT_API bool Reflect_setColor(ReflectComponent component, uint8_t r,
                                  uint8_t g, uint8_t b) {
  // color coding?
  external::setComponentColor(component.id, r, g, b);
  return true; // false false false
};

// string manipulation
REFLECT_API ReflectString Reflect_getText(ReflectComponent component) {
  // text
  return external::getComponentText(component.id);
}
REFLECT_API bool Reflect_addComponent(ReflectComponent parent,
                                      ReflectComponent child) {
  // drag and drop component inside of component
  external::addComponent(parent.id, child.id);
  return true;
};
REFLECT_API bool Reflect_setVisibleCommand(ReflectComponent comp, uint8_t cmd) {
  // make the button disappear
  external::setComponentVisibleCommand(comp.id, cmd);
  return true;
};
REFLECT_API bool Reflect_setVisible(ReflectComponent comp, bool flag) {
  // wizardo disappear?
  external::setComponentVisible(comp.id, flag);
  return true;
}
REFLECT_API bool Reflect_setFont(ReflectComponent comp, ReflectString font) {
  // font color coding???
  external::setComponentFont(comp.id, font);
  return true;
}
REFLECT_API bool Reflect_setFontSize(ReflectComponent comp, uint8_t fontSize) {
  // font color sizing???
  external::setComponentFontSize(comp.id, fontSize);
  return true;
};

REFLECT_API bool Reflect_run(ReflectComponent comp) {
  external::run(comp.id);
  return true;
};

REFLECT_API bool Reflect_invoke(const char *location) {
  external::invoke(location);
  return true;
};