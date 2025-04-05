#ifdef CTOAST_LUA
#include "LuaAPI.h"
#include "Console.h"
#include "ui/Components.h"
#include "ui/TextComponent.h"
#include <fstream>
#include <vector>
using namespace CinnamonToast;
namespace {
bool luaReadWrite = false;
}
int LuaAPI::luaInfo(lua_State *L) {
  const char *msg = lua_tostring(L, 1);
  Console::info(msg);
  return 0;
}
int LuaAPI::luaWarn(lua_State *L) {
  const char *msg = lua_tostring(L, 1);
  Console::warn(msg);
  return 0;
}
int LuaAPI::luaCError(lua_State *L) {
  const char *msg = lua_tostring(L, 1);
  Console::error(msg);
  return 0;
}
int LuaAPI::luaDebug(lua_State *L) {
  const char *msg = lua_tostring(L, 1);
  Console::debug(msg);
  return 0;
}
int LuaAPI::luaGetComponentText(lua_State *L) {
  // Retrieve the upvalue (Component pointer)
  TextComponent *comp = (TextComponent *)lua_touserdata(L, lua_upvalueindex(1));

  if (!comp) {
    lua_pushnil(L);
    return 1;
  }

  // Call GetText() and return the result as a string
  std::string text = comp->getText();
  lua_pushstring(L, text.c_str());

  return 1;
}
int LuaAPI::luaReadFile(lua_State *L) {
  if (!luaReadWrite) {
    lua_pushnil(L);
    return 1;
  }
  const char *path = lua_tostring(L, 1);

  std::ifstream file(path, std::ios::binary);
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  file.close();
  lua_pushstring(L, content.c_str());
}
int LuaAPI::luaWriteFile(lua_State *L) {
  if (!luaReadWrite) {
    lua_pushnil(L);
    return 1;
  }
  const char *path = lua_tostring(L, 1);
  const char *content = lua_tostring(L, 2);
  std::ofstream file(path, std::ios::binary);
  file << content;
  file.close();
  lua_pushboolean(L, true);
  return 1;
}
int LuaAPI::luaRequestPermission(lua_State *L) {
  std::string permission = std::string(lua_tostring(L, 1));
  if (permission == "PERMISSION_FILESYSTEM") {
    int result = MessageBoxA(NULL,
                             "The app is requesting permission for read/write "
                             "operations. Are you sure?",
                             "Permission Request", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) {
      luaReadWrite = true;
      lua_pushboolean(L, true);
      return 1;
    } else {
      lua_pushboolean(L, false);
      return 1;
    }
  }
  return 0;
}
// table constructor for components
int LuaAPI::getComponentById(lua_State *L) {
  const char *id = lua_tostring(L, 1);
  Component *comp = Components::getComponentById(id);
  if (!comp)
    return 0; // If no component was found, return nil

  // Create a new Lua table to represent the object
  lua_newtable(L);

  // Push the component pointer as a lightuserdata and store it as an upvalue
  lua_pushlightuserdata(L, comp);
  // upcast and see if its a text component
  if (dynamic_cast<TextComponent *>(comp)) {

    lua_pushcclosure(L, luaGetComponentText, 1); // Closure with 1 upvalue
    lua_setfield(L, -2, "getText");
  }

  return 1; // Return the new table
}
void CinnamonToast::injectLuaApis(LuaInstance *self) {
  self->registerFunction("CinnamonToast", "getComponentById",
                         LuaAPI::getComponentById);
  self->registerFunction("CinnamonToast", "requestPermission",
                         LuaAPI::luaRequestPermission);
  self->registerFunction("info", LuaAPI::luaInfo);
  self->registerFunction("warn", LuaAPI::luaWarn);
  self->registerFunction("cerror", LuaAPI::luaCError);
  self->registerFunction("debug", LuaAPI::luaDebug);
  self->registerFunction("writeFile", LuaAPI::luaWriteFile);
  self->registerFunction("readFile", LuaAPI::luaReadFile);
};

#endif