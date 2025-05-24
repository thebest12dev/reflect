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
#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

// Include Lua headers
extern "C" {
#include "luajit/lua.h"
#include "luajit/lualib.h"
#include <luajit/lauxlib.h>
}
// Include C++ headers
#include "TypeDefinitions.h"
#include <concepts>
#include <string>
#include <type_traits>
#include <vector>

// For some reason, extern "C++" is required for the build to work.
extern "C++" {
namespace reflect {
template <typename T>
/**
 * @brief A concept to check if the type is a valid Lua type.
 */
concept ValidLuaType = std::is_same_v<T, int> || std::is_same_v<T, double> ||
                       std::is_same_v<T, std::string>;
/**
 * @brief A trait to check if the type is a valid Lua type or a vector of valid
 */
template <typename T> struct IsValidLuaTypeOrVectorTrait : std::false_type {};

template <typename T>

/**
 * @brief A specialization of the trait to check if the type is a valid Lua type
 */
struct IsValidLuaTypeOrVectorTrait<std::vector<T>>
    : std::conditional_t<ValidLuaType<T> ||
                             IsValidLuaTypeOrVectorTrait<T>::value,
                         std::true_type, std::false_type> {};

template <typename T>

/**
 * @brief A specialization of the trait to check if the type is a valid Lua type
 */
constexpr bool IsValidLuaTypeOrVector = IsValidLuaTypeOrVectorTrait<T>::value;

template <typename T>
/**
 * @brief A concept to check if the type is a valid Lua type or a vector of
 * valid Lua types.
 */
concept ValidLuaVector = requires(T t) {
  typename T::value_type;
  requires IsValidLuaTypeOrVector<typename T::value_type>;
};

template <typename T>
/**
 * @brief A concept of a valid Lua type.
 */
concept LuaType = ValidLuaType<T> || ValidLuaVector<T>;

/**
 * @brief A class to represent a Lua instance.
 *
 * Internally, it uses Lua's C API to execute Lua code and manage Lua state.
 */
class LuaInstance {
private:
  lua_State *L;

  // Helper method to set a value in the Lua stack
  template <LuaType T> void setValue(int index, T value);

public:
  /**
   * @brief A method to execute arbitrary Lua code.
   * @param code The Lua code to execute.
   *
   * This maps directly to Lua APIs.
   */
  REFLECT_API void execute(std::string code);

  /**
   * @brief A method to execute a Lua file.
   * @param filePath The path to the Lua file to execute.
   *
   * This maps directly to Lua APIs.
   */
  REFLECT_API void executeFile(std::string filePath);
  /**
   * @brief A method to register a function in Lua.
   * @param name The name of the function to register.
   * @param func The function to register.
   */
  REFLECT_API void registerFunction(std::string name, lua_CFunction func);

  /**
   * @brief A method to set a value in Lua.
   * @param name The name of the variable to set.
   * @param value The value to set. Must be a valid Lua type.
   */
  template <LuaType T> REFLECT_API void setValue(std::string name, T value);
  template <LuaType T>
  /**
   * @brief Sets the value to a valid Lua type.
   * @param hierarchy The table hierarchy (excluding the key)
   * @param key The key, as a string.
   * @param value The value, as a Lua type.
   */
  REFLECT_API void setValue(std::string hierarchy, std::string key, T value);
  /**
   * @brief A method to create a table.
   * @param name The name of the table.
   */
  REFLECT_API void createTable(std::string name);

  /**
   * Note: hierarchy corresponds to the table hierarchy (not including the table
   * itself), e.g. "table1.table2.table3" or "foo" (if foo.bar will be
   * initialized).
   */

  REFLECT_API void createTable(std::string hierarchy, std::string name);
  // REFLECT_IMPORT void SetValue(std::string tableName, std::string key,
  //  int value);
  // REFLECT_IMPORT void SetValue(std::string tableName, std::string key,
  //  double value);

  /**
   * @brief Same as setValue but value is a string.
   */
  REFLECT_API void setValue(std::string tableName, std::string key,
                            std::string value);

  /**
   * @brief Sets a value in a table to a function.
   * @param tableName The table name.
   * @param name The key to the function.
   * @param func The function.
   */
  REFLECT_API void registerFunction(std::string tableName, std::string name,
                                    lua_CFunction func);
  /**
   * @brief A method to initialize Lua APIs.
   * @param customInitializer A custom initializer function.
   */
  REFLECT_API void
  initializeLuaApis(void (*customInitializer)(LuaInstance *self));
  REFLECT_API LuaInstance();
};

} // namespace reflect
}
