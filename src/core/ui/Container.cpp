#include "Container.h"
#include "Canvas.h"
#include "Components.h"
#include <Console.h>

namespace reflect {

// void Container::registerClass() {
//   WNDCLASS wc = {};
//   wc.lpszClassName = "ReflectContainer";
//   wc.hInstance = GetModuleHandle(nullptr);
//   wc.lpfnWndProc = containerProc;
//   RegisterClass(&wc);
// }
Container::Container() {
  initializeObject(REFLECT_OBJECT_CONTAINER, REFLECT_OBJECT_COMPONENT);
  this->setClassName("Container");
  this->setStyles(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
}
// void Container::render(HWND &parentHWND, HWND &windowHWND_) {
//   windowHWND = windowHWND_;
//   registerClass();
//   hwnd =
//       CreateWindowEx(0, "ReflectContainer", nullptr,
//                      WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
//                      WS_CLIPSIBLINGS, position.x, position.y, size.x, size.y,
//                      parentHWND, nullptr, GetModuleHandle(nullptr), this);
//
//   for (Component *comp : componentsQueue) {
//     reflectDebug("added new component");
//     comp->winstance = this->winstance;
//     comp->render(this->hwnd, windowHWND);
//   }
// }
void Container::onPaint() {
  Canvas &canvas = getCanvas();
  canvas.beginDraw();
  canvas.background(bgColor);
  canvas.endDraw();
}

void Container::onCreate() {
  for (Component *comp : componentsQueue) {
    reflectDebug("added new component");
    comp->winstance = this->winstance;
    comp->render(this->hwnd, getRootWindow());
  }
}

void Container::add(reflect::Component &comp, std::string id) {

  if (Components::gchildren[id] == nullptr) {
    Components::gchildren[id] = &comp;

    if (!getRootWindow()) {
      componentsQueue.push_back(&comp);
    } else {

      reflectDebug("added new component");
      comp.winstance = this->winstance;
      comp.render(this->hwnd, getRootWindow());
    }
  }
}
void Container::add(reflect::Component &comp) {

  if (!getRootWindow()) {
    componentsQueue.push_back(&comp);
  } else {
    reflectDebug("added new component");
    comp.winstance = this->winstance;
    comp.render(this->hwnd, getRootWindow());
  }
}
} // namespace reflect