#pragma once
#include "Component.h"

namespace reflect {
class ProgressBar : public Component {
private:
  float minValue;
  float value;
  float maxValue;
  /// @brief The ApplicationHandle object associated
  /// with the program required for window creation.
  ApplicationHandle winstance;

  /// @brief The window handle (WindowHandle) associated
  /// with the object needed for Win32 APIs.
  WindowHandle hwnd;

  /// @brief The color of the
  /// component in RGB.
  Color3Float bgColor;

public:
  // COMPONENT_DECL(ProgressBar);
  ProgressBar();
  REFLECT_API void setPosition(Vector2 pos);
  REFLECT_API void render(WindowHandle &parentHWND, WindowHandle &windowHWND);
  void setValue(float value);
  void setMaximumValue(float value);
  void setMininumValue(float value);
  float getValue();
  float getMaximumValue();
  float getMinimumValue();
};
} // namespace reflect