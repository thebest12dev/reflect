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
#undef byte
typedef unsigned char byte ;
#include <Windows.h>
extern "C" {
		#include "../lib/luajit/lua.h"
		#include "../lib/luajit/lauxlib.h"
		#include "../lib/luajit/lualib.h"
}
#include "TypeDefinitions.h"
#include <string>
#include <vector>
#include <type_traits>
#include <concepts>


extern "C++" {
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
			__declspec(dllimport) void SetValue(int index, T value);
		public:
			__declspec(dllimport) void Execute(std::string code);
			__declspec(dllimport) void ExecuteFile(std::string filePath);
			__declspec(dllimport) void RegisterFunction(std::string name, lua_CFunction func);

			template<LuaType T>
			__declspec(dllimport) void SetValue(std::string name, T value);
			template<LuaType T>
			__declspec(dllimport) void SetValue(string hierarchy, string key, T value);
			__declspec(dllimport) void CreateTable(std::string name);

			// Note: hierarchy corresponds to the table hierarchy (not including the table itself), e.g. "table1.table2.table3" or "foo" (if foo.bar will be initialized).
			__declspec(dllimport) void CreateTable(std::string hierarchy, std::string name);
			//__declspec(dllimport) void SetValue(std::string tableName, std::string key, int value);
			//__declspec(dllimport) void SetValue(std::string tableName, std::string key, double value);
			__declspec(dllimport) void SetValue(std::string tableName, std::string key, std::string value);
			__declspec(dllimport) void RegisterFunction(std::string tableName, std::string name, lua_CFunction func);
			__declspec(dllimport) void InitializeLuaApis(void (*customInitializer)(LuaInstance* self));
			__declspec(dllimport) LuaInstance();
		};
		
	}

}
