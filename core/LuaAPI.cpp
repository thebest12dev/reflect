#ifdef CTOAST_LUA
	#include "LuaAPI.h"
	#include <vector>
	#include <fstream>
	#include "ui/Components.h"
	#include "ui/TextComponent.h"
	#include "Console.h"
	using namespace CinnamonToast;
	namespace {
		bool luaReadWrite = false;
	}
	int LuaAPI::LuaInfo(lua_State* L) {
		const char* msg = lua_tostring(L, 1);
		Console::info(msg);
		return 0;
	}
	int LuaAPI::LuaWarn(lua_State* L) {
		const char* msg = lua_tostring(L, 1);
		Console::warn(msg);
		return 0;
	}
	int LuaAPI::LuaCError(lua_State* L) {
		const char* msg = lua_tostring(L, 1);
		Console::error(msg);
		return 0;
	}
	int LuaAPI::LuaDebug(lua_State* L) {
		const char* msg = lua_tostring(L, 1);
		Console::debug(msg);
		return 0;
	}
	int LuaAPI::LuaGetComponentText(lua_State* L) {
		// Retrieve the upvalue (Component pointer)
		TextComponent* comp = (TextComponent*)lua_touserdata(L, lua_upvalueindex(1));

		if (!comp) {
			lua_pushnil(L);
			return 1;
		}

		// Call GetText() and return the result as a string
		std::string text = comp->GetText();
		lua_pushstring(L, text.c_str());

		return 1;
	}
	int LuaAPI::LuaReadFile(lua_State* L) {
		if (!luaReadWrite) {
			lua_pushnil(L);
			return 1;
		}
		const char* path = lua_tostring(L, 1);
		
		std::ifstream file(path, ios::binary);
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		lua_pushstring(L, content.c_str());

		
	}
	int LuaAPI::LuaWriteFile(lua_State* L) {
		if (!luaReadWrite) {
			lua_pushnil(L);
			return 1;
		}
		const char* path = lua_tostring(L, 1);
		const char* content = lua_tostring(L, 2);
		std::ofstream file(path, ios::binary);
		file << content;
		file.close();
		lua_pushboolean(L, true);
		return 1;
	}
	int LuaAPI::LuaRequestPermission(lua_State* L) {
		string permission = string(lua_tostring(L, 1));
		if (permission == "PERMISSION_FILESYSTEM") {
			int result = MessageBoxA(NULL, "The app is requesting permission for read/write operations. Are you sure?", "Permission Request", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) {
				luaReadWrite = true;
				lua_pushboolean(L, true);
				return 1;
			}
			else {
				lua_pushboolean(L, false);
				return 1;
			}
		}
		return 0;
	}
	// table constructor for components
	int LuaAPI::GetComponentById(lua_State* L) {
		const char* id = lua_tostring(L, 1);
		Component* comp = Components::GetComponentById(id);
		if (!comp) return 0;  // If no component was found, return nil

		// Create a new Lua table to represent the object
		lua_newtable(L);

		// Push the component pointer as a lightuserdata and store it as an upvalue
		lua_pushlightuserdata(L, comp);
		// upcast and see if its a text component
		if (dynamic_cast<TextComponent*>(comp)) {
			
			lua_pushcclosure(L, LuaGetComponentText, 1); // Closure with 1 upvalue
			lua_setfield(L, -2, "getText");
		}

		return 1; // Return the new table
	}
	void CinnamonToast::InjectLuaApis(LuaInstance* self) {
		self->RegisterFunction("CinnamonToast", "getComponentById", LuaAPI::GetComponentById);
		self->RegisterFunction("CinnamonToast", "requestPermission", LuaAPI::LuaRequestPermission);
		self->RegisterFunction("info", LuaAPI::LuaInfo);
		self->RegisterFunction("warn", LuaAPI::LuaWarn);
		self->RegisterFunction("cerror", LuaAPI::LuaCError);
		self->RegisterFunction("debug", LuaAPI::LuaDebug);
		self->RegisterFunction("writeFile", LuaAPI::LuaWriteFile);
		self->RegisterFunction("readFile", LuaAPI::LuaReadFile);
	};

#endif