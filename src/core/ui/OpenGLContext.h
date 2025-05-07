#pragma once
#include "TypeDefinitions.h"
#include <Windows.h>
typedef HWND WindowHandle;
namespace cinnamontoast {
class OpenGLContext {
public:
  CTOAST_API void initializeContext(WindowHandle hwnd, HDC hdc);
};
} // namespace cinnamontoast