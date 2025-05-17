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
#include "Colors.h"
#include "Object.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif
#define COMPONENT_COMMON(classname)                                            \
  reflect::Vector2 classname::getSize() { return size; };                      \
  reflect::Vector2 classname::getPosition() { return position; };              \
  reflect::Color3 classname::getColor() { return color; };                     \
  void classname::setColor(Color3 color) { this->color = color; }              \
  void classname::setColor(Color3Array color) {                                \
    this->color.r = color[0];                                                  \
    this->color.g = color[1];                                                  \
    this->color.b = color[2];                                                  \
  }                                                                            \
  void classname::setColor(uint8_t r, uint8_t g, uint8_t b) {                  \
    this->color.r = r;                                                         \
    this->color.g = g;                                                         \
    this->color.b = b;                                                         \
  }

#define COMPONENT_DECL(classname)                                              \
  REFLECT_API void render(WindowHandle &parentHWND, WindowHandle &windowHWND); \
  REFLECT_API void setVisible(bool flag);                                      \
  REFLECT_API void add(Component &comp);                                       \
  REFLECT_API void setSize(Vector2 size);                                      \
  REFLECT_API void setVisible(int cmd);                                        \
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);                  \
  REFLECT_API void setColor(Color3 color);                                     \
  REFLECT_API void setColor(Color3Array color);                                \
  REFLECT_API virtual ~classname();                                            \
  REFLECT_API Vector2 getPosition();                                           \
  REFLECT_API Vector2 getSize();                                               \
  REFLECT_API bool getVisible();                                               \
  REFLECT_API Color3 getColor()

namespace reflect {
class Component : public Object {
protected:
  /// @brief The ApplicationHandle object associated
  /// with the program required for window creation.
  ApplicationHandle winstance;

  /// @brief The window handle (WindowHandle) associated
  /// with the object needed for Win32 APIs.
  WindowHandle hwnd;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 position;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 size;

  /// @brief The color of the
  /// component in RGB.
  Color3Float bgColor;

#ifdef __linux__
  // Passed to the X11 window loop
  bool visible = true;
#endif
public:
  /**
   * @brief Creation of the component as well as
   * fields and other properties.
   */
  REFLECT_API Component(/* args */);

  /**
   * @brief Method to render the component, primarily handled by the derived
   * class.
   *
   * It uses 2 arguments, the parent window handle and the window's window
   * handle. Using these arguments, the component can be attached to the parent
   * window and rendered as necessary. Note that this is intentionally left
   * blank in some use cases, but it is not recommended to leave it blank.
   *
   * @param parentHWND The parent window handle (obtained through
   * CreateWindowEx/CreateWindow).
   * @param windowHWND The window window handle (similar to the parent handle
   * but for the window).
   */
  REFLECT_API virtual void render(WindowHandle &parentHWND,
                                  WindowHandle &windowHWND) = 0;

  /**
   * @brief Sets the component visibility to either show or hide depending on
   * the flag.
   *
   * @param flag The flag to set visibility.
   */
  REFLECT_API void setVisible(bool flag);
  REFLECT_API void paint();
  /**
   * @brief Adds a component to the hierarchy, as well as calling the `Render`
   * function of the child component to properly render it.
   */
  REFLECT_API void add(Component &comp);

  /**
   * @brief Sets the size of the component.
   */
  REFLECT_API void setSize(Vector2 size);

  /**
   * @brief Similar to `SetVisible(bool)` but directly use Win32 commands. On
   * Linux, this is mapped to the appropriate X11 commands.
   */
  REFLECT_API void setVisible(int cmd);

  /**
   * @brief Sets the color of the component by its red, green and blue
   * components.
   */
  REFLECT_API void setColor(uint8_t r, uint8_t g, uint8_t b);

  /**
   * @brief Similar to SetColor(uint8_t, uint8_t, uint8_t) but with a Color3
   * struct.
   */
  REFLECT_API void setColor(Color3 color);

  /**
   * @brief Same as other methods but with an int[3] type.
   */
  REFLECT_API void setColor(Color3Array color);

  /**
   * @brief Virtual destructor needed for runtime polymorphism.
   */
  REFLECT_API virtual ~Component() = default; // To allow dynamic_cast to work

  /**
   * @brief Returns the component's position.
   * @returns The component's position.
   */
  REFLECT_API Vector2 getPosition();
  /**
   * @brief Returns the component's size.
   * @returns The component's size.
   */
  REFLECT_API Vector2 getSize();
  /**
   * @brief Returns the component's visibility.
   * @returns The component's visibility.
   */
  REFLECT_API bool getVisible();
  /**
   * @brief Returns the component's color.
   * @returns The component's color.
   */
  REFLECT_API Color3 getColor();
  REFLECT_API void setPosition(Vector2 pos);
  REFLECT_API void *operator new(std::size_t size);
  REFLECT_API void operator delete(void *ptr) noexcept;
  REFLECT_API void *operator new[](std::size_t size);
  REFLECT_API void operator delete[](void *ptr) noexcept;

  friend class Window;
  friend class Label;
  friend class Button;
};

} // namespace reflect
