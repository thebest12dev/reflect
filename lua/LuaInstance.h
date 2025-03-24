#pragma once
/**
* Copyright (c) 2025 thebest12lines
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../lib/luajit/lua.hpp"
#include "TypeDefinitions.h"
#include <string>
#include "Console.h"
#include <vector>
#include <type_traits>
#include <concepts>

#define CTOAST_LUA_API "CinnamonToast"

namespace CinnamonToast {
	template<typename T>
	concept ValidLuaType = std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::string>;
	template<typename T>
	struct IsValidLuaTypeOrVectorTrait : std::false_type {};

	template<typename T>
	struct IsValidLuaTypeOrVectorTrait<std::vector<T>> : std::conditional_t<
		ValidLuaType<T> || IsValidLuaTypeOrVectorTrait<T>::value,
		std::true_type,
		std::false_type
	> {
	};

	template<typename T>
	constexpr bool IsValidLuaTypeOrVector = IsValidLuaTypeOrVectorTrait<T>::value;

	template<typename T>
	concept ValidLuaVector = requires(T t) {
		typename T::value_type;
		requires IsValidLuaTypeOrVector<typename T::value_type>;
	};

	template<typename T>
	concept LuaType = ValidLuaType<T> || ValidLuaVector<T>;

	class LuaInstance {
		private:
			lua_State* L;
			template<LuaType T>
			CTOAST_API void SetValue(int index, T value) {
				// push to stack
				if constexpr (std::is_same_v<T, int>) {
					lua_pushinteger(L, value);
				}
				else if constexpr (std::is_same_v<T, double>) {
					lua_pushnumber(L, value);
				}
				else if constexpr (std::is_same_v<T, std::string>) {
					lua_pushstring(L, value.c_str());
				}
				else if constexpr (ValidLuaVector<T>) {
					lua_newtable(L);
					for (int i = 0; i < value.size(); i++) {
						if constexpr (ValidLuaType<typename T::value_type>) {
							SetValue(i + 1, value[i]);
						}
						else if constexpr (ValidLuaVector<typename T::value_type>) {
							SetValue(i + 1, value[i]);
						}
					}
				}
			}
		public:
			CTOAST_API void Execute(std::string code) {
				luaL_dostring(L, code.c_str());
			};
			CTOAST_API void ExecuteFile(std::string filePath) {
				luaL_dofile(L, filePath.c_str());
			};
			CTOAST_API void RegisterFunction(std::string name, lua_CFunction func) {
				lua_register(L, name.c_str(), func);
			};

			template<LuaType T>
			CTOAST_API void SetValue(std::string name, T value) {
				if constexpr (ValidLuaType<T>) {
					if constexpr (std::is_same_v<T, int>) {
						lua_pushinteger(L, value);
					}
					else if constexpr (std::is_same_v<T, double>) {
						lua_pushnumber(L, value);
					}
					else if constexpr (std::is_same_v<T, std::string>) {
						lua_pushstring(L, value.c_str());
					}
				} else if constexpr (ValidLuaVector<T>) {
					lua_newtable(L);
					for (int i = 0; i < value.size(); i++) {
						SetValue(i + 1, value[i]);
					}
				}
				lua_setglobal(L, name.c_str());
			};
			template<LuaType T>
			CTOAST_API void SetValue(string hierarchy, string key, T value) {
				std::vector<std::string> tables;
				std::string currentTable = "";
				for (int i = 0; i < hierarchy.size(); i++) {
					if (hierarchy[i] == '.') {
						tables.push_back(currentTable);
						currentTable = "";
					}
					else {
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
					}
					else if constexpr (std::is_same_v<T, double>) {
						lua_pushnumber(L, value);
					}
					else if constexpr (std::is_same_v<T, std::string>) {
						lua_pushstring(L, value.c_str());
					}
				}
				else if constexpr (ValidLuaVector<T>) {
					lua_newtable(L);
					for (int i = 0; i < value.size(); i++) {
						SetValue(i + 1, value[i]);
					}
				}
				lua_setfield(L, -2, key.c_str());
			};
			CTOAST_API void SetValue(string hierarchy, string key, string value) {
				std::vector<std::string> tables;
				std::string currentTable = "";
				for (int i = 0; i < hierarchy.size(); i++) {
					if (hierarchy[i] == '.') {
						tables.push_back(currentTable);
						currentTable = "";
					}
					else {
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
			CTOAST_API void CreateTable(std::string name) {
				lua_newtable(L);
				lua_setglobal(L, name.c_str());
			};
		
			// Note: hierarchy corresponds to the table hierarchy (not including the table itself), e.g. "table1.table2.table3" or "foo" (if foo.bar will be initialized).
			CTOAST_API void CreateTable(std::string hierarchy, std::string name) {
				std::vector<std::string> tables;
				std::string currentTable = "";
				for (int i = 0; i < hierarchy.size(); i++) {
					if (hierarchy[i] == '.') {
						tables.push_back(currentTable);
						currentTable = "";
					}
					else {
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
			};
			/*void SetValue(std::string tableName, std::string key, int value);
			void SetValue(std::string tableName, std::string key, double value);
			void SetValue(std::string tableName, std::string key, std::string value);*/
			CTOAST_API void RegisterFunction(std::string tableName, std::string name, lua_CFunction func) {
				lua_getglobal(L, tableName.c_str());
				lua_pushcfunction(L, func);
				lua_setfield(L, -2, name.c_str());
			};
			
			CTOAST_API void InitializeLuaApis(void (*customInitializer)(LuaInstance* self)) {
				CreateTable(CTOAST_LUA_API);
				if (customInitializer != nullptr) {
					customInitializer(this);
				}
			};
			CTOAST_API LuaInstance() {
				L = luaL_newstate();
				luaopen_base(L);    // Opens basic functions (_G)
				luaopen_table(L);   // Opens table library
				luaopen_string(L);  // Opens string library
				luaopen_math(L);    // Opens math library
				lua_pushnil(L);
				lua_setglobal(L, "dofile");  // Remove dofile (prevents running files)
				lua_pushnil(L);
				lua_setglobal(L, "loadfile"); // Remove loadfile (prevents loading scripts)
				lua_pushnil(L);
				lua_setglobal(L, "load");     // Remove load (prevents running arbitrary Lua code)
				lua_pushnil(L);
				lua_setglobal(L, "require");  // Remove require (prevents loading additional modules)

			}
	};
	
}
