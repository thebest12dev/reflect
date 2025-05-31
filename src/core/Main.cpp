/**
 * Copyright (c) 2025 thebest12lines
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// define byte as unsigned char to prevent conflicts
typedef unsigned char byte;

// if win32
#ifdef _WIN32
// include windows.h
#include <windows.h>
#endif

// standard libraries we need
#include "logging/LogBuffer.h"
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// other libraries
#include "tinyxml2.h"
#ifdef REFLECT_LUA

#include "../api/lua/ReflectLua.h"

#endif

// our header files here
#include "Console.h"
#include "CrashHandler.h"
#include "Definitions.h"
#include "Main.h"
#include "ReflectAPI.h"
#include "TypeDefinitions.h"
#include "Utilities.h"
#include "ui/TextField.h"
// ui components
#include "ui/Button.h"
#include "ui/Canvas.h"
#include "ui/Colors.h"
#include "ui/Component.h"
#include "ui/Components.h"
#include "ui/Container.h"
#include "ui/Image.h"
#include "ui/Label.h"
#include "ui/MenuItem.h"
#include "ui/TextComponent.h"
#include "ui/Vector2.h"
#include <numbers>
#ifdef REFLECT_LUA
#include "LuaAPI.h"
#endif
// windows-specific code
#ifdef _WIN32

// include commctrl.h (for visual styles)
#include <commctrl.h>

// standard libs
#include "memory/HeapPool.h"
#include "ui/MenuBar.h"
#include "ui/Notification.h"
#include "ui/ProgressBar.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory> // for std::unique_ptr
#include <string>
#include <vector>
// using namespaces and alias
using namespace reflect::console;
using namespace reflect::utilities;

namespace fs = std::filesystem;

namespace {
void onExecute(reflect::Window &win) { /*win.Close();*/ }
std::vector<void *> heapAllocations;
struct Cleaner {
  ~Cleaner() {
    for (void *ptr : heapAllocations) {
      reflect::getHeapPool()->deallocate(ptr, sizeof(ptr));
    }
  }
};
Cleaner cleaner;

} // namespace
#include "ui/Canvas.h"
#include "xml/ProcessorRegistry.h"
#include <ShellScalingAPI.h>
#include <numbers>
#include <random>
#pragma comment(lib, "Shcore.lib")

void reflect::addToHeap(void *ptr) { heapAllocations.push_back(ptr); };
/**
 * Invokes and loads a .xml file and also loads the specific libraries. It will
 * setup the GUI as well as registering APIs for the libraries to use.
 */
int reflect::invokeExecutable(std::string xmlFile, bool blocking) {
  // if lua enabled
  /*INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;

  InitCommonControlsEx(&icex);*/

  InitCommonControls();
  HRESULT hr = CoInitialize(nullptr);
  // load the xml file
  tinyxml2::XMLDocument doc;
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
  // platform
  reflectInfo("platform: " + getOSPlatformAndVersion());

  // common control stuff

  // file doesnt exist
  if (!fs::exists(xmlFile)) {
    // reflectError and return
    reflectError("file not found!");
    return ERROR_FILE_NOT_FOUND;
  }

  // reflectDebug print and load xml file
  reflectDebug("loading XML file...");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  // reflectError handling
  if (eResult != tinyxml2::XML_SUCCESS) {
    reflectError(
        std::string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return REFLECT_ERROR_GENERIC_XML_ERROR;
  }

  // get the root element
  reflectDebug("getting root...");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  // handling
  if (root == nullptr) {
    reflectError("no root element found.");
    return REFLECT_ERROR_XML_NO_ROOT;
  }

  // checking compat
  reflectDebug("file is compatible with app?");
  const uint32_t version = std::stoi(root->Attribute("version"));
  if (version > APP_INTERNAL_VERSION) {
    // handle and return
    reflectError("file not compatible! please upgrade to a newer version.");
    return REFLECT_ERROR_XML_NOT_COMPATIBLE;
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    reflectError("no window element found.");
    return REFLECT_ERROR_XML_NO_WINDOW;
  }
  std::string winId = winXml->Attribute("id");
  tinyxml2::XMLElement *gctx = winXml->FirstChildElement("graphicsContext");
#ifdef _WIN32
  Window *win = nullptr;
  if (gctx == nullptr) {
    reflectDebug("getting HINSTANCE...");
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    reflectDebug("creating window...");

    // OpenGLContext ctx;
    // WindowCreateInfo info = {true};
    win = new Window(hInstance, winId);
    /*win->setBeforeRenderLoop([](Window &win) {
      wglSwapIntervalEXT(1);
      int i = 12;
      std::cout << "i" << std::endl;
    });
    win->setRenderLoop([](Window &win) {
      reflectDebug(win.isKeyPressed('W'));
      glClearColor(1.0, 0.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      win.swapBuffers();
    });*/
  } else {
    reflectDebug("graphics context found!");
    std::string ctx = gctx->Attribute("type");
    if (ctx == "opengl") {
      reflectDebug("using custom OpenGL context...");
      reflectWarn(
          "Since a custom graphics context is specified, UI controls (buttons, "
          "labels, etc.) will not render. This is only for if you want to "
          "develop things like games or simulations. (To disable this "
          "warning, set contextSetWarning to false in your XML file.");
      reflectDebug("getting HINSTANCE...");
      HINSTANCE hInstance = GetModuleHandle(nullptr);
      reflectDebug("creating window...");
      OpenGLContext ctx;
      win = new Window(hInstance, ctx, winId);
    }
  }
  win->addStyle(STYLE_DARK_TITLE_BAR);
#endif

  reflectDebug("window title: " + std::string(winXml->Attribute("title")));
  win->setTitle(winXml->Attribute("title"));
  reflectDebug("resizing window...");
  win->setSize(Vector2(std::stoi(winXml->Attribute("width")),
                       std::stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  reflectDebug("setting window background color...");
  if (bgColor == "systemDefault") {
    win->setColor(255, 255, 255);
  } else {
    if (bgColor.at(0) == '#' && bgColor.length() == 7) {

      const std::string hex = bgColor.substr(1);

      const uint8_t r = stoi(hex.substr(0, 2), nullptr, 16);
      const uint8_t g = stoi(hex.substr(2, 2), nullptr, 16);
      const uint8_t b = stoi(hex.substr(4, 2), nullptr, 16);
      const Color3 color = {r, g, b};
      win->setColor(color);
    } else {
      reflectError("invalid hex color representation");
      return REFLECT_ERROR_HEX_COLOR_MALFORMED;
    }
  }
  for (tinyxml2::XMLElement *element = winXml->FirstChildElement();
       element != nullptr; element = element->NextSiblingElement()) {
    std::pair<Component &, std::string> comp =
        reflect::ProcessorRegistry::invokeProcessor(element->Name(), win,
                                                    element);
    win->add(comp.first, comp.second);
    heapAllocations.push_back(&comp.first);
  }
  reflectDebug("loading libraries...");
#ifdef REFLECT_LUA
  LuaInstance *lua = nullptr;
#endif
  for (tinyxml2::XMLElement *sharedLib = winXml->FirstChildElement("library");
       sharedLib != nullptr;
       sharedLib = sharedLib->NextSiblingElement("library")) {
    if (std::string(sharedLib->Attribute("type")) == "native") {
      HMODULE hDll = LoadLibrary(
          (const char
               *)(fs::absolute(fs::path(xmlFile)).parent_path().string() +
                  "\\" + sharedLib->Attribute("location"))
              .c_str());
      if (!hDll) {
        reflectError("cannot load shared library");
        return REFLECT_ERROR_CANNOT_LOAD_SHARED_LIBRARY;
      }
      reflectDebug("loaded shared library!");

      // Get the address of the Add function
      reflect::SharedLibraryMain mainFunc =
          (reflect::SharedLibraryMain)GetProcAddress(hDll, "reflectMain");
      if (!mainFunc) {
        reflectError("cannot find reflectMain function of library!");

        FreeLibrary(hDll); // Free the DLL
        return REFLECT_ERROR_CANNOT_LOAD_LIBRARY_FUNCTION;
      }
      reflect::ReflectAPI reflectApi = {};
      reflectApi.addComponent = external::addComponent;
      reflectApi.getComponentById = external::getComponentById;
      reflectApi.getText = external::getComponentText;
      reflectApi.setColor = external::setComponentColor;
      reflectApi.setFont = external::setComponentFont;
      reflectApi.setFontSize = external::setComponentFontSize;
      reflectApi.setVisible = external::setComponentVisible;
      reflectApi.setVisibleCommand = external::setComponentVisibleCommand;

      reflectApi.setOnClick = external::setOnClick;
      std::thread *thread = new std::thread(mainFunc, &reflectApi);
    }
#ifdef REFLECT_LUA
    else if (std::string(sharedLib->Attribute("type")) == "lua") {
      reflectDebug("loading lua file...");
      if (lua == nullptr) {
        reflectDebug("initializing lua...");
        lua = new LuaInstance();
        // heapAllocations.push_back(lua);
        lua->initializeLuaApis(injectLuaApis);
      }

      lua->executeFile((fs::absolute(fs::path(xmlFile)).parent_path().string() +
                        "\\" + sharedLib->Attribute("location"))
                           .c_str());
    }
#endif
  }
  tinyxml2::XMLElement *menu = winXml->FirstChildElement("menuBar");
  if (menu) {
    MenuBar *menuBar = new MenuBar();
    heapAllocations.push_back(menuBar);
    for (tinyxml2::XMLElement *menuItem = menu->FirstChildElement("menuItem");
         menuItem != nullptr;
         menuItem = menuItem->NextSiblingElement("menuItem")) {
      MenuItem *item = new MenuItem(menuItem->GetText());
      heapAllocations.push_back(item);
      menuBar->add(*item);
    }
    win->add(*menuBar);
  }
  reflectDebug("entering main loop...");
  // Canvas canvas;
  // canvas.setSize({1366, 768});
  // canvas.setPosition({0, 0});

  // win->add(canvas);

  // canvas.strokeWeight(10);
  /*canvas.background({20, 100, 255});
  canvas.stroke({0, 70, 156});
  canvas.fill({30, 255, 0});
  canvas.rect({100, 100}, {100, 100});*/

  // int x = 100, y = 100;
  // float vx = 1.0f, vy = 1.0f; // initial velocity
  // const int width = 1366;
  // const int height = 768;
  // const int rectSize = 100;
  // auto hsvToRGB = [](float h, float s, float v, float &r, float &g, float &b)
  // {
  //   float c = v * s;
  //   float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
  //   float m = v - c;

  //  float r1, g1, b1;

  //  if (h < 60) {
  //    r1 = c;
  //    g1 = x;
  //    b1 = 0;
  //  } else if (h < 120) {
  //    r1 = x;
  //    g1 = c;
  //    b1 = 0;
  //  } else if (h < 180) {
  //    r1 = 0;
  //    g1 = c;
  //    b1 = x;
  //  } else if (h < 240) {
  //    r1 = 0;
  //    g1 = x;
  //    b1 = c;
  //  } else if (h < 300) {
  //    r1 = x;
  //    g1 = 0;
  //    b1 = c;
  //  } else {
  //    r1 = c;
  //    g1 = 0;
  //    b1 = x;
  //  }

  //  r = (r1 + m);
  //  g = (g1 + m);
  //  b = (b1 + m);
  //};

  // int hue = 0;
  // std::random_device
  //     rd; // Obtain a random seed from the hardware (if available)
  // std::mt19937 gen(rd()); // Seed Mersenne Twister engine with rd()

  // std::uniform_int_distribution<> dist(0, 255); // Distribution range [1, 10]
  // std::vector<Vector2> points;
  // win->setRenderLoop([&canvas, &x, &y, &vx, &vy, &hue, &hsvToRGB, &dist,
  // &gen,
  //                     &points](Window &win) mutable {
  // win->setRenderLoop([&canvas](Window &win) mutable {
  // float r, g, b;
  // hsvToRGB(hue % 360, 1.0f, 1.0f, r, g, b);
  //  canvas.beginDraw();
  // canvas.clear();
  //  canvas.background({20, 100, 255});

  //  canvas.endDraw();
  // canvas.repaint();
  // Sleep(16); // Uncomment for manual throttling if needed
  //});

  // canvas.arc({146, 191}, {40, 40},
  //  {-265 * 3.1415926f / 180.0f, -48 * 3.1415926f / 180.0f});

  /*canvas.background({20, 100, 255});
  canvas.triangle({100, 100}, {200, 100}, {150, 200});
  canvas.line({0, 0}, {100, 100});
  canvas.arc({50, 50}, {50, 50}, {0, std::numbers::pi});*/
  win->setVisible(true);
  if (blocking) {
    return win->run(onExecute);
  } else {
    return 0;
  }
}
int reflect::cliMain(const uint8_t argc, const std::vector<std::string> argv) {

  reflect::CrashConfig config = {};
  config.crashType = CRASH_INVOKE | CRASH_SEGFAULT | CRASH_UNHANDLED_EXCEPTION;

  reflect::CrashHandler *ch = new reflect::CrashHandler(config);
  reflect::CrashManager::setActiveCrashHandler(ch);

  /*LogBuffer logbuf;

  std::cout.rdbuf(&logbuf);*/
  try {

    if (argc <= 1) {

      reflectError("error: no files specified. please pass an argument to "
                   "a valid file");
      reflectError("error code: ERROR_NO_FILES_SPECIFIED");
      return REFLECT_ERROR_NO_FILES_SPECIFIED;
    } else {
      size_t size = 2 * 1024 * 1024;
      for (std::string arg : argv) {
        if (arg == "--help" || arg == "-h") {
          reflectPrintln("usage: " + argv[0] + " <file/url> [--verbose]");
          reflectPrintln("options:");
          reflectPrintln("  --help, -h ");
          reflectPrintln("    shows the list of arguments");
          reflectPrintln("  --verbose");
          reflectPrintln("    enables verbose logging");
          reflectPrintln("  --version, -v");
          reflectPrintln("    shows the version");
          reflectPrintln("  --heap=<size>, -H<size>");
          reflectPrintln("    sets the heap size. must be a valid");
          reflectPrintln("    number. default is 64MB.");
          reflectPrintln("  --ignore-version-compat, -ivc");
          reflectPrintln("    ignores version compatibility check");
          reflectPrintln("    (use at your own risk)");
          return 0;
        } else if (arg == "--verbose") {
          reflectDebugEnabled(true);
          reflectInfo("verbose enabled, debug logs shown");
        } else if (arg == "--version" || arg == "-v") {
          reflectPrintln(APP_NAME + std::string(" ") + APP_VERSION);
          return 0;
        } else if (arg.starts_with("--heap=")) {
          std::string heapSize = arg.substr(arg.find("=") + 1);
          if (heapSize.empty()) {
            reflectError("heap size not specified");
            return 1;
          }
          // MB, KB, etc.

          std::string abbr = heapSize.substr(heapSize.length() - 2);
          if (abbr == "mb" || abbr == "MB") {
            heapSize = heapSize.substr(0, heapSize.length() - 1);
            std::string::size_type sz;
            size = std::stoul(heapSize, &sz);
            if (size > 0) {
              size *= 1024 * 1024;
            }
          } else if (abbr == "gb" || abbr == "GB") {
            heapSize = heapSize.substr(0, heapSize.length() - 1);
            std::string::size_type sz;
            size = std::stoul(heapSize, &sz);
            if (size > 0) {
              size *= 1024 * 1024 * 1024;
            }
          }
          // set the heap size

        } else if (arg.starts_with("-H")) {

          std::string heapSize = arg.substr(arg.find("-H") + 2);
          if (heapSize.empty()) {
            reflectError("heap size not specified");
            return 1;
          }

          std::string abbr = heapSize.substr(heapSize.length() - 2);

          if (abbr == "mb" || abbr == "MB") {
            heapSize = heapSize.substr(0, heapSize.length() - 1);
            std::string::size_type sz;
            size = std::stoul(heapSize, &sz);
            if (size > 0) {
              size *= 1024 * 1024;
            }
          } else if (abbr == "gb" || abbr == "GB") {
            heapSize = heapSize.substr(0, heapSize.length() - 1);
            std::string::size_type sz;
            size = std::stoul(heapSize, &sz);
            if (size > 0) {
              size *= 1024 * 1024 * 1024;
            }
          }
          // set the heap size
          initializeHeapPool(size);
        }
      }

      reflectInfo("launching executable...");
      initializeHeapPool(size);

      return reflect::invokeExecutable(argv[1]);
    }
  } catch (std::exception e) {
    ch->invokeUnhandledExceptionCrash(e);
    return 1;
  }
}

#elif __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>

#include <vector>

using namespace reflect::console;
using namespace reflect::utilities;
namespace fs = std::filesystem;

int reflect::invokeExecutable(std::string xmlFile) {
  tinyxml2::XMLDocument doc;
  // Load the XML file
  reflectInfo("Platform: " + GetOSPlatformAndVersion(), "CLIMain");
  if (!fs::exists(xmlFile)) {
    reflectError("file not found!");
    return reflectError_file_not_found;
  }
  reflectDebug("loading XML file...");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  if (eResult != tinyxml2::XML_SUCCESS) {
    reflectError(
        string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return reflectError_generic_xml_reflectError;
  }

  // Get the root element
  reflectDebug("getting root...");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  if (root == nullptr) {
    reflectError("no root element found.");
    return reflectError_xml_no_root;
  }
  reflectDebug("file is compatible with app?");
  const uint32_t version = stoi(root->Attribute("version"));
  if (version > app_internal_version) {
    reflectError("file not compatible! please upgrade to a newer version.");
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    reflectError("no window element found.");
    return reflectError_xml_no_window;
  }

#ifdef _WIN32
  reflectDebug("getting HINSTANCE...");
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  reflectDebug("creating window...");
  Window win(hInstance);
#elif __linux__
  Window win(XOpenDisplay(NULL));
#endif

  reflectDebug("window title: " + std::string(winXml->Attribute("title")),
               "invokeExecutable");
  win.SetTitle(winXml->Attribute("title"));
  reflectDebug("resizing window...");
  win.SetSize(Vector2(stoi(winXml->Attribute("width")),
                      stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  reflectDebug("setting window background color...");
  if (bgColor == "systemDefault") {
    win.SetColor(255, 255, 255);
  } else {
    if (bgColor.at(0) == '#' && bgColor.length() == 7) {

      const std::string hex = bgColor.substr(1);

      const uint8_t r = stoi(hex.substr(0, 2), nullptr, 16);
      const uint8_t g = stoi(hex.substr(2, 2), nullptr, 16);
      const uint8_t b = stoi(hex.substr(4, 2), nullptr, 16);

      win.SetColor(r, g, b);
    } else {
      reflectError("invalid hex color representation");
      return reflectError_hex_color_malformed;
    }
  }
  reflectDebug("parsing labels...");
  for (tinyxml2::XMLElement *label = winXml->FirstChildElement("label");
       label != nullptr; label = label->NextSiblingElement("label")) {
    // Access attributes
    std::string contents = label->GetText();
    Vector2 position(stoi(label->Attribute("x")), stoi(label->Attribute("y")));
    Label *labelComp = new Label(contents, position);
    labelComp->SetFont(label->Attribute("font"));
    labelComp->SetFontSize(stoi(label->Attribute("fontSize")));

    win.Add(labelComp);
  }
  reflectDebug("entering main loop...");
  win.SetVisible(true);

  return win.Run();
}
int reflect::CLIMain(const uint8_t argc, const vector<string> argv) {
  println(app_name + string(" ") + app_version);
  if (argv.size() == 1) {
    reflectError(
        "reflectError: no files specified. please pass an argument to a "
        "valid file");
    reflectError("reflectError code: ERROR_NO_FILES_SPECIFIED");
    return reflectError_no_files_specified;
  } else {
    reflectInfo("launching executable...", "CLIMain");
    return reflect::invokeExecutable(argv[1]);
  }
}
#endif
// Main entrypoint
