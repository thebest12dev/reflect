#pragma once
#ifdef _WIN32
#pragma once
#include "Colors.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <cstdint>
#include <windows.h>

namespace CinnamonToast {
class TextField : public TextComponent {
protected:
  HINSTANCE winstance;
  HWND hwnd;

  Vector2 position;

  Vector2 size;
  Color3Float bgColor;
  void (*focusCallback)(TextField &);
  bool focused;
  CTOAST_API static LRESULT CALLBACK editProc(HWND hwnd, UINT uMsg,
                                              WPARAM wParam, LPARAM lParam);

public:
  CTOAST_API TextField();
  CTOAST_API void render(HWND &parentHWND, HWND &windowHWND);
  CTOAST_API void setText(std::string text);
  CTOAST_API std::string getText();
  CTOAST_API void setSize(Vector2 size);
  CTOAST_API void setPosition(Vector2 pos);
  CTOAST_API bool isFocused();
  CTOAST_API void setFont(std::string font);
  CTOAST_API void focus();
  CTOAST_API void onFocus(void (*callback)(TextField &));
};
} // namespace CinnamonToast
#endif