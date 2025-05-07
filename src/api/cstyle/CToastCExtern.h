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
// pragma once if supported
#if defined(__cplusplus) && (defined(_MSVC_VER) || defined(__GNUC__) ||        \
                             defined(__clang__) || defined(__MINGW__))
#pragma once
#endif
// if not supported, use #ifndef
#ifndef CTOASTC_H
#define CTOASTC_H
#undef CTOAST_API

#if defined(CTOAST_BUILDING) && defined(_WIN32)
#undef CTOAST_API
#define CTOAST_API __declspec(dllexport)
#elif !defined(CTOAST_BUILDING) && defined(_WIN32)
#undef CTOAST_API
#define CTOAST_API __declspec(dllimport)
#endif
#if defined(CTOAST_BUILDING) && __linux__
#define CTOAST_API __attribute__((visibility("default")))
#else
#define CTOAST_API extern
#endif
#ifdef __cplusplus
// use c++ standard libraries
#include "../../src/core/ui/Components.h"
#include <cstdint>
#else
// use c standard libraries
#include <stdbool.h>
#include <stdint.h>
#endif //

#ifdef __cplusplus
// extern "C" to prevent name mangling
extern "C" {
#endif
// C-style API functions

/**
 * @brief A C-style component structure mapping to C++.
 *
 * Every component in C has an ID, which maps to the component in
 * `cinnamontoast::Components`.
 */
typedef struct {
  /**
   * @brief The ID of the component.
   *
   * This ID is used to identify the component in C++.
   * Note that for now, only 256 components are supported.
   */
  uint8_t id;

} CToastComponent;

/**
 * @brief A type alias for a string in C.
 */
typedef const char *CToastString;

/**
 * @brief Method to get a component by its ID.
 *
 * It uses the `ExternalAPI::getComponentById` method to get the component,
 * which goes to `Components::getComponentById`.
 *
 * @return A CToastComponent with the given ID.
 */
CTOAST_API CToastComponent CToast_getComponentById(CToastString id);

/**
 * @brief Method to set the color of a component.
 *
 * It uses the `ExternalAPI::setComponentColor` method to set the color of the
 * component, which goes to `Component::setColor`.
 *
 * @param component The component to set the color of.
 * @param r The red value of the color.
 * @param g The green value of the color.
 * @param b The blue value of the color.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_setColor(CToastComponent component, uint8_t r, uint8_t g,
                                uint8_t b);

/**
 * @brief Method to get the text of a component.
 *
 * It uses the `ExternalAPI::getComponentText` method to get the text of the
 * component, which goes to `TextComponent::getText`. Please note that this
 * only works for `TextComponent`.
 *
 * @param component The component to get the text of.
 * @return The text of the component.
 */
CTOAST_API CToastString CToast_getText(CToastComponent component);

/**
 * @brief Method to add a child component to a parent component.
 *
 * It uses the `ExternalAPI::addComponent` method to add the child component
 * to the parent component, which goes to `Component::add`.
 *
 * @param parent The parent component.
 * @param child The child component.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_addComponent(CToastComponent parent,
                                    CToastComponent child);
/**
 * @brief Method to set the visibility command of a component.
 *
 * It uses the `ExternalAPI::setComponentVisibleCommand` method to set the
 * visibility command of the component, which goes to
 * `Component::setVisible`.
 *
 * @param comp The component to set the visibility command of.
 * @param cmd The visibility command.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_setVisibleCommand(CToastComponent comp, uint8_t cmd);
/**
 * @brief Method to set the visibility of a component.
 *
 * It uses the `ExternalAPI::setVisible` method to set the
 * visibility of the component, which goes to `Component::setVisible`.
 *
 * @param comp The component to set the visibility of.
 * @param flag The visibility flag.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_setVisible(CToastComponent comp, bool flag);
/**
 * @brief Method to set the font of a component.
 *
 * It uses the `ExternalAPI::setFont` method to set the
 * font of the component, which goes to `TextComponent::setFont`.
 *
 * @param comp The component to set the font of.
 * @param font The font to set.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_setFont(CToastComponent comp, CToastString font);
/**
 * @brief Method to set the font size of a component.
 *
 * It uses the `ExternalAPI::setFontSize` method to set the
 * font size of the component, which goes to `TextComponent::setFontSize`.
 *
 * @param comp The component to set the font size of.
 * @param fontSize The font size to set.
 * @return true if successful, false otherwise.
 */
CTOAST_API bool CToast_setFontSize(CToastComponent comp, uint8_t fontSize);
#ifdef __cplusplus
// end extern "C"
}
#endif
#endif
