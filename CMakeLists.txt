# Set the minimum required CMake version
cmake_minimum_required (VERSION 3.8)
message(STATUS "Generating files...")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the project
project("cinnamontoast")
enable_testing()


# Collect .cpp and .h files from specific directories
file(GLOB_RECURSE CTOAST_CORE "src/core/*.cpp" "src/core/*.h" "src/windows/CinnamonToast.Core.rc")
file(GLOB_RECURSE CTOAST_CRASHHANDLER "src/crash/*.cpp" "src/crash/*.h" "src/windows/CinnamonToast.CrashHandler.rc")
file(GLOB_RECURSE CTOAST_CLI 
    "src/cli/*.cpp" "src/cli/*.h" "src/windows/ctoasti.rc"
)
file(GLOB_RECURSE CTOAST_TEST 
    "test/*.cpp"
)
file(GLOB_RECURSE CTOAST_CSTYLE "src/api/cstyle/*.cpp" "src/api/cstyle/*.h" "src/windows/CinnamonToast.CStyleAPI.rc")
file(GLOB_RECURSE CTOAST_LUA "src/api/lua/*.cpp" "src/api/lua/*.h" "src/windows/CinnamonToast.LuaAPI.rc")
file(GLOB_RECURSE CTOAST_JAVA "src/api/java/*.c" "src/api/java/*.h" "src/windows/CinnamonToast.JavaAPI.rc")
get_filename_component(VCPKG_DIR "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
set(VCPKG_DIR "${VCPKG_DIR}/../../")
set(PKG_CONFIG_EXECUTABLE "${VCPKG_DIR}/installed/x64-windows/tools/pkgconf/pkgconf.exe")
set(LIB_FILE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib")
set(JAVA_VERSION "21")
set(JNI_INCLUDE_DIR "$ENV{JAVA_HOME}/include/win32" "$ENV{JAVA_HOME}/include")
# Add the source files for the specific DLLs
add_library(CinnamonToast.CStyleAPI SHARED ${CTOAST_CSTYLE})
add_library(CinnamonToast.CrashHandler SHARED ${CTOAST_CRASHHANDLER} )
add_library(CinnamonToast.LuaAPI SHARED ${CTOAST_LUA})
add_library(CinnamonToast.JavaAPI SHARED ${CTOAST_JAVA})
add_library(CinnamonToast.Core SHARED ${CTOAST_CORE})
link_directories("${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib")
target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_SHARED_LIBRARY=1)
target_compile_definitions(CinnamonToast.CrashHandler PRIVATE CTOAST_SHARED_LIBRARY=1)

target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_LUA=1)
target_compile_definitions(CinnamonToast.LuaAPI PRIVATE CTOAST_SHARED_LIBRARY=1)
target_compile_definitions(CinnamonToast.CStyleAPI PRIVATE CTOAST_BUILDING=1)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
file(GLOB_RECURSE CTOAST_FILES "src/*.cpp" "src/*.c" "src/*.h")

# Define a custom target to run clang-format
add_custom_target(
    format
    COMMAND clang-format -i ${CTOAST_FILES}  # -i option applies changes to the files
    COMMENT "Running clang-format on source files"
)

# Optionally, make the "format" target depend on the "all" target so that it runs by default
add_dependencies(CinnamonToast.Core format)
# Add source files for the executable (cli)
add_executable(ctoasti ${CTOAST_CLI})
add_executable(ctoastt ${CTOAST_TEST})
find_package(tinyxml2 CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

set(LUAJIT_LIBRARIES "${LIB_FILE_DIR}/${LUAJIT_LIBRARIES}.lib")
message(STATUS "LuaJIT binaries: ${LUAJIT_LIBRARIES}")
file(GLOB DLL_FILE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/*")

foreach(file ${DLL_FILE_DIR})
    if(NOT IS_DIRECTORY ${file})
        # Copy the file if it is not a directory
        message(STATUS "Copying file ${file}...")
        file(COPY ${file} DESTINATION ".")
    endif()
endforeach()
target_link_libraries(CinnamonToast.CStyleAPI PRIVATE CinnamonToast.Core)
target_link_libraries(CinnamonToast.JavaAPI PRIVATE CinnamonToast.CStyleAPI)
target_link_libraries(CinnamonToast.LuaAPI PRIVATE ${LUAJIT_LIBRARIES})
target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.CrashHandler)
target_link_libraries(CinnamonToast.Core PRIVATE tinyxml2::tinyxml2)
target_link_libraries(CinnamonToast.Core PRIVATE ${LUAJIT_LIBRARIES})
target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.LuaAPI)
# do not let main() of program conflict with main() of gtest
target_compile_definitions(ctoastt PRIVATE CTOAST_TESTS=1)
# add tests
target_link_libraries(ctoastt PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(CinnamonToast.JavaAPI PRIVATE ${JNI_INCLUDE_DIR})
target_include_directories(CinnamonToast.JavaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(CinnamonToast.LuaAPI PRIVATE ${LUAJIT_INCLUDE_DIRS})
target_include_directories(CinnamonToast.Core PRIVATE ${LUAJIT_INCLUDE_DIRS})
target_include_directories(CinnamonToast.LuaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(CinnamonToast.CrashHandler PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(CinnamonToast.Core PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(CinnamonToast.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(CinnamonToast.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/")
target_include_directories(ctoasti PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(ctoastt PRIVATE "${CMAKE_SOURCE_DIR}/src/common")

# Add the target executable (cli) - linking the core library
target_link_libraries(ctoasti PRIVATE CinnamonToast.Core)
target_link_libraries(ctoastt PRIVATE CinnamonToast.Core)
target_link_libraries(ctoastt PRIVATE CinnamonToast.CrashHandler)
if(MSVC)
   target_link_libraries(CinnamonToast.Core PRIVATE d2d1 gdi32 comctl32)
endif()

# Set C++20 standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CinnamonToast.Core PROPERTY CXX_STANDARD 20)
  set_property(TARGET CinnamonToast.CrashHandler PROPERTY CXX_STANDARD 20)
  set_property(TARGET ctoasti PROPERTY CXX_STANDARD 20)
  set_property(TARGET ctoastt PROPERTY CXX_STANDARD 20)
  set_property(TARGET CinnamonToast.LuaAPI PROPERTY CXX_STANDARD 20)
  set_property(TARGET CinnamonToast.JavaAPI PROPERTY C_STANDARD 99)
endif()


add_test(NAME CToastTest COMMAND ctoastt)