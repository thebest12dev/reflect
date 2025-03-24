#include "MenuItem.h"
#include "Console.h"
namespace CinnamonToast {
	MenuItem::MenuItem() : hmenu(CreatePopupMenu()), size(Vector2(0,0)){
		// do nothing
	}
	MenuItem::MenuItem(string contents) : hmenu(CreatePopupMenu()), text(contents), size(Vector2(0, 0)) {
		// do nothing
	}
	void MenuItem::Render(HWND& parentHWND, HWND& windowHWND) {
		// do nothing
	}
	void MenuItem::SetText(string contents) {
		text = contents;
	}
	void MenuItem::SetFontSize(int font) {
		fontSize = font;
	}
	void MenuItem::SetFont(string font) {
		fontStr = font;
	}
	string MenuItem::GetText() {
		return text;
	}
	void MenuItem::Add(MenuItem& comp) {

		AppendMenu(hmenu, MF_STRING, 2, comp.GetText().c_str());
	}
}