#pragma once
#include "TypeDefinitions.h"
#include <Windows.h>
typedef HWND WindowHandle;
namespace CinnamonToast {
class OpenGLContext {
public:
  CTOAST_API void initializeContext(WindowHandle hwnd);
};
} // namespace CinnamonToast