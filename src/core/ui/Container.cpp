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
}
void Container::render(HWND &parentHWND, HWND &windowHWND_) {
  windowHWND = windowHWND_;
  registerClass();
  hwnd =
      CreateWindowEx(0, "ReflectContainer", nullptr,
                     WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                     position.x, position.y, size.x, size.y, parentHWND,
                     nullptr, GetModuleHandle(nullptr), this);

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
  if (GetParent(hwnd) != nullptr) {

    reflect::Container *pThis = nullptr;

    if (uMsg == WM_CREATE) {
      CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
      pThis = reinterpret_cast<reflect::Container *>(pCreate->lpCreateParams);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
      reflect::Window *win = reinterpret_cast<reflect::Window *>(
          GetWindowLongPtr(GetAncestor(hwnd, GA_ROOT), GWLP_USERDATA));

      RECT rc;

      GetClientRect(hwnd, &rc);
      // Initialize Direct2D
      ID2D1Factory *pFactory =
          win->getProperty<ID2D1Factory *>("direct2DFactory");
      D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
      D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
          D2D1::HwndRenderTargetProperties(
              hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

      HRESULT hr = pFactory->CreateHwndRenderTarget(
          rtProps, hwndRTProps,
          &(pThis->childRenderTarget)); // <--- store it on pThis

    } else {
      pThis = reinterpret_cast<reflect::Container *>(
          GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (pThis) {
      switch (uMsg) {
      case WM_PAINT: {
        if (pThis->childRenderTarget) {
          PAINTSTRUCT ps;
          BeginPaint(hwnd, &ps);
          pThis->childRenderTarget->BeginDraw();
          ID2D1SolidColorBrush *brush = nullptr;
          pThis->childRenderTarget->CreateSolidColorBrush(
              D2D1_COLOR_F(pThis->bgColor[0], pThis->bgColor[1],
                           pThis->bgColor[2]),
              &brush);
          pThis->childRenderTarget->FillRectangle(
              D2D1_RECT_F(0, 0, pThis->size.x, pThis->size.y), brush);
          pThis->childRenderTarget->EndDraw();

          brush->Release();
          EndPaint(hwnd, &ps);
          return 0;
        }
        break;
      }
      }
    }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
};
} // namespace reflect