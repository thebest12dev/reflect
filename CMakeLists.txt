# Set the minimum required CMake version
cmake_minimum_required (VERSION 3.8)
#set(CTOAST_BUNDLE 1)
message(STATUS "Generating files...")
message(STATUS "vcpkg toolchain location: ${CMAKE_TOOLCHAIN_FILE}")
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the project
set(TRIPLET "x64-windows")
project("cinnamontoast")
enable_testing()

file(GLOB_RECURSE CTOAST_CORE "src/core/*.cpp" "src/core/*.h")
file(GLOB_RECURSE CTOAST_NET "src/net/*.cpp" "src/net/*.h" )
file(GLOB_RECURSE CTOAST_CRASHHANDLER "src/crash/*.cpp" "src/crash/*.h")
file(GLOB_RECURSE CTOAST_CLI 
    "src/cli/*.cpp" "src/cli/*.h" "src/windows/ctoasti.rc"
)
file(GLOB_RECURSE CTOAST_TEST 
    "test/*.cpp"
)
file(GLOB_RECURSE CTOAST_CSTYLE "src/api/cstyle/*.cpp" "src/api/cstyle/*.h")
file(GLOB_RECURSE CTOAST_LUA "src/api/lua/*.cpp" "src/api/lua/*.h")
file(GLOB_RECURSE CTOAST_JAVA "src/api/java/*.c" "src/api/java/*.h")
if(UNIX)
    file(GLOB_RECURSE CTOAST_CORE "src/core/*.cpp" "src/core/*.h")
    file(GLOB_RECURSE CTOAST_CRASHHANDLER "src/crash/*.cpp" "src/crash/*.h")
    file(GLOB_RECURSE CTOAST_CLI 
        "src/cli/*.cpp" "src/cli/*.h" "src/windows/ctoasti.rc"
    )
    file(GLOB_RECURSE CTOAST_TEST 
        "test/*.cpp"
    )
    file(GLOB_RECURSE CTOAST_CSTYLE "src/api/cstyle/*.cpp" "src/api/cstyle/*.h")
    file(GLOB_RECURSE CTOAST_LUA "src/api/lua/*.cpp" "src/api/lua/*.h")
    file(GLOB_RECURSE CTOAST_JAVA "src/api/java/*.c" "src/api/java/*.h")
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
if(DEFINED CTOAST_BUNDLE) 
    file(GLOB_RECURSE CTOAST_COMMONS "src/common/*.cpp" "src/common/*.h")
    
    add_library(CinnamonToast SHARED ${CTOAST_CSTYLE} 
                ${CTOAST_CRASHHANDLER} 
                ${CTOAST_LUA}
                ${CTOAST_JAVA}
                ${CTOAST_NET} ${CTOAST_CORE} ${CTOAST_COMMONS}
    )
    target_compile_definitions(CinnamonToast PRIVATE CTOAST_SHARED_LIBRARY=1)
    target_compile_definitions(CinnamonToast PRIVATE CTOAST_LUA=1)
    target_compile_definitions(CinnamonToast PRIVATE CTOAST_BUILDING=1)
    target_compile_definitions(CinnamonToast PRIVATE CTOAST_BUNDLED=1)
else()
    # Add the source files for the specific DLLs
    add_library(CinnamonToast.CStyleAPI SHARED ${CTOAST_CSTYLE} "src/windows/CinnamonToast.CStyleAPI.rc")
    add_library(CinnamonToast.CrashHandler SHARED ${CTOAST_CRASHHANDLER} "src/windows/CinnamonToast.CrashHandler.rc")
    add_library(CinnamonToast.LuaAPI SHARED ${CTOAST_LUA} "src/windows/CinnamonToast.LuaAPI.rc")
    add_library(CinnamonToast.JavaAPI SHARED ${CTOAST_JAVA} "src/windows/CinnamonToast.JavaAPI.rc")
    add_library(CinnamonToast.Networking SHARED ${CTOAST_NET})
    add_library(CinnamonToast.Core SHARED ${CTOAST_CORE} "src/windows/CinnamonToast.Core.rc") 
    target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_SHARED_LIBRARY=1)
    target_compile_definitions(CinnamonToast.CrashHandler PRIVATE CTOAST_SHARED_LIBRARY=1)

    target_compile_definitions(CinnamonToast.Core PRIVATE CTOAST_LUA=1)
    target_compile_definitions(CinnamonToast.LuaAPI PRIVATE CTOAST_SHARED_LIBRARY=1)
    target_compile_definitions(CinnamonToast.CStyleAPI PRIVATE CTOAST_BUILDING=1)
endif()
#link_directories("${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/lib")

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
file(GLOB_RECURSE CTOAST_FILES "src/*.cpp" "src/*.c" "src/*.h")

# Define a custom target to run clang-format
add_custom_target(
    format
    COMMAND clang-format -i ${CTOAST_FILES}  # -i option applies changes to the files
    COMMENT "Running clang-format on source files"
)

# Optionally, make the "format" target depend on the "all" target so that it runs by default
if(DEFINED CTOAST_BUNDLE)
    add_dependencies(CinnamonToast format)
else()
    add_dependencies(CinnamonToast.Core format)
endif()

# Add source files for the executable (cli)
add_executable(ctoasti ${CTOAST_CLI})
add_executable(ctoastt ${CTOAST_TEST})
find_package(tinyxml2 CONFIG REQUIRED)
find_package(GTest REQUIRED)
set(OPENSSL_USE_STATIC_LIBS OFF)
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)

set(LUAJIT_LIBRARIES "${LIB_FILE_DIR}/${LUAJIT_LIBRARIES}.lib")
message(STATUS "LuaJIT binaries: ${LUAJIT_LIBRARIES}")
file(GLOB DLL_FILE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/bin/*")

foreach(file ${DLL_FILE_DIR})
    if(NOT IS_DIRECTORY ${file})
        # Copy the file if it is not a directory
        message(STATUS "Copying file ${file}...")
        file(COPY ${file} DESTINATION ".")
    endif()
endforeach()
if(UNIX)
    set(LUAJIT_LIBRARIES "${CMAKE_BINARY_DIR}/vcpkg_installed/${TRIPLET}/lib/libluajit-5.1.so")

endif()
if(DEFINED CTOAST_BUNDLE)
    target_link_libraries(CinnamonToast PRIVATE tinyxml2::tinyxml2)
    target_link_libraries(CinnamonToast PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(CinnamonToast PRIVATE OpenSSL::SSL ws2_32)
else()
    target_link_libraries(CinnamonToast.CStyleAPI PRIVATE CinnamonToast.Core)
    target_link_libraries(CinnamonToast.JavaAPI PRIVATE CinnamonToast.CStyleAPI)
    target_link_libraries(CinnamonToast.LuaAPI PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.CrashHandler)
    target_link_libraries(CinnamonToast.Core PRIVATE tinyxml2::tinyxml2)
    target_link_libraries(CinnamonToast.Core PRIVATE ${LUAJIT_LIBRARIES})
    target_link_libraries(CinnamonToast.Core PRIVATE CinnamonToast.LuaAPI)
    target_link_libraries(CinnamonToast.Networking PRIVATE OpenSSL::SSL ws2_32)
    target_link_options(CinnamonToast.Networking PRIVATE /OPT:REF /OPT:ICF)
endif()
# do not let main() of program conflict with main() of gtest
target_compile_definitions(ctoastt PRIVATE CTOAST_TESTS=1)
# add tests
target_link_libraries(ctoastt PRIVATE GTest::gtest GTest::gtest_main)

if(DEFINED CTOAST_BUNDLE)
    target_include_directories(CinnamonToast PRIVATE ${JNI_INCLUDE_DIR})
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(CinnamonToast PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/")
    target_include_directories(CinnamonToast PRIVATE "${CMAKE_SOURCE_DIR}/src/common" )
else()
    target_include_directories(CinnamonToast.JavaAPI PRIVATE ${JNI_INCLUDE_DIR})
    target_include_directories(CinnamonToast.JavaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast.LuaAPI PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(CinnamonToast.Core PRIVATE ${LUAJIT_INCLUDE_DIRS})
    target_include_directories(CinnamonToast.LuaAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast.CrashHandler PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast.Core PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
    target_include_directories(CinnamonToast.CStyleAPI PRIVATE "${CMAKE_SOURCE_DIR}/src/")
    
    target_include_directories(CinnamonToast.Networking PRIVATE "${CMAKE_SOURCE_DIR}/src/common" )
    
endif()
target_include_directories(ctoasti PRIVATE "${CMAKE_SOURCE_DIR}/src/common")
target_include_directories(ctoastt PRIVATE "${CMAKE_SOURCE_DIR}/src/common")

# Add the target executable (cli) - linking the core library

if(DEFINED CTOAST_BUNDLE)
    target_link_libraries(ctoasti PRIVATE CinnamonToast)
    target_link_libraries(ctoastt PRIVATE CinnamonToast)
else()
    target_link_libraries(ctoasti PRIVATE CinnamonToast.Core)
    target_link_libraries(ctoastt PRIVATE CinnamonToast.Core)
    target_link_libraries(ctoastt PRIVATE CinnamonToast.Networking)
    target_link_libraries(ctoastt PRIVATE CinnamonToast.CrashHandler)
endif()

if(MSVC)
    if(DEFINED CTOAST_BUNDLE)
        target_link_libraries(CinnamonToast PRIVATE d2d1 gdi32 comctl32 opengl32 ws2_32)
    else()
        target_link_libraries(CinnamonToast.Core PRIVATE d2d1 gdi32 comctl32 opengl32)
    endif()
   
endif()

# Set C++20 standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
    if(DEFINED CTOAST_BUNDLE)
      set_property(TARGET CinnamonToast PROPERTY CXX_STANDARD 20)
      set_property(TARGET CinnamonToast PROPERTY C_STANDARD 99)
    else()
      set_property(TARGET CinnamonToast.Core PROPERTY CXX_STANDARD 20)
      set_property(TARGET CinnamonToast.CrashHandler PROPERTY CXX_STANDARD 20)
      set_property(TARGET ctoasti PROPERTY CXX_STANDARD 20)
      set_property(TARGET ctoastt PROPERTY CXX_STANDARD 20)
      set_property(TARGET CinnamonToast.LuaAPI PROPERTY CXX_STANDARD 20)
      set_property(TARGET CinnamonToast.JavaAPI PROPERTY C_STANDARD 99)
    endif(DEFINED CTOAST_BUNDLE)
  
endif()


add_test(NAME CToastTest COMMAND ctoastt)