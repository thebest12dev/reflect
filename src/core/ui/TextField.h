#pragma once
#ifdef _WIN32
#pragma once
#include "Colors.h"
#include "TextComponent.h"
#include "TypeDefinitions.h"
#include "Vector2.h"
#include <chrono>
#include <cstdint>
#include <windows.h>

namespace reflect {
class REFLECT_API TextField : public TextComponent {
protected:
  // HINSTANCE winstance;
  // HWND hwnd;

  // Vector2 position;
  std::map<std::string, bool> pressedKeys;
  // Vector2 size;
  // Color3Float bgColor;
  void (*focusCallback)(TextField &);
  bool focused;
  static LRESULT CALLBACK editProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam);
  int cursorIndex = 0;

private:
  bool cursorBlinking = false;
  int interval = 500;
  std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();

public:
  TextField();
  // REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  // REFLECT_API void setSize(Vector2 size);
  // REFLECT_API void setPosition(Vector2 pos);
  bool isFocused();
  void focus();
  void onPaint(PaintEvent event) override;
  void onKeyPressed(KeyboardEvent key) override;
  void onUpdate(UpdateEvent update) override;
  void onFocus(void (*callback)(TextField &));
};
} // namespace reflect
#endif