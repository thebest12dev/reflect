#pragma once

#include "Component.h"
#include "TypeDefinitions.h"

namespace reflect {
class Container : public Component {
private:
  static LRESULT CALLBACK containerProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                        LPARAM lParam);
  void registerClass();
  HWND windowHWND = nullptr;
  std::vector<Component *> componentsQueue;

public:
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  REFLECT_API Container();
  REFLECT_API void add(Component &comp, std::string id);
  REFLECT_API void add(Component &comp);
};
} // namespace reflect