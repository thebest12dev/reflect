#include "MenuBar.h"
#include "Console.h"
namespace CinnamonToast {
void MenuBar::add(Component &comp) {
  if (dynamic_cast<MenuItem *>(&comp)) {

    MenuItem *comp2 = dynamic_cast<MenuItem *>(&comp);
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)comp2->hmenu,
               (comp2->getText()).c_str());
  }
};
void MenuBar::render(HWND &parentHWND, HWND &windowHWND) {
  SetMenu(windowHWND, hmenu);
};
MenuBar::MenuBar() : hmenu(CreateMenu()) {}
} // namespace CinnamonToast