#include "CToastLua.h"

namespace CinnamonToast {

// Constructor
LuaInstance::LuaInstance() {
  L = luaL_newstate();
  luaopen_base(L);   // Opens basic functions (_G)
  luaopen_table(L);  // Opens table library
  luaopen_string(L); // Opens string library
  luaopen_math(L);   // Opens math library
  lua_pushnil(L);
  lua_setglobal(L, "dofile"); // Remove dofile (prevents running files)
  lua_pushnil(L);
  lua_setglobal(L, "loadfile"); // Remove loadfile (prevents loading scripts)
  lua_pushnil(L);
  lua_setglobal(L, "load"); // Remove load (prevents running arbitrary Lua code)
  lua_pushnil(L);
  lua_setglobal(
      L, "require"); // Remove require (prevents loading additional modules)
}

// Execute Lua code
void LuaInstance::execute(std::string code) { luaL_dostring(L, code.c_str()); }

// Execute Lua file
void LuaInstance::executeFile(std::string filePath) {
  luaL_dofile(L, filePath.c_str());
}

// Register a Lua function
void LuaInstance::registerFunction(std::string name, lua_CFunction func) {
  lua_register(L, name.c_str(), func);
}

// Set a value in Lua
template <LuaType T> void LuaInstance::setValue(std::string name, T value) {
  if constexpr (ValidLuaType<T>) {
    if constexpr (std::is_same_v<T, int>) {
      lua_pushinteger(L, value);
    } else if constexpr (std::is_same_v<T, double>) {
      lua_pushnumber(L, value);
    } else if constexpr (std::is_same_v<T, std::string>) {
      lua_pushstring(L, value.c_str());
    }
  } else if constexpr (ValidLuaVector<T>) {
    lua_newtable(L);
    for (int i = 0; i < value.size(); i++) {
      setValue(i + 1, value[i]);
    }
  }
  lua_setglobal(L, name.c_str());
}

// Set a value in a Lua table
template <LuaType T>
void LuaInstance::setValue(std::string hierarchy, std::string key, T value) {
  std::vector<std::string> tables;
  std::string currentTable = "";
  for (int i = 0; i < hierarchy.size(); i++) {
    if (hierarchy[i] == '.') {
      tables.push_back(currentTable);
      currentTable = "";
    } else {
      currentTable += hierarchy[i];
    }
  }
  tables.push_back(currentTable);
  lua_getglobal(L, tables[0].c_str());
  for (int i = 1; i < tables.size(); i++) {
    lua_pushstring(L, tables[i].c_str());
    lua_gettable(L, -2);
  }
  if constexpr (ValidLuaType<T>) {
    if constexpr (std::is_same_v<T, int>) {
      lua_pushinteger(L, value);
    } else if constexpr (std::is_same_v<T, double>) {
      lua_pushnumber(L, value);
    } else if constexpr (std::is_same_v<T, std::string>) {
      lua_pushstring(L, value.c_str());
    }
  } else if constexpr (ValidLuaVector<T>) {
    lua_newtable(L);
    for (int i = 0; i < value.size(); i++) {
      setValue(i + 1, value[i]);
    }
  }
  lua_setfield(L, -2, key.c_str());
}

// Set a string value in a Lua table
void LuaInstance::setValue(std::string hierarchy, std::string key,
                           std::string value) {
  std::vector<std::string> tables;
  std::string currentTable = "";
  for (int i = 0; i < hierarchy.size(); i++) {
    if (hierarchy[i] == '.') {
      tables.push_back(currentTable);
      currentTable = "";
    } else {
      currentTable += hierarchy[i];
    }
  }
  tables.push_back(currentTable);
  lua_getglobal(L, tables[0].c_str());
  for (int i = 1; i < tables.size(); i++) {
    lua_pushstring(L, tables[i].c_str());
    lua_gettable(L, -2);
  }
  lua_pushstring(L, value.c_str());
  lua_setfield(L, -2, key.c_str());
}

// Create a Lua table
void LuaInstance::createTable(std::string name) {
  lua_newtable(L);
  lua_setglobal(L, name.c_str());
}

// Create a Lua table with hierarchy
void LuaInstance::createTable(std::string hierarchy, std::string name) {
  std::vector<std::string> tables;
  std::string currentTable = "";
  for (int i = 0; i < hierarchy.size(); i++) {
    if (hierarchy[i] == '.') {
      tables.push_back(currentTable);
      currentTable = "";
    } else {
      currentTable += hierarchy[i];
    }
  }
  tables.push_back(currentTable);
  lua_getglobal(L, tables[0].c_str());
  for (int i = 1; i < tables.size(); i++) {
    lua_pushstring(L, tables[i].c_str());
    lua_gettable(L, -2);
  }
  lua_newtable(L);
  lua_setglobal(L, name.c_str());
}

// Register a function in a Lua table
void LuaInstance::registerFunction(std::string tableName, std::string name,
                                   lua_CFunction func) {
  lua_getglobal(L, tableName.c_str());
  lua_pushcfunction(L, func);
  lua_setfield(L, -2, name.c_str());
}

// Initialize Lua APIs
void LuaInstance::initializeLuaApis(
    void (*customInitializer)(LuaInstance *self)) {
  createTable("CinnamonToast");
  if (customInitializer != nullptr) {
    customInitializer(this);
  }
}

} // namespace CinnamonToast
