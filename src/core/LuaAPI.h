#pragma once
#ifdef CTOAST_LUA
typedef unsigned char byte;
#include "../api/lua/CToastLua.h"
#include "TypeDefinitions.h"
namespace CinnamonToast {
namespace LuaAPI {
CTOAST_API int getComponentById(lua_State *L);
CTOAST_API int luaGetComponentText(lua_State *L);
CTOAST_API int luaInfo(lua_State *L);
CTOAST_API int luaWarn(lua_State *L);
CTOAST_API int luaCError(lua_State *L);
CTOAST_API int luaReadFile(lua_State *L);
CTOAST_API int luaWriteFile(lua_State *L);
CTOAST_API int luaRequestPermission(lua_State *L);
CTOAST_API int luaDebug(lua_State *L);
} // namespace LuaAPI
CTOAST_API void injectLuaApis(LuaInstance *self);
} // namespace CinnamonToast
#endif
