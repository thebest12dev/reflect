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
#ifdef _WIN32
#pragma once
#include "Colors.h"
#include "Object.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#include <windows.h>
#define COMPONENT_COMMON(classname)                                            \
  ctoast Vector2 classname::getSize() { return size; };                        \
  ctoast Vector2 classname::getPosition() { return position; };                \
  ctoast Color3 classname::getColor() { return color; };                       \
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
  CTOAST_API void render(HWND &parentHWND, HWND &windowHWND);                  \
  CTOAST_API void setVisible(bool flag);                                       \
  CTOAST_API void add(Component &comp);                                        \
  CTOAST_API void setSize(Vector2 size);                                       \
  CTOAST_API void setVisible(int cmd);                                         \
  CTOAST_API void setColor(uint8_t r, uint8_t g, uint8_t b);                   \
  CTOAST_API void setColor(Color3 color);                                      \
  CTOAST_API void setColor(Color3Array color);                                 \
  CTOAST_API virtual ~classname();                                             \
  CTOAST_API Vector2 getPosition();                                            \
  CTOAST_API Vector2 getSize();                                                \
  CTOAST_API bool getVisible();                                                \
  CTOAST_API Color3 getColor()

namespace CinnamonToast {
class Component : public Object {
protected:
  /// @brief The HINSTANCE object associated
  /// with the program required for window creation.
  HINSTANCE winstance;

  /// @brief The window handle (HWND) associated
  /// with the object needed for Win32 APIs.
  HWND hwnd;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 position;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 size;

  /// @brief The color of the
  /// component in RGB.
  Color3Float bgColor;

public:
  /**
   * @brief Creation of the component as well as
   * fields and other properties.
   */
  CTOAST_API Component(/* args */);

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
  CTOAST_API virtual void render(HWND &parentHWND, HWND &windowHWND) = 0;

  /**
   * @brief Sets the component visibility to either show or hide depending on
   * the flag.
   *
   * @param flag The flag to set visibility.
   */
  CTOAST_API void setVisible(bool flag);

  /**
   * @brief Adds a component to the hierarchy, as well as calling the `Render`
   * function of the child component to properly render it.
   */
  CTOAST_API void add(Component &comp);

  /**
   * @brief Sets the size of the component.
   */
  CTOAST_API void setSize(Vector2 size);

  /**
   * @brief Similar to `SetVisible(bool)` but directly use Win32 commands. On
   * Linux, this is mapped to the appropriate X11 commands.
   */
  CTOAST_API void setVisible(int cmd);

  /**
   * @brief Sets the color of the component by its red, green and blue
   * components.
   */
  CTOAST_API void setColor(uint8_t r, uint8_t g, uint8_t b);

  /**
   * @brief Similar to SetColor(uint8_t, uint8_t, uint8_t) but with a Color3
   * struct.
   */
  CTOAST_API void setColor(Color3 color);

  /**
   * @brief Same as other methods but with an int[3] type.
   */
  CTOAST_API void setColor(Color3Array color);

  /**
   * @brief Virtual destructor needed for runtime polymorphism.
   */
  CTOAST_API virtual ~Component() = default; // To allow dynamic_cast to work

  /**
   * @brief Returns the component's position.
   * @returns The component's position.
   */
  CTOAST_API Vector2 getPosition();
  /**
   * @brief Returns the component's size.
   * @returns The component's size.
   */
  CTOAST_API Vector2 getSize();
  /**
   * @brief Returns the component's visibility.
   * @returns The component's visibility.
   */
  CTOAST_API bool getVisible();
  /**
   * @brief Returns the component's color.
   * @returns The component's color.
   */
  CTOAST_API Color3 getColor();

  CTOAST_API void *operator new(std::size_t size);
  CTOAST_API void operator delete(void *ptr) noexcept;
  CTOAST_API void *operator new[](std::size_t size);
  CTOAST_API void operator delete[](void *ptr) noexcept;

  friend class Window;
  friend class Label;
  friend class Button;
};

} // namespace CinnamonToast
#elif __linux__
#pragma once
#include <cstdint>
// #include "Window.h"
#include "Colors.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <vector>
// namespace CinnamonToast {
// class Component {
// protected:
//   Vector2 position;
//   Vector2 size;
//   bool visible;

//   Color3 color;
//   std::vector<Component *> children;

// public:
//   Component(/* args */);
//   CTOAST_API void SetVisible(bool flag);
//   CTOAST_API void Add(Component *comp);
//   CTOAST_API void SetSize(Vector2 size);
//   CTOAST_API Vector2 GetSize();
//   CTOAST_API Vector2 GetPosition();
//   CTOAST_API bool GetVisible();
//   CTOAST_API int *GetColor();
//   virtual CTOAST_API std::string GetProperty(std::string name) {
//     return "null";
//   };
//   CTOAST_API void SetVisible(int cmd);
//   CTOAST_API void SetColor(uint8_t r, uint8_t g, uint8_t b);
//   virtual ~Component(){};
//   friend class Window;
//   friend class Label;
//   friend class Button;
// };
// } // namespace CinnamonToast

// #endif
namespace CinnamonToast {
class Component {
protected:
  /// @brief The Display object associated
  /// with the program required for window creation.
  Display *winstance;

  /// @brief The window handle (XWindow) associated
  /// with the object needed for X11 APIs.
  XWindow hwnd;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 position;

  /// @brief The position of the component
  /// in X and Y coordinates.
  Vector2 size;

  /// @brief The color of the
  /// component in RGB.
  Color3 color;

  /// @brief A Linux-specific vector of child components.
  ///
  /// Since this is a
  /// platform-specific implementation, please do not interact with this
  /// property and use cross-platform methods instead. This is only used due to
  /// the limitations of X11.
  ///
  std::vector<Component *> children;

public:
  /**
   * @brief Creation of the component as well as
   * fields and other properties.
   */
  CTOAST_API Component(/* args */);

  /**
   * @brief Method to render the component, primarily handled by the derived
   * class.
   *
   * It uses 2 arguments, the parent window handle and the window's window
   * handle. Using these arguments, the component can be attached to the parent
   * window and rendered as necessary. Note that this is intentionally left
   * blank in some use cases, but it is not recommended to leave it blank.
   *
   * @param parentWindow The parent window handle (obtained through
   * X11 APIs).
   * @param windowWindow The window window handle (similar to the parent handle
   * but for the window).
   */
  CTOAST_API virtual void render(XWindow &parentHWND, XWindow &windowHWND) = 0;

  /**
   * @brief Sets the component visibility to either show or hide depending on
   * the flag.
   *
   * @param flag The flag to set visibility.
   */
  CTOAST_API void setVisible(bool flag);

  /**
   * @brief Adds a component to the hierarchy, as well as calling the `Render`
   * function of the child component to properly render it.
   */
  CTOAST_API void add(Component &comp);

  /**
   * @brief Sets the size of the component.
   */
  CTOAST_API void setSize(Vector2 size);

  /**
   * @brief Similar to `SetVisible(bool)` but directly use Win32 commands.
   */
  CTOAST_API void setVisible(int cmd);

  /**
   * @brief Sets the color of the component by its red, green and blue
   * components.
   */
  CTOAST_API void setColor(uint8_t r, uint8_t g, uint8_t b);

  /**
   * @brief Similar to SetColor(uint8_t, uint8_t, uint8_t) but with a Color3
   * struct.
   */
  CTOAST_API void setColor(Color3 color);

  /**
   * @brief Returns the component's position.
   * @returns The component's position.
   */
  CTOAST_API Vector2 getPosition();
  /**
   * @brief Returns the component's size.
   * @returns The component's size.
   */
  CTOAST_API Vector2 getSize();
  /**
   * @brief Returns the component's visibility.
   * @returns The component's visibility.
   */
  CTOAST_API virtual bool getVisible() { return false; };
  /**
   * @brief Returns the component's color.
   * @returns The component's color.
   */
  CTOAST_API Color3 getColor();

  /**
   * @brief Same as other methods but with an int[3] type.
   */
  CTOAST_API void setColor(Color3Array color);

  /**
   * @brief Virtual destructor needed for runtime polymorphism.
   */
  CTOAST_API virtual ~Component() = default; // To allow dynamic_cast to work
  friend class Window;
  friend class Label;
  friend class Button;
};
} // namespace CinnamonToast
#endif