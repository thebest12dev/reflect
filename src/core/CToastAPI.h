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
typedef unsigned char byte;
#include "TypeDefinitions.h"
#include "ui/Colors.h"
#include <Windows.h>
#include <string>
namespace CinnamonToast {
typedef unsigned long long ComponentId;
typedef const char *CToastString;
struct CToastAPI {
  ComponentId (*getComponentById)(const char *id);
  bool (*setColor)(ComponentId component, uint8_t r, uint8_t g, uint8_t b);
  CToastString (*getText)(ComponentId component);
  bool (*addComponent)(ComponentId parent, ComponentId child);
  bool (*setVisibleCommand)(ComponentId comp, uint8_t cmd);
  bool (*setVisible)(ComponentId comp, bool flag);
  bool (*setFont)(ComponentId comp, CToastString font);
  bool (*setFontSize)(ComponentId comp, uint8_t fontSize);
};
namespace ExternalAPI {
CTOAST_API ComponentId getComponentById(const char *id);
CTOAST_API const char *getComponentText(ComponentId ref);
CTOAST_API bool setComponentColor(ComponentId component, uint8_t r, uint8_t g,
                                  uint8_t b);
CTOAST_API bool addComponent(ComponentId parent, ComponentId child);
CTOAST_API bool setComponentVisibleCommand(ComponentId comp, uint8_t cmd);
CTOAST_API bool setComponentVisible(ComponentId comp, bool flag);
CTOAST_API bool setComponentFont(ComponentId comp, CToastString font);
CTOAST_API bool setComponentFontSize(ComponentId comp, uint8_t fontSize);
} // namespace ExternalAPI
typedef void (*SharedLibraryMain)(CToastAPI *api);
} // namespace CinnamonToast