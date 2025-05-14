#include "MenuItem.h"
#include "Console.h"
namespace reflect {
MenuItem::MenuItem() : hmenu(CreatePopupMenu()), size(Vector2(0, 0)) {
  initializeObject(REFLECT_OBJECT_MENUITEM, REFLECT_OBJECT_TEXTCOMPONENT);
  // do nothing
}
MenuItem::MenuItem(std::string contents)
    : hmenu(CreatePopupMenu()), text(contents), size(Vector2(0, 0)) {
  // do nothing
  initializeObject(REFLECT_OBJECT_MENUITEM, REFLECT_OBJECT_TEXTCOMPONENT);
}
void MenuItem::render(HWND &parentHWND, HWND &windowHWND) {
  // do nothing
}
void MenuItem::setText(std::string contents) { text = contents; }
void MenuItem::setFontSize(int font) { fontSize = font; }
void MenuItem::setFont(std::string font) { fontStr = font; }
std::string MenuItem::getText() { return text; }
void MenuItem::add(MenuItem &comp) {

  AppendMenu(hmenu, MF_STRING, 2, comp.getText().c_str());
}
} // namespace reflect