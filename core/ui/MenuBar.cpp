#include "MenuBar.h"
#include "Console.h"
namespace CinnamonToast {
	void MenuBar::Add(Component& comp) {
		if (dynamic_cast<MenuItem*>(&comp)) {
			
			MenuItem* comp2 = dynamic_cast<MenuItem*>(&comp);
			AppendMenu(hmenu, MF_POPUP, (UINT_PTR)comp2->hmenu, (comp2->GetText()).c_str());
		}
	};
	void MenuBar::Render(HWND& parentHWND, HWND& windowHWND) {
		SetMenu(windowHWND, hmenu);
	};
	MenuBar::MenuBar() : hmenu(CreateMenu()) {}
}