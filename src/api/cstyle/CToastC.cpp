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
#include "CToastC.h"
#include "../../core/CToastAPI.h"

// For convenience
using namespace CinnamonToast;

// C-style API functions

CTOAST_API CToastComponent CToast_getComponentById(CToastString id) {
  // Return a CToastComponent with the given id
  return {(uint8_t)ExternalAPI::getComponentById(id)};
};
CTOAST_API bool CToast_setColor(CToastComponent component, uint8_t r, uint8_t g,
                                uint8_t b) {
  // Set the color of the component
  ExternalAPI::setComponentColor(component.id, r, g, b);
  return true;
};
CTOAST_API CToastString CToast_getText(CToastComponent component) {
  // Get the text of the component
  return ExternalAPI::getComponentText(component.id);
}
CTOAST_API bool CToast_addComponent(CToastComponent parent,
                                    CToastComponent child) {
  // Add a child component to the parent component
  ExternalAPI::addComponent(parent.id, child.id);
  return true;
};
CTOAST_API bool CToast_setVisibleCommand(CToastComponent comp, uint8_t cmd) {
  // Set the visibility command of the component
  ExternalAPI::setComponentVisibleCommand(comp.id, cmd);
  return true;
};
CTOAST_API bool CToast_setVisible(CToastComponent comp, bool flag) {
  // Set the visibility of the component
  ExternalAPI::setComponentVisible(comp.id, flag);
  return true;
}
CTOAST_API bool CToast_setFont(CToastComponent comp, CToastString font) {
  // Set the font of the component
  ExternalAPI::setComponentFont(comp.id, font);
  return true;
}
CTOAST_API bool CToast_setFontSize(CToastComponent comp, uint8_t fontSize) {
  // Set the font size of the component
  ExternalAPI::setComponentFontSize(comp.id, fontSize);
  return true;
};