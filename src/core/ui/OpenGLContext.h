#pragma once
#include "TypeDefinitions.h"
#ifdef _WIN32
#include <Windows.h>
#endif
namespace reflect {
class OpenGLContext {
public:
  REFLECT_API void initializeContext(HWND hwnd, HDC hdc);
};
} // namespace reflect