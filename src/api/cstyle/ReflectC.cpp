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

// For convenience
using namespace reflect;

// C-style API functions

REFLECT_API ReflectComponent Reflect_getComponentById(ReflectString id) {
  // Return a ReflectComponent with the given id
  return {(uint8_t)ExternalAPI::getComponentById(id)};
};
REFLECT_API bool Reflect_setColor(ReflectComponent component, uint8_t r,
                                  uint8_t g, uint8_t b) {
  // Set the color of the component
  ExternalAPI::setComponentColor(component.id, r, g, b);
  return true;
};
REFLECT_API ReflectString Reflect_getText(ReflectComponent component) {
  // Get the text of the component
  return ExternalAPI::getComponentText(component.id);
}
REFLECT_API bool Reflect_addComponent(ReflectComponent parent,
                                      ReflectComponent child) {
  // Add a child component to the parent component
  ExternalAPI::addComponent(parent.id, child.id);
  return true;
};
REFLECT_API bool Reflect_setVisibleCommand(ReflectComponent comp, uint8_t cmd) {
  // Set the visibility command of the component
  ExternalAPI::setComponentVisibleCommand(comp.id, cmd);
  return true;
};
REFLECT_API bool Reflect_setVisible(ReflectComponent comp, bool flag) {
  // Set the visibility of the component
  ExternalAPI::setComponentVisible(comp.id, flag);
  return true;
}
REFLECT_API bool Reflect_setFont(ReflectComponent comp, ReflectString font) {
  // Set the font of the component
  ExternalAPI::setComponentFont(comp.id, font);
  return true;
}
REFLECT_API bool Reflect_setFontSize(ReflectComponent comp, uint8_t fontSize) {
  // Set the font size of the component
  ExternalAPI::setComponentFontSize(comp.id, fontSize);
  return true;
};