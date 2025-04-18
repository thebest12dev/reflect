#include "TextField.h"
#include "../Utilities.h"
#include "Console.h"
#include <CommCtrl.h>
namespace {
WNDPROC originalEditProc = nullptr; // Store the original window procedure
} // namespace
namespace CinnamonToast {
LRESULT CALLBACK TextField::editProc(HWND hwnd, UINT msg, WPARAM wParam,
                                     LPARAM lParam) {
  TextField *pThis = nullptr;
  pThis = reinterpret_cast<ctoast TextField *>(
      GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (msg) {
  case WM_SETFOCUS:
    pThis->focused = true;
    if (pThis->focusCallback)
      pThis->focusCallback(*pThis);
    break;
  case WM_KILLFOCUS:
    pThis->focused = false;
    break;
  }
  return CallWindowProc(originalEditProc, hwnd, msg, wParam, lParam);
}
TextField::TextField()
    : winstance(GetModuleHandle(nullptr)), hwnd(nullptr), position(0, 0),
      size(200, 20), bgColor(0.2f, 0.2f, 0.2f), focused(false),
      focusCallback(nullptr) {}
void TextField::setSize(Vector2 size) { this->size = size; };
void TextField::setPosition(Vector2 pos) { position = pos; };
void TextField::render(HWND &parentHWND, HWND &windowHWND) {
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, // Extended styles
                        WC_EDIT,          // Class name
                        "",               // Initial text
                        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, position.x,
                        position.y, size.x, size.y,

                        parentHWND, // Parent window
                        NULL,
                        winstance, // App instance
                        this       // Additional data
  );
  originalEditProc =
      (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)editProc);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  SendMessage(hwnd, WM_SETFONT, (WPARAM)CinnamonToast::Utilities::getFont(),
              TRUE);
}
void TextField::setText(std::string text) {
  this->text = text;
  SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)text.c_str());
}
std::string TextField::getText() {
  char buffer[256];
  SendMessage(hwnd, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
  return std::string(buffer);
}

bool TextField::isFocused() { return focused; }
void TextField::focus() {
  SetFocus(hwnd);
  focused = true;
}
void TextField::onFocus(void (*callback)(TextField &)) {
  focusCallback = callback;
}
void TextField::setFont(std::string font) {
  fontStr = font;
  SendMessage(hwnd, WM_SETFONT,
              (WPARAM)CinnamonToast::Utilities::getFont(fontStr, fontSize),
              TRUE);
}
} // namespace CinnamonToast