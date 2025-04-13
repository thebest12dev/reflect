#include "OpenGLContext.h"
#include "Console.h"
#include "Windows.h"

namespace CinnamonToast {
void OpenGLContext::initializeContext(WindowHandle hwnd, HDC hdc) {

  if (!hdc) {
    std::cerr << "Failed to get device context!" << std::endl;
    return;
  }

  // Step 2: Set up the pixel format descriptor
  PIXELFORMATDESCRIPTOR pfd = {};
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cRedBits = 8;
  pfd.cGreenBits = 8;
  pfd.cBlueBits = 8;
  pfd.cAlphaBits = 8;
  pfd.cDepthBits = 24;
  pfd.iLayerType = PFD_MAIN_PLANE;

  // Step 3: Choose a pixel format
  int pixelFormat = ChoosePixelFormat(hdc, &pfd);
  if (pixelFormat == 0) {
    std::cerr << "Failed to choose pixel format!" << std::endl;
    return;
  }

  // Step 4: Set the pixel format for the window's DC
  if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
    std::cerr << "Failed to set pixel format!" << std::endl;
    return;
  }

  // Step 5: Create an OpenGL rendering context (RC)
  HGLRC hglrc = wglCreateContext(hdc);
  if (!hglrc) {
    std::cerr << "Failed to create OpenGL rendering context!" << std::endl;
    return;
  }

  if (!wglMakeCurrent(hdc, hglrc)) {
    std::cerr << "Failed to make OpenGL context current!" << std::endl;
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    return;
  }

  // Step 7: Optionally, initialize OpenGL extensions (if needed)
  // You can use GLEW or another extension loader library here, if necessary

  // Now OpenGL is initialized for the window, you can render
  CinnamonToast::Console::debug("OpenGL initialized successfully!",
                                "initializeContext");
  ReleaseDC(hwnd, hdc);
  return;
}
} // namespace CinnamonToast