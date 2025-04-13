#pragma once
#include "Component.h"

namespace CinnamonToast {
class ProgressBar : public Component {
private:
  float minValue;
  float value;
  float maxValue;
  /// @brief The HINSTANCE object associated
  /// with the program required for window creation.
  HINSTANCE winstance;

  /// @brief The window handle (HWND) associated
  /// with the object needed for Win32 APIs.
  HWND hwnd;

  /// @brief The color of the
  /// component in RGB.
  Color3Float bgColor;

public:
  // COMPONENT_DECL(ProgressBar);
  ProgressBar();
  CTOAST_API void setPosition(Vector2 pos);
  CTOAST_API void render(HWND &parentHWND, HWND &windowHWND);
  void setValue(float value);
  void setMaximumValue(float value);
  void setMininumValue(float value);
  float getValue();
  float getMaximumValue();
  float getMinimumValue();
};
} // namespace CinnamonToast