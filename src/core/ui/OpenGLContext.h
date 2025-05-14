#pragma once
#include "TypeDefinitions.h"
#include <Windows.h>
typedef HWND WindowHandle;
namespace reflect {
class OpenGLContext {
public:
  REFLECT_API void initializeContext(WindowHandle hwnd, HDC hdc);
};
} // namespace reflect