#include "MenuBar.h"
#include "Console.h"
namespace reflect {
void MenuBar::add(Component &comp) {
  if (dynamic_cast<MenuItem *>(&comp)) {

    MenuItem *comp2 = dynamic_cast<MenuItem *>(&comp);
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)comp2->hmenu,
               (comp2->getText()).c_str());
  }
};
void MenuBar::render(WindowHandle &parentHWND, WindowHandle &windowHWND) {
  SetMenu(windowHWND, hmenu);
};
MenuBar::MenuBar() : hmenu(CreateMenu()) {
  initializeObject(REFLECT_OBJECT_MENUBAR, REFLECT_OBJECT_COMPONENT);
}
} // namespace reflect