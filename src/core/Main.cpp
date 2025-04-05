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
extern "C" {
#include "../api/lua/CToastLua.h"
}
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
#include "ui/MenuBar.h"
#include "ui/Notification.h"
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
  info("platform: " + getOSPlatformAndVersion(), "cliMain");

  // common control stuff
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_STANDARD_CLASSES;

  InitCommonControlsEx(&icex);

  // file doesnt exist
  if (!fs::exists(xmlFile)) {
    // error and return
    Console::error("file not found!");
    return ERROR_FILE_NOT_FOUND;
  }

  // debug print and load xml file
  Console::debug("loading XML file...", "invokeExecutable");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  // error handling
  if (eResult != tinyxml2::XML_SUCCESS) {
    Console::error(
        std::string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return CTOAST_ERROR_GENERIC_XML_ERROR;
  }

  // get the root element
  Console::debug("getting root...", "invokeExecutable");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  // handling
  if (root == nullptr) {
    error("no root element found.");
    return CTOAST_ERROR_XML_NO_ROOT;
  }

  // checking compat
  Console::debug("file is compatible with app?", "invokeExecutable");
  const uint32_t version = std::stoi(root->Attribute("version"));
  if (version > APP_INTERNAL_VERSION) {
    // handle and return
    error("file not compatible! please upgrade to a newer version.");
    return CTOAST_ERROR_XML_NOT_COMPATIBLE;
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    error("no window element found.");
    return CTOAST_ERROR_XML_NO_WINDOW;
  }

#ifdef _WIN32
  debug("getting HINSTANCE...", "invokeExecutable");
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  debug("creating window...", "invokeExecutable");
  OpenGLContext ctx;
  Window win(hInstance, ctx);

#endif

  Console::debug("window title: " + std::string(winXml->Attribute("title")),
                 "invokeExecutable");
  win.setTitle(winXml->Attribute("title"));
  Console::debug("resizing window...", "invokeExecutable");
  win.setSize(Vector2(std::stoi(winXml->Attribute("width")),
                      std::stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  Console::debug("setting window background color...", "invokeExecutable");
  if (bgColor == "systemDefault") {
    win.setColor(255, 255, 255);
  } else {
    if (bgColor.at(0) == '#' && bgColor.length() == 7) {

      const std::string hex = bgColor.substr(1);

      const uint8_t r = stoi(hex.substr(0, 2), nullptr, 16);
      const uint8_t g = stoi(hex.substr(2, 2), nullptr, 16);
      const uint8_t b = stoi(hex.substr(4, 2), nullptr, 16);
      const Color3 color = {r, g, b};
      win.setColor(color);
    } else {
      error("invalid hex color representation");
      return CTOAST_ERROR_HEX_COLOR_MALFORMED;
    }
  }
  Console::debug("parsing labels...", "invokeExecutable");
  for (tinyxml2::XMLElement *label = winXml->FirstChildElement("label");
       label != nullptr; label = label->NextSiblingElement("label")) {
    // Access attributes
    std::string c = std::string(label->GetText());
    std::string *contents = &c;
    std::string id = label->Attribute("id");
    Vector2 position(std::stoi(label->Attribute("x")),
                     std::stoi(label->Attribute("y")));
    Label *labelComp = new Label(*contents, position);
    labelComp->setFont(label->Attribute("font"));
    labelComp->setFontSize(std::stoi(label->Attribute("fontSize")));
    win.add(*labelComp, id);
  }
  Console::debug("parsing buttons...", "invokeExecutable");
  for (tinyxml2::XMLElement *button = winXml->FirstChildElement("button");
       button != nullptr; button = button->NextSiblingElement("button")) {
    std::string contents = button->GetText();
    std::string id = button->Attribute("id");
    Vector2 position(std::stoi(button->Attribute("x")),
                     std::stoi(button->Attribute("y")));
    Button buttonComp(contents, position);
    buttonComp.setFont(button->Attribute("font"));
    buttonComp.setFontSize(std::stoi(button->Attribute("fontSize")));
    win.add(buttonComp, id);
  }
  Console::debug("loading libraries...", "invokeExecutable");
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
        Console::error("cannot load shared library", "invokeExecutable");
        return CTOAST_ERROR_CANNOT_LOAD_SHARED_LIBRARY;
      }
      Console::debug("loaded shared library!", "invokeExecutable");

      // Get the address of the Add function
      CinnamonToast::SharedLibraryMain mainFunc =
          (CinnamonToast::SharedLibraryMain)GetProcAddress(hDll, "CToastMain");
      if (!mainFunc) {
        Console::error("cannot find CToastMain function of library!",
                       "invokeExecutable");

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
      Console::debug("loading lua file...", "invokeExecutable");
      if (lua == nullptr) {
        Console::debug("initializing lua...", "invokeExecutable");
        lua = new LuaInstance();
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
    for (tinyxml2::XMLElement *menuItem = menu->FirstChildElement("menuItem");
         menuItem != nullptr; menuItem = menu->NextSiblingElement("menuItem")) {
      MenuItem *item = new MenuItem(menuItem->GetText());
      menuBar->add(*item);
    }
    win.add(*menuBar);
  }

  Console::debug("entering main loop...", "invokeExecutable");
  win.setVisible(true);

  return win.run(onExecute);
}
int ctoast cliMain(const uint8_t argc, const std::vector<std::string> argv) {
  CinnamonToast::CrashConfig config = {};
  config.crashType = CRASH_INVOKE | CRASH_SEGFAULT | CRASH_UNHANDLED_EXCEPTION;

  CinnamonToast::CrashHandler *ch = new CinnamonToast::CrashHandler(config);
  CinnamonToast::CrashManager::setActiveCrashHandler(ch);

  try {
    println(APP_NAME + std::string(" ") + APP_VERSION);
    if (argc == 1) {
      Console::error(
          "error: no files specified. please pass an argument to a valid file");
      Console::error("error code: ERROR_NO_FILES_SPECIFIED");
      return CTOAST_ERROR_NO_FILES_SPECIFIED;
    } else {
      Console::info("launching executable...", "cliMain");
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
  info("Platform: " + GetOSPlatformAndVersion(), "CLIMain");
  if (!fs::exists(xmlFile)) {
    error("file not found!");
    return error_file_not_found;
  }
  debug("loading XML file...", "invokeExecutable");
  tinyxml2::XMLError eResult = doc.LoadFile(xmlFile.c_str());

  if (eResult != tinyxml2::XML_SUCCESS) {
    error(string("cannot load XML file: " + std::string(doc.ErrorStr())));
    return error_generic_xml_error;
  }

  // Get the root element
  debug("getting root...", "invokeExecutable");
  tinyxml2::XMLElement *root = doc.FirstChildElement("root");
  if (root == nullptr) {
    error("no root element found.");
    return error_xml_no_root;
  }
  debug("file is compatible with app?", "invokeExecutable");
  const uint32_t version = stoi(root->Attribute("version"));
  if (version > app_internal_version) {
    error("file not compatible! please upgrade to a newer version.");
  }
  tinyxml2::XMLElement *winXml = root->FirstChildElement("window");
  if (winXml == nullptr) {
    error("no window element found.");
    return error_xml_no_window;
  }

#ifdef _WIN32
  debug("getting HINSTANCE...", "invokeExecutable");
  HINSTANCE hInstance = GetModuleHandle(nullptr);
  debug("creating window...", "invokeExecutable");
  Window win(hInstance);
#elif __linux__
  Window win(XOpenDisplay(NULL));
#endif

  debug("window title: " + std::string(winXml->Attribute("title")),
        "invokeExecutable");
  win.SetTitle(winXml->Attribute("title"));
  debug("resizing window...", "invokeExecutable");
  win.SetSize(Vector2(stoi(winXml->Attribute("width")),
                      stoi(winXml->Attribute("height"))));
  std::string bgColor = winXml->Attribute("bgColor");
  debug("setting window background color...", "invokeExecutable");
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
      error("invalid hex color representation");
      return error_hex_color_malformed;
    }
  }
  debug("parsing labels...", "invokeExecutable");
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
  debug("entering main loop...", "invokeExecutable");
  win.SetVisible(true);

  return win.Run();
}
int ctoast CLIMain(const uint8_t argc, const vector<string> argv) {
  println(app_name + string(" ") + app_version);
  if (argv.size() == 1) {
    error("error: no files specified. please pass an argument to a valid file");
    error("error code: ERROR_NO_FILES_SPECIFIED");
    return error_no_files_specified;
  } else {
    info("launching executable...", "CLIMain");
    return CinnamonToast::invokeExecutable(argv[1]);
  }
}
#endif
// Main entrypoint

#ifndef CTOAST_API_library
int main(const int argc, const char *argv[]) {
  return CinnamonToast::cliMain(
      argc, CinnamonToast::Utilities::cstrArrToVector(argv));
}
#endif