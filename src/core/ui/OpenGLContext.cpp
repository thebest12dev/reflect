#include "OpenGLContext.h"
#include "Console.h"
#include "Windows.h"
#include <GL/glew.h>
#include <GL/wglew.h>
namespace reflect {
void OpenGLContext::initializeContext(HWND hwnd, HDC hdc) {

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
  glewExperimental = GL_TRUE; // Important for core profile features

  if (glewInit() != GLEW_OK) {
    MessageBoxA(0, "Failed to initialize GLEW!", "Error", MB_ICONERROR | MB_OK);
    return;
  }
  int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                   3,
                   WGL_CONTEXT_MINOR_VERSION_ARB,
                   3,
                   WGL_CONTEXT_PROFILE_MASK_ARB,
                   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                   0};

  HGLRC modernContext = wglCreateContextAttribsARB(hdc, 0, attribs);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hglrc);            // Kill the dummy
  wglMakeCurrent(hdc, modernContext); // Use the new context

  // Step 7: Optionally, initialize OpenGL extensions (if needed)
  // You can use GLEW or another extension loader library here, if necessary

  // Now OpenGL is initialized for the window, you can render
  reflect::console::debug("OpenGL initialized successfully!",
                          "initializeContext");
  // ReleaseDC(hwnd, hdc);
}
} // namespace reflect