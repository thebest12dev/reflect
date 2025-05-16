#pragma once
#ifdef _WIN32
#pragma once
#include "Colors.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#include <windows.h>

namespace reflect {
class TextField : public TextComponent {
protected:
  ApplicationHandle winstance;
  WindowHandle hwnd;

  Vector2 position;
  std::map<std::string, bool> pressedKeys;
  Vector2 size;
  Color3Float bgColor;
  void (*focusCallback)(TextField &);
  bool focused;
  REFLECT_API static LRESULT CALLBACK editProc(WindowHandle hwnd, UINT uMsg,
                                               WPARAM wParam, LPARAM lParam);

public:
  REFLECT_API TextField();
  REFLECT_API void render(WindowHandle &parentHWND, WindowHandle &windowHWND);
  REFLECT_API void setText(std::string text);
  REFLECT_API std::string getText();
  REFLECT_API void setSize(Vector2 size);
  REFLECT_API void setPosition(Vector2 pos);
  REFLECT_API bool isFocused();
  REFLECT_API void setFont(std::string font);
  REFLECT_API void focus();
  REFLECT_API void onFocus(void (*callback)(TextField &));
};
} // namespace reflect
#endif