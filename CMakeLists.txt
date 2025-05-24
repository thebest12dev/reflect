# Set the minimum required CMake version
cmake_minimum_required (VERSION 3.8)
set(REFLECT_BUNDLE 1)
set(CMAKE_CXX_STANDARD 20) # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Generating files...")
message(STATUS "vcpkg toolchain location: ${CMAKE_TOOLCHAIN_FILE}")
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if (CMAKE_C_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
  
endif()
# Enable exceptions in MSVC
if(MSVC)
    add_compile_options(/EHsc)
endif()

# Define the project
set(TRIPLET "x64-windows")
set(VCPKG_TARGET_TRIPLET "x64-windows")
project("cinnamontoast")
set(CMAKE_GENERATOR_PLATFORM "x64")

enable_testing()

file(GLOB_RECURSE REFLECT_CORE "src/core/*.cpp" "src/core/*.h")
file(GLOB_RECURSE REFLECT_NET "src/net/*.cpp" "src/net/*.h" )
file(GLOB_RECURSE REFLECT_CRASHHANDLER "src/crash/*.cpp" "src/crash/*.h")
file(GLOB_RECURSE REFLECT_CLI 
    "src/cli/*.cpp" "src/cli/*.h" "src/windows/reflecti.rc"
)
file(GLOB_RECURSE REFLECT_TEST 
    "test/*.cpp"
)
file(GLOB_RECURSE REFLECT_CSTYLE "src/api/cstyle/*.cpp" "src/api/cstyle/*.h")
file(GLOB_RECURSE REFLECT_LUA "src/api/lua/*.cpp" "src/api/lua/*.h")
file(GLOB_RECURSE REFLECT_JAVA "src/api/java/*.c" "src/api/java/*.h")
if(UNIX)
    file(GLOB_RECURSE REFLECT_CORE "src/core/*.cpp" "src/core/*.h")
    file(GLOB_RECURSE REFLECT_CRASHHANDLER "src/crash/*.cpp" "src/crash/*.h")
    file(GLOB_RECURSE REFLECT_CLI 
        "src/cli/*.cpp" "src/cli/*.h" "src/windows/reflecti.rc"
    )
    file(GLOB_RECURSE REFLECT_TEST 
        "test/*.cpp"
    )
    file(GLOB_RECURSE REFLECT_CSTYLE "src/api/cstyle/*.cpp" "src/api/cstyle/*.h")
    file(GLOB_RECURSE REFLECT_LUA "src/api/lua/*.cpp" "src/api/lua/*.h")
    file(GLOB_RECURSE REFLECT_JAVA "src/api/java/*.c" "src/api/java/*.h")
endif()

# Collect .cpp and .h files from specific directories

get_filename_component(VCPKG_DIR "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
set(VCPKG_DIR "${VCPKG_DIR}/../../")
set(PKG_CONFIG_EXECUTABLE "${VCPKG_DIR}/installed/${TRIPLET}/tools/pkgconf/pkgconf.exe")
set(LIB_FILE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/lib")
set(JAVA_VERSION "21")
set(JNI_INCLUDE_DIR "$ENV{JAVA_HOME}/include/win32" "$ENV{JAVA_HOME}/include")
if(UNIX)
    set(JNI_INCLUDE_DIR "$ENV{JAVA_HOME}/include/linux" "$ENV{JAVA_HOME}/include")
endif(UNIX)
if(DEFINED REFLECT_BUNDLE) 
    file(GLOB_RECURSE REFLECT_COMMONS "src/common/*.h" )
    file(GLOB_RECURSE REFLECT_COMMONS_CORE "src/common/*.h"  "src/common/*.cpp")
    add_library(Reflect SHARED ${REFLECT_CSTYLE} 
                ${REFLECT_CRASHHANDLER} 
                ${REFLECT_LUA}
                ${REFLECT_JAVA}
                ${REFLECT_NET} ${REFLECT_CORE} ${REFLECT_COMMONS_CORE}
    )
    target_compile_definitions(Reflect PRIVATE REFLECT_SHARED_LIBRARY=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_LUA=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_BUILDING=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_BUNDLED=1)

else()
    if(DEFINED REFLECT_ONEFILE)
    set(REFLECT_BUNDLE 1)
        file(GLOB_RECURSE REFLECT_COMMONS "src/common/*.h" )
    file(GLOB_RECURSE REFLECT_COMMONS_CORE "src/common/*.h"  "src/common/*.cpp")

   
    add_library(Reflect STATIC ${REFLECT_CSTYLE} 
                ${REFLECT_CRASHHANDLER} 
                ${REFLECT_LUA}
                ${REFLECT_JAVA}
                ${REFLECT_NET} ${REFLECT_CORE} ${REFLECT_COMMONS_CORE}
    )
    target_compile_definitions(Reflect PRIVATE REFLECT_STATIC_LIBRARY=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_LUA=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_BUILDING=1)
    target_compile_definitions(Reflect PRIVATE REFLECT_BUNDLED=1)
    else()
        file(GLOB_RECURSE REFLECT_COMMONS_CORE "src/common/*.h"  "src/common/*.cpp")
        # Add the source files for the specific DLLs
        add_library(Reflect.CStyleAPI SHARED ${REFLECT_CSTYLE} "src/windows/Reflect.CStyleAPI.rc")
        add_library(Reflect.CrashHandler SHARED ${REFLECT_CRASHHANDLER} "src/windows/Reflect.CrashHandler.rc")
        add_library(Reflect.LuaAPI SHARED ${REFLECT_LUA} "src/windows/Reflect.LuaAPI.rc")
        add_library(Reflect.JavaAPI SHARED ${REFLECT_JAVA} "src/windows/Reflect.JavaAPI.rc")
        add_library(Reflect.Networking SHARED ${REFLECT_NET})
        add_library(Reflect.Core SHARED ${REFLECT_CORE} ${REFLECT_COMMONS_CORE} "src/windows/Reflect.Core.rc") 
        add_library(reflect::Core ALIAS Reflect.Core)
        add_library(reflect::CrashHandler ALIAS Reflect.CrashHandler)
        add_library(reflect::LuaAPI ALIAS Reflect.LuaAPI)
        add_library(reflect::JavaAPI ALIAS Reflect.JavaAPI)
        add_library(reflect::CStyleAPI ALIAS Reflect.CStyleAPI)
        add_library(reflect::Networking ALIAS Reflect.Networking)
        target_compile_definitions(Reflect.Core PRIVATE REFLECT_SHARED_LIBRARY=1)
        target_compile_definitions(Reflect.Core PRIVATE REFLECT_EXT_NETWORKING=1)
        target_compile_definitions(Reflect.CrashHandler PRIVATE REFLECT_SHARED_LIBRARY=1)

        target_compile_definitions(Reflect.Core PRIVATE REFLECT_LUA=1)
        target_compile_definitions(Reflect.LuaAPI PRIVATE REFLECT_SHARED_LIBRARY=1)
        target_compile_definitions(Reflect.CStyleAPI PRIVATE REFLECT_BUILDING=1)
    endif()
    
endif()
#link_directories("${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/lib")

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
file(GLOB_RECURSE REFLECT_FILES "src/*.cpp" "src/*.c" "src/*.h")

# Define a custom target to run clang-format
add_custom_target(
    format
    COMMAND clang-format -i ${REFLECT_FILES}  # -i option applies changes to the files
    COMMENT "Running clang-format on source files"
)

# Optionally, make the "format" target depend on the "all" target so that it runs by default
if(DEFINED REFLECT_BUNDLE)
    add_dependencies(Reflect format)
else()
    add_dependencies(Reflect.Core format)
endif()
set(APP_MANIFEST "${CMAKE_SOURCE_DIR}/src/windows/reflecti.manifest")
# Add source files for the executable (cli)
add_executable(reflecti ${REFLECT_CLI} ${APP_MANIFEST})
add_executable(reflectw ${REFLECT_CLI})

if(REFLECT_ONEFILE)
  target_compile_definitions(reflecti PRIVATE REFLECT_STATIC_LIBRARY=1)
  target_compile_definitions(reflectw PRIVATE REFLECT_STATIC_LIBRARY=1)
endif()


# Embed the manifest into the executables
set_target_properties(reflecti PROPERTIES LINK_FLAGS "/MANIFEST /MANIFESTFILE:${APP_MANIFEST}")
set_target_properties(reflectw PROPERTIES LINK_FLAGS "/MANIFEST /MANIFESTFILE:${APP_MANIFEST}")
target_include_directories(reflectw PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_compile_definitions(reflectw PRIVATE REFLECT_NO_CONSOLE=1)
set_target_properties(reflectw PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
     

add_executable(reflectt ${REFLECT_TEST})
find_package(tinyxml2 CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

set(LUAJIT_LIBRARIES "${LIB_FILE_DIR}/${LUAJIT_LIBRARIES}.lib")
message(STATUS "LuaJIT binaries: ${LUAJIT_LIBRARIES}")
file(GLOB DLL_FILE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/bin/*")

#foreach(file ${DLL_FILE_DIR})
#    if(NOT IS_DIRECTORY ${file})
#        # Copy the file if it is not a directory
#        message(STATUS "Copying file ${file}...")
#        file(COPY ${file} DESTINATION ".")
#    endif()
# #endforeach()
if(UNIX)
    set(LUAJIT_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/lib/libluajit-5.1.a")

endif()
if(DEFINED REFLECT_BUNDLE)

    target_link_libraries(Reflect PRIVATE tinyxml2::tinyxml2 GLEW::GLEW)
    target_link_libraries(Reflect PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(Reflect PRIVATE OpenSSL::SSL OpenSSL::Crypto ws2_32)
else()
  
    target_link_libraries(Reflect.CStyleAPI PRIVATE Reflect.Core)
    target_link_libraries(Reflect.JavaAPI PRIVATE Reflect.CStyleAPI)
    target_link_libraries(Reflect.LuaAPI PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(Reflect.Core PRIVATE Reflect.CrashHandler)
    target_link_libraries(Reflect.Core PRIVATE tinyxml2::tinyxml2)
    target_link_libraries(Reflect.Core PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(Reflect.Core PRIVATE Reflect.LuaAPI GLEW::glew)
    target_link_libraries(Reflect.Networking PRIVATE OpenSSL::SSL ws2_32)
endif()
# do not let main() of program conflict with main() of gtest
target_compile_definitions(reflectt PRIVATE REFLECT_TESTS=1)
# add tests
target_link_libraries(reflectt PRIVATE GTest::gtest GTest::gtest_main)
 
if(DEFINED REFLECT_BUNDLE)
    target_include_directories(Reflect PRIVATE ${JNI_INCLUDE_DIR})
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(Reflect PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/")
    target_include_directories(Reflect PRIVATE "${CMAKE_SOURCE_DIR}/src/common" )
else()
    target_include_directories(Reflect.JavaAPI PRIVATE ${JNI_INCLUDE_DIR})
    target_include_directories(Reflect.JavaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect.LuaAPI PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(Reflect.Core PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(Reflect.LuaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect.CrashHandler PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect.Core PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(Reflect.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/")
    
    target_include_directories(Reflect.Networking PRIVATE "${CMAKE_SOURCE_DIR}/src/common" )
    
endif()
target_include_directories(reflecti PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(reflectt PRIVATE "${CMAKE_SOURCE_DIR}/src/common")

# Add the target executable (cli) - linking the core library

if(DEFINED REFLECT_BUNDLE)
    target_link_libraries(reflecti PRIVATE Reflect)
    target_link_libraries(reflectt PRIVATE Reflect)
else()
    target_link_libraries(reflecti PRIVATE Reflect.Core)
    target_link_libraries(reflectt PRIVATE Reflect.Core)
    target_link_libraries(reflectt PRIVATE Reflect.Networking)
    target_link_libraries(reflectt PRIVATE Reflect.CrashHandler)
endif()

if(WIN32)
    if(DEFINED REFLECT_BUNDLE)
        target_link_libraries(Reflect PRIVATE d2d1 gdi32 comctl32 opengl32 ws2_32)
    else()
       if(REFLECT_ONEFILE)
        target_link_libraries(Reflect.Core PRIVATE d2d1 gdi32 comctl32 opengl32)
       else()
        target_link_libraries(Reflect.Core PRIVATE d2d1 gdi32 comctl32 opengl32)
       endif()
        
    endif()
   
endif()
if(WIN32 AND NOT DEFINED REFLECT_UNIX_NAMES) 
# use plain old names
message(STATUS "Using plain names!")
endif()
if (REFLECT_UNIX_NAMES EQUAL 0 OR UNIX)
    if (DEFINED REFLECT_BUNDLE)
        set_target_properties(Reflect PROPERTIES OUTPUT_NAME "reflect")
    else()
    set_target_properties(Reflect.Core PROPERTIES OUTPUT_NAME "reflect_core")
    set_target_properties(Reflect.Networking PROPERTIES OUTPUT_NAME "reflect_networking")
    set_target_properties(Reflect.CrashHandler PROPERTIES OUTPUT_NAME "eflect_crash")
    set_target_properties(Reflect.CStyleAPI PROPERTIES OUTPUT_NAME "reflect_cstyle")
    set_target_properties(Reflect.LuaAPI PROPERTIES OUTPUT_NAME "reflect_lua")
    set_target_properties(Reflect.JavaAPI PROPERTIES OUTPUT_NAME "reflect_jni")
    endif()
elseif (REFLECT_DOTNET_NAMES)
    # Warning: may be unstable
    message(WARNING ".NET-style names work, but this may break some scripts and programs.")
    if (DEFINED REFLECT_BUNDLE)
        set_target_properties(Reflect PROPERTIES OUTPUT_NAME "Reflect")
    else()
        set_target_properties(Reflect.Core PROPERTIES OUTPUT_NAME "Reflect.Core")
        set_target_properties(Reflect.Networking PROPERTIES OUTPUT_NAME "Reflect.Networking")
        set_target_properties(Reflect.CrashHandler PROPERTIES OUTPUT_NAME "Reflect.CrashHandler")
        set_target_properties(Reflect.CStyleAPI PROPERTIES OUTPUT_NAME "Reflect.CStyleAPI")
        set_target_properties(Reflect.LuaAPI PROPERTIES OUTPUT_NAME "Reflect.LuaAPI")
        set_target_properties(Reflect.JavaAPI PROPERTIES OUTPUT_NAME "Reflect.JavaAPI")
    endif()
    
    
else()
    # use plain names
    if (DEFINED REFLECT_BUNDLE)
        set_target_properties(Reflect PROPERTIES OUTPUT_NAME "reflect")
    else()
       set_target_properties(Reflect.Core PROPERTIES OUTPUT_NAME "reflectcore")
        set_target_properties(Reflect.Networking PROPERTIES OUTPUT_NAME "reflectnet")
        set_target_properties(Reflect.CrashHandler PROPERTIES OUTPUT_NAME "reflectcrash")
        set_target_properties(Reflect.CStyleAPI PROPERTIES OUTPUT_NAME "reflectcstyle")
        set_target_properties(Reflect.LuaAPI PROPERTIES OUTPUT_NAME "reflectlua")
        set_target_properties(Reflect.JavaAPI PROPERTIES OUTPUT_NAME "reflectjni")
    endif()
    
endif()
# Set C++20 standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
    if(DEFINED REFLECT_BUNDLE)
      set_property(TARGET Reflect PROPERTY CXX_STANDARD 20)
      set_property(TARGET Reflect PROPERTY C_STANDARD 99)
    else()
      set_property(TARGET Reflect.Core PROPERTY CXX_STANDARD 20)
      set_property(TARGET Reflect.CrashHandler PROPERTY CXX_STANDARD 20)
      set_property(TARGET reflecti PROPERTY CXX_STANDARD 20)
      set_property(TARGET reflectt PROPERTY CXX_STANDARD 20)
      set_property(TARGET Reflect.LuaAPI PROPERTY CXX_STANDARD 20)
      set_property(TARGET Reflect.JavaAPI PROPERTY C_STANDARD 99)
    endif(DEFINED REFLECT_BUNDLE)
  
endif()

target_link_libraries(reflectw PRIVATE Reflect)
add_test(NAME ReflectTest COMMAND reflectt)