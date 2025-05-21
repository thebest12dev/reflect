#include "Container.h"
#include "Components.h"
#include <Console.h>

namespace reflect {

void Container::registerClass() {
  WNDCLASS wc = {};
  wc.lpszClassName = "ReflectContainer";
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpfnWndProc = containerProc;
  RegisterClass(&wc);
}
Container::Container() {
  initializeObject(REFLECT_OBJECT_CONTAINER, REFLECT_OBJECT_COMPONENT);
  registerClass();
  hwnd = CreateWindowEx(0, "ReflectContainer", nullptr, 0, 0, 0, 10, 10,
                        nullptr, nullptr, GetModuleHandle(nullptr), this);
}
void Container::render(HWND &parentHWND, HWND &windowHWND_) {
  windowHWND = windowHWND_;
  SetParent(hwnd, parentHWND);
  SetWindowLongPtr(hwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);

  ShowWindow(hwnd, SW_SHOW);

  for (Component *comp : componentsQueue) {
    reflectDebug("added new component");
    comp->winstance = this->winstance;
    comp->render(this->hwnd, windowHWND);
  }
}
void Container::add(reflect::Component &comp, std::string id) {

  if (Components::gchildren[id] == nullptr) {
    Components::gchildren[id] = &comp;

    if (!windowHWND) {
      componentsQueue.push_back(&comp);
    } else {

      reflectDebug("added new component");
      comp.winstance = this->winstance;
      comp.render(this->hwnd, windowHWND);
    }
  }
}
void Container::add(reflect::Component &comp) {

  if (!windowHWND) {
    componentsQueue.push_back(&comp);
  } else {
    reflectDebug("added new component");
    comp.winstance = this->winstance;
    comp.render(this->hwnd, windowHWND);
  }
}
LRESULT CALLBACK Container::containerProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                          LPARAM lParam) {
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
} // namespace reflect