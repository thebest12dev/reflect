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
#ifdef CTOAST_LUA

#include "../api/lua/CToastLua.h"

#endif

// our header files here
#include "CToastAPI.h"
#include "Console.h"
#include "CrashHandler.h"
#include "Definitions.h"
#include "Main.h"
#include "TypeDefinitions.h"
#include "Utilities.h"
// ui components
#include "ui/Button.h"
#include "ui/Colors.h"
#include "ui/Component.h"
#include "ui/Components.h"
#include "ui/Label.h"
#include "ui/MenuItem.h"
#include "ui/TextComponent.h"
#include "ui/Vector2.h"
#ifdef CTOAST_LUA
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
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory> // for std::unique_ptr
#include <string>
#include <vector>
// using namespaces and alias
using namespace CinnamonToast::Console;
using namespace CinnamonToast::Utilities;

namespace fs = std::filesystem;

namespace {

void onExecute(CinnamonToast::Window &win) { /*win.Close();*/ }
std::vector<void *> heapAllocations;
struct Cleaner {
  ~Cleaner() {
    for (void *ptr : heapAllocations) {
      CinnamonToast::getHeapPool()->deallocate(ptr, sizeof(ptr));
    }
  }
};
Cleaner cleaner;

} // namespace
/**
 * Invokes and loads a .xml file and also loads the specific libraries. It will
 * setup the GUI as well as registering APIs for the libraries to use.
 */
int ctoast invokeExecutable(std::string xmlFile) {
  // if lua enabled

  // load the xml file
  tinyxml2::XMLDocument doc;

  // platform
  ctoastInfo("platform: " + getOSPlatformAndVersion());

  // common control stuff
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_STANDARD_CLASSES;

  InitCommonControlsEx(&icex);

  // file doesnt exist
  if (!fs::exists(xmlFile)) {
    // ctoastError and return
    ctoastError("file not found!");
    return ERROR_FILE_NOT_FOUND;
  }

  // ctoastDebug print and load xml file
  ctoastDebug("loading XML file...");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  // ctoastError handling
  if (eResult != tinyxml2::XML_SUCCESS) {
    ctoastError(
        std::string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return CTOAST_ERROR_GENERIC_XML_ERROR;
  }

  // get the root element
  ctoastDebug("getting root...");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  // handling
  if (root == nullptr) {
    ctoastError("no root element found.");
    return CTOAST_ERROR_XML_NO_ROOT;
  }

  // checking compat
  ctoastDebug("file is compatible with app?");
  const uint32_t version = std::stoi(root->Attribute("version"));
  if (version > APP_INTERNAL_VERSION) {
    // handle and return
    ctoastError("file not compatible! please upgrade to a newer version.");
    return CTOAST_ERROR_XML_NOT_COMPATIBLE;
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    ctoastError("no window element found.");
    return CTOAST_ERROR_XML_NO_WINDOW;
  }
  tinyxml2::XMLElement *gctx = winXml->FirstChildElement("graphicsContext");
#ifdef _WIN32
  Window *win = nullptr;
  if (gctx == nullptr) {
    ctoastDebug("getting HINSTANCE...");
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    ctoastDebug("creating window...");
    // OpenGLContext ctx;
    win = new Window(hInstance);
  } else {
    ctoastDebug("graphics context found!");
    std::string ctx = gctx->Attribute("type");
    if (ctx == "opengl") {
      ctoastDebug("using custom OpenGL context...");
      ctoastWarn(
          "Since a custom graphics context is specified, UI controls (buttons, "
          "labels, etc.) will not render. This is only for if you want to "
          "develop things like games or simulations. (To disable this "
          "warning, set contextSetWarning to false in your XML file.");
      ctoastDebug("getting HINSTANCE...");
      HINSTANCE hInstance = GetModuleHandle(nullptr);
      ctoastDebug("creating window...");
      OpenGLContext ctx;
      win = new Window(hInstance, ctx);
    }
  }

#endif
  ctoastDebug("window title: " + std::string(winXml->Attribute("title")));
  win->setTitle(winXml->Attribute("title"));
  ctoastDebug("resizing window...");
  win->setSize(Vector2(std::stoi(winXml->Attribute("width")),
                       std::stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  ctoastDebug("setting window background color...");
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
      ctoastError("invalid hex color representation");
      return CTOAST_ERROR_HEX_COLOR_MALFORMED;
    }
  }
  ctoastDebug("parsing labels...");
  for (tinyxml2::XMLElement *label = winXml->FirstChildElement("label");
       label != nullptr; label = label->NextSiblingElement("label")) {
    // Access attributes
    std::string c = std::string(label->GetText());
    std::string *contents = &c;
    std::string id = label->Attribute("id");
    Vector2 position(std::stoi(label->Attribute("x")),
                     std::stoi(label->Attribute("y")));
    Label *labelComp = new Label(*contents, position);
    heapAllocations.push_back(labelComp);
    labelComp->setFont(label->Attribute("font"));
    labelComp->setFontSize(std::stoi(label->Attribute("fontSize")));
    win->add(*labelComp, id);
  }
  ctoastDebug("parsing progress bars...");
  for (tinyxml2::XMLElement *label = winXml->FirstChildElement("progressBar");
       label != nullptr; label = label->NextSiblingElement("progressBar")) {
    // Access attributes
    std::string id = label->Attribute("id");
    Vector2 position(std::stoi(label->Attribute("x")),
                     std::stoi(label->Attribute("y")));
    Vector2 size(std::stoi(label->Attribute("width")),
                 std::stoi(label->Attribute("height")));
    ProgressBar *pb = new ProgressBar();
    heapAllocations.push_back(pb);
    pb->setMininumValue(std::stof(label->Attribute("min")));
    pb->setMaximumValue(std::stof(label->Attribute("max")));
    pb->setValue(std::stoi(label->Attribute("value")));
    pb->setPosition(position);
    pb->setSize(size);
    win->add(*pb, id);
  }
  ctoastDebug("parsing buttons...");
  for (tinyxml2::XMLElement *button = winXml->FirstChildElement("button");
       button != nullptr; button = button->NextSiblingElement("button")) {
    std::string contents = button->GetText();
    std::string id = button->Attribute("id");
    Vector2 position(std::stoi(button->Attribute("x")),
                     std::stoi(button->Attribute("y")));
    Button *buttonComp = new Button(contents, position);
    heapAllocations.push_back(buttonComp);
    buttonComp->setFont(button->Attribute("font"));
    buttonComp->setFontSize(std::stoi(button->Attribute("fontSize")));
    win->add(*buttonComp, id);
  }
  ctoastDebug("loading libraries...");
#ifdef CTOAST_LUA
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
        ctoastError("cannot load shared library");
        return CTOAST_ERROR_CANNOT_LOAD_SHARED_LIBRARY;
      }
      ctoastDebug("loaded shared library!");

      // Get the address of the Add function
      CinnamonToast::SharedLibraryMain mainFunc =
          (CinnamonToast::SharedLibraryMain)GetProcAddress(hDll, "CToastMain");
      if (!mainFunc) {
        ctoastError("cannot find CToastMain function of library!");

        FreeLibrary(hDll); // Free the DLL
        return CTOAST_ERROR_CANNOT_LOAD_LIBRARY_FUNCTION;
      }
      CinnamonToast::CToastAPI ctoastApi = {};
      ctoastApi.addComponent = ExternalAPI::addComponent;
      ctoastApi.getComponentById = ExternalAPI::getComponentById;
      ctoastApi.getText = ExternalAPI::getComponentText;
      ctoastApi.setColor = ExternalAPI::setComponentColor;
      ctoastApi.setFont = ExternalAPI::setComponentFont;
      ctoastApi.setFontSize = ExternalAPI::setComponentFontSize;
      ctoastApi.setVisible = ExternalAPI::setComponentVisible;
      ctoastApi.setVisibleCommand = ExternalAPI::setComponentVisibleCommand;
      std::thread *thread = new std::thread(mainFunc, &ctoastApi);
    }
#ifdef CTOAST_LUA
    else if (std::string(sharedLib->Attribute("type")) == "lua") {
      ctoastDebug("loading lua file...");
      if (lua == nullptr) {
        ctoastDebug("initializing lua...");
        lua = new LuaInstance();
        heapAllocations.push_back(lua);
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

  ctoastDebug("entering main loop...");
  win->setVisible(true);

  return win->run(onExecute);
}
int ctoast cliMain(const uint8_t argc, const std::vector<std::string> argv) {

  CinnamonToast::CrashConfig config = {};
  config.crashType = CRASH_INVOKE | CRASH_SEGFAULT | CRASH_UNHANDLED_EXCEPTION;

  CinnamonToast::CrashHandler *ch = new CinnamonToast::CrashHandler(config);
  CinnamonToast::CrashManager::setActiveCrashHandler(ch);

  /*LogBuffer logbuf;

  std::cout.rdbuf(&logbuf);*/
  try {

    if (argc <= 1) {

      ctoastError("ctoastError: no files specified. please pass an argument to "
                  "a valid file");
      ctoastError("ctoastError code: ERROR_NO_FILES_SPECIFIED");
      return CTOAST_ERROR_NO_FILES_SPECIFIED;
    } else {
      size_t size = 64 * 1024 * 1024;
      for (std::string arg : argv) {
        if (arg == "--help" || arg == "-h") {
          ctoastPrintln("usage: " + argv[0] + " <file/url> [--verbose]");
          ctoastPrintln("options:");
          ctoastPrintln("  --help, -h ");
          ctoastPrintln("    shows the list of arguments");
          ctoastPrintln("  --verbose");
          ctoastPrintln("    enables verbose logging");
          ctoastPrintln("  --version, -v");
          ctoastPrintln("    shows the version");
          ctoastPrintln("  --heap=<size>, -H<size>");
          ctoastPrintln("    sets the heap size. must be a valid");
          ctoastPrintln("    number. default is 64MB.");
          ctoastPrintln("  --ignore-version-compat, -ivc");
          ctoastPrintln("    ignores version compatibility check");
          ctoastPrintln("    (use at your own risk)");
          return 0;
        } else if (arg == "--verbose") {
          ctoastDebugEnabled(true);
          ctoastInfo("verbose enabled, debug logs shown");
        } else if (arg == "--version" || arg == "-v") {
          ctoastPrintln(APP_NAME + std::string(" ") + APP_VERSION);
          return 0;
        } else if (arg.starts_with("--heap=")) {
          std::string heapSize = arg.substr(arg.find("=") + 1);
          if (heapSize.empty()) {
            ctoastError("heap size not specified");
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
            ctoastError("heap size not specified");
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

      ctoastInfo("launching executable...");
      initializeHeapPool(size);
      return CinnamonToast::invokeExecutable(argv[1]);
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

using namespace CinnamonToast::Console;
using namespace CinnamonToast::Utilities;
namespace fs = std::filesystem;

int ctoast invokeExecutable(std::string xmlFile) {
  tinyxml2::XMLDocument doc;
  // Load the XML file
  ctoastInfo("Platform: " + GetOSPlatformAndVersion(), "CLIMain");
  if (!fs::exists(xmlFile)) {
    ctoastError("file not found!");
    return ctoastError_file_not_found;
  }
  ctoastDebug("loading XML file...");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  if (eResult != tinyxml2::XML_SUCCESS) {
    ctoastError(string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return ctoastError_generic_xml_ctoastError;
  }

  // Get the root element
  ctoastDebug("getting root...");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  if (root == nullptr) {
    ctoastError("no root element found.");
    return ctoastError_xml_no_root;
  }
  ctoastDebug("file is compatible with app?");
  const uint32_t version = stoi(root->Attribute("version"));
  if (version > app_internal_version) {
    ctoastError("file not compatible! please upgrade to a newer version.");
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    ctoastError("no window element found.");
    return ctoastError_xml_no_window;
  }

#ifdef _WIN32
  ctoastDebug("getting HINSTANCE...");
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  ctoastDebug("creating window...");
  Window win(hInstance);
#elif __linux__
  Window win(XOpenDisplay(NULL));
#endif

  ctoastDebug("window title: " + std::string(winXml->Attribute("title")),
              "invokeExecutable");
  win.SetTitle(winXml->Attribute("title"));
  ctoastDebug("resizing window...");
  win.SetSize(Vector2(stoi(winXml->Attribute("width")),
                      stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  ctoastDebug("setting window background color...");
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
      ctoastError("invalid hex color representation");
      return ctoastError_hex_color_malformed;
    }
  }
  ctoastDebug("parsing labels...");
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
  ctoastDebug("entering main loop...");
  win.SetVisible(true);

  return win.Run();
}
int ctoast CLIMain(const uint8_t argc, const vector<string> argv) {
  println(app_name + string(" ") + app_version);
  if (argv.size() == 1) {
    ctoastError("ctoastError: no files specified. please pass an argument to a "
                "valid file");
    ctoastError("ctoastError code: ERROR_NO_FILES_SPECIFIED");
    return ctoastError_no_files_specified;
  } else {
    ctoastInfo("launching executable...", "CLIMain");
    return CinnamonToast::invokeExecutable(argv[1]);
  }
}
#endif
// Main entrypoint
