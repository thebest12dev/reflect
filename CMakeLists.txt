# Set the minimum required CMake version
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the project
project ("cinnamontoast")

# Collect .cpp and .h files from specific directories
file(GLOB_RECURSE CTOAST_CORE "src/*.cpp" "src/*.h")
file(GLOB_RECURSE CTOAST_CRASHHANDLER "crash/*.cpp" "crash/*.h")
file(GLOB_RECURSE CTOAST_CLI "cli/*.cpp" "cli/*.h")
file(GLOB_RECURSE CTOAST_LUA "lua/*.cpp" "lua/*.h")
file(GLOB_RECURSE TINYXML2 "lib/tinyxml2/*.cpp" "lib/tinyxml2/*.h")


# Add the source files for the specific DLLs
add_library(CinnamonToast.CrashHandler SHARED ${CTOAST_CRASHHANDLER} )
add_library(CinnamonToast.LuaAPI SHARED ${CTOAST_LUA} )
add_library(tinyxml2 SHARED ${TINYXML2} )

# use precompiled binaries as it is too hard to compile luajit
add_library(CinnamonToast.Core SHARED ${CTOAST_CORE} )

target_compile_definitions(CinnamonToast.CrashHandler PRIVATE CTOAST_SHARED_LIBRARY=1)
target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_SHARED_LIBRARY=1)
target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_LUA=1)
target_compile_definitions(CinnamonToast.LuaAPI PRIVATE CTOAST_SHARED_LIBRARY=1)
target_compile_definitions(tinyxml2 PRIVATE TINYXML2_EXPORT=1)

# Add source files for the executable (cli)
add_executable(ctoasti ${CTOAST_CLI} )

# Link CinnamonToast.Core with CinnamonToast.CrashHandler (making the dependency)

target_link_libraries(CinnamonToast.LuaAPI PRIVATE "${CMAKE_SOURCE_DIR}/lib/luajit/lua51.lib")
target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.CrashHandler)
target_link_libraries(CinnamonToast.Core PRIVATE tinyxml2)
target_link_libraries(CinnamonToast.Core PRIVATE "${CMAKE_SOURCE_DIR}/lib/luajit/lua51.lib")
target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.LuaAPI)
target_include_directories(CinnamonToast.LuaAPI PRIVATE "${CMAKE_SOURCE_DIR}/common")
target_include_directories(CinnamonToast.CrashHandler PRIVATE "${CMAKE_SOURCE_DIR}/common")
target_include_directories(CinnamonToast.Core PRIVATE "${CMAKE_SOURCE_DIR}/common")
target_include_directories(ctoasti PRIVATE "${CMAKE_SOURCE_DIR}/common")
# Add the target executable (cli) - linking the core library
target_link_libraries(ctoasti PRIVATE CinnamonToast.Core)
target_compile_options(ctoasti PRIVATE /MT)
if(MSVC)
    target_link_libraries(CinnamonToast.Core PRIVATE d2d1 gdi32 comctl32)
endif()

# Set C++20 standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CinnamonToast.Core PROPERTY CXX_STANDARD 20)
  set_property(TARGET CinnamonToast.CrashHandler PROPERTY CXX_STANDARD 20)
  set_property(TARGET ctoasti PROPERTY CXX_STANDARD 20)
  set_property(TARGET CinnamonToast.LuaAPI PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/lua51.dll  # The new renamed file
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${CMAKE_SOURCE_DIR}/lib/luajit/lua51.dll  # Source DLL
            ${CMAKE_BINARY_DIR}/lua51.dll          # Destination with new name
    DEPENDS ${CMAKE_SOURCE_DIR}/lib/luajit/lua51.dll  # Ensure it runs when the source changes
)
