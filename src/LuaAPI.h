#pragma once
#ifdef CTOAST_LUA
	typedef unsigned char byte;
	#include "TypeDefinitions.h"
	#include "../lua/CToastLua.h"
	namespace CinnamonToast {
		namespace LuaAPI {
			CTOAST_API int GetComponentById(lua_State* L);
			CTOAST_API int LuaGetComponentText(lua_State* L);
			CTOAST_API int LuaInfo(lua_State* L);
			CTOAST_API int LuaWarn(lua_State* L);
			CTOAST_API int LuaCError(lua_State* L);
			CTOAST_API int LuaReadFile(lua_State* L);
			CTOAST_API int LuaWriteFile(lua_State* L);
			CTOAST_API int LuaRequestPermission(lua_State* L);
			CTOAST_API int LuaDebug(lua_State* L);
		}
		CTOAST_API void InjectLuaApis(LuaInstance* self);
	}
#endif


