#include "MenuBar.h"
#include "Console.h"
namespace cinnamontoast {
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
MenuBar::MenuBar() : hmenu(CreateMenu()) {
  initializeObject(CTOAST_OBJECT_MENUBAR, CTOAST_OBJECT_COMPONENT);
}
} // namespace cinnamontoast