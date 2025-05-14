#pragma once
#ifdef REFLECT_LUA
typedef unsigned char byte;
#include "../api/lua/ReflectLua.h"
#include "TypeDefinitions.h"
namespace reflect {
namespace LuaAPI {
REFLECT_API int getComponentById(lua_State *L);
REFLECT_API int luaGetComponentText(lua_State *L);
REFLECT_API int luaInfo(lua_State *L);
REFLECT_API int luaWarn(lua_State *L);
REFLECT_API int luaCError(lua_State *L);
REFLECT_API int luaReadFile(lua_State *L);
REFLECT_API int luaWriteFile(lua_State *L);
REFLECT_API int luaRequestPermission(lua_State *L);
REFLECT_API int luaDebug(lua_State *L);
} // namespace LuaAPI
REFLECT_API void injectLuaApis(LuaInstance *self);
} // namespace reflect
#endif
