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
typedef unsigned char byte;
#include "CToastAPI.h"
#include "Console.h"
#include "ui/Button.h"
#include "ui/Component.h"
#include "ui/Components.h"
#include "ui/Label.h"
#include "ui/TextComponent.h"
#include <Windows.h>
#include <iostream>
#include <vector>
using namespace CinnamonToast;
namespace {
std::vector<const char *> cull = {};
}

ComponentId ExternalAPI::getComponentById(const char *id) {

  cull.push_back(id);
  return cull.size();
};
const char *ExternalAPI::getComponentText(ComponentId ref) {
  // Ensure the ref is within bounds
  if (ref - 1 < 0 || ref - 1 >= cull.size()) {

    return nullptr; // Return null if invalid
  }

  // Attempt dynamic_cast to TextComponent*
  TextComponent *comp = fastCast<TextComponent>(
      Components::getComponentById(cull[ref - 1]), CTOAST_OBJECT_TEXTCOMPONENT);
  // Label *label = dynamic_cast<Label *>(comp);
  if (comp) {

    std::string *text = new std::string(comp->getText());
    const char *textc = text->c_str();

    return textc;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return nullptr; // Return null if the cast fails
  }
}
// const char* ExternalAPI::GetComponentText(ComponentId ref) {
//     // Ensure the ref is within bounds
//     if (ref - 1 < 0 || ref - 1 >= cull.size()) {
//
//         return nullptr;  // Return null if invalid
//     }
//
//     // Attempt dynamic_cast to TextComponent*
//     TextComponent* comp =
//     (TextComponent*)(Components::GetComponentById(cull[ref - 1])); Label*
//     label = dynamic_cast<Label*>(comp); if (label) {
//
//         string* text = new std::string(label->GetText());
//         const char* textc = text->c_str();
//
//         return textc;
//     }
//     else {
//         Console::warn("cast for component failed! (nullptr)",
//         "GetComponentText"); return nullptr;  // Return null if the cast
//         fails
//     }
// }
#include <functional>
bool ExternalAPI::setComponentColor(ComponentId component, uint8_t r, uint8_t g,
                                    uint8_t b) {
  // Ensure the component is within bounds
  if (component - 1 < 0 || component - 1 >= cull.size()) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to TextComponent*
  Component *comp =
      (Component *)(Components::getComponentById(cull[component - 1]));
  if (comp) {
    comp->setColor(r, g, b);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}

static ComponentId comp1_;
void (*callback_)(ComponentId comp);
void staticWrapper(Button &button) { callback_(comp1_); }
bool ExternalAPI::setOnClick(ComponentId comp1,
                             void (*callback)(ComponentId comp)) {
  if (comp1 - 1 < 0 || comp1 - 1 >= cull.size()) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to TextComponent*
  Button *comp =
      dynamic_cast<Button *>(Components::getComponentById(cull[comp1 - 1]));
  if (comp) {
    callback_ = callback;
    comp->onClick(staticWrapper);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}
bool ExternalAPI::addComponent(ComponentId parent, ComponentId child) {
  // Ensure the parent and child are within bounds
  if (parent - 1 < 0 || parent - 1 >= cull.size() || child - 1 < 0 ||
      child - 1 >= cull.size()) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to Component*
  Component *parentComp = Components::getComponentById(cull[parent - 1]);
  Component *childComp = Components::getComponentById(cull[child - 1]);
  if (parentComp && childComp) {
    parentComp->add(*childComp);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}

// ExternalAPI::SetComponentVisibleCommand
bool ExternalAPI::setComponentVisibleCommand(ComponentId comp, uint8_t cmd) {
  // Ensure the component is within bounds
  if (comp - 1 < 0 || comp - 1 >= cull.size()) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to Component*
  Component *compComp = Components::getComponentById(cull[comp - 1]);
  if (compComp) {
    compComp->setVisible(cmd);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}

// ExternalAPI::SetComponentVisible
bool ExternalAPI::setComponentVisible(ComponentId comp, bool flag) {
  // Ensure the component is within bounds
  if (comp - 1 < 0 || comp - 1 >= cull.size()) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to Component*
  Component *compComp = Components::getComponentById(cull[comp - 1]);
  if (compComp) {
    compComp->setVisible(flag);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}

// ExternalAPI::SetComponentFont
bool ExternalAPI::setComponentFont(ComponentId comp2, CToastString font) {
  // Ensure the font is not null
  if (font == nullptr) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to Component*
  TextComponent *comp = dynamic_cast<TextComponent *>(
      Components::getComponentById(cull[comp2 - 1]));
  if (comp) {
    comp->setFont(font);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}
// ExternalAPI::SetComponentFontSize
bool ExternalAPI::setComponentFontSize(ComponentId comp_, uint8_t fontSize) {
  // Ensure the font size is not null
  if (fontSize == 0) {
    return false; // Return false if invalid
  }
  // Attempt dynamic_cast to Component*
  TextComponent *comp = dynamic_cast<TextComponent *>(
      Components::getComponentById(cull[comp_ - 1]));
  if (comp) {
    comp->setFontSize(fontSize);
    return true;
  } else {
    ctoastWarn("cast for component failed! (nullptr)");
    return false; // Return false if the cast fails
  }
}
