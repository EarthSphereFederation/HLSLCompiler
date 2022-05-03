cmake_minimum_required(VERSION 3.17)
project(HLSLCompiler)

set(CMAKE_CXX_STANDARD 17)

option(HLSLCOMPILER_ENABLE_TEST "Enable test" ON)

if (MSVC)
    add_compile_options("/MP")
    #    add_compile_options("/experimental:module")
    #    add_compile_options("/std:c++latest")
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DNOMINMAX)
endif()

add_library(dxc INTERFACE)
set(hlsl_compiler_root "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(dxc INTERFACE "${hlsl_compiler_root}/3rdparty/dxc/include")
target_compile_definitions(dxc INTERFACE DXC_CUSTOM_LOCATION="") # Do it if you need it!
if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(windows_kits_location "C:/Program Files (x86)/Windows Kits/10/Bin/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/x64")
else()
    set(windows_kits_location "C:/Program Files (x86)/Windows Kits/10/Redist/D3D/x64") # work around for clion/ninja etc.
endif()
target_compile_definitions(dxc INTERFACE WINDOWS_KITS_LOCATION="${windows_kits_location}")

list(APPEND headers
    ShaderBaseTypes.h
    SystemUtils.h
    DXUtility.h
    DXCLoader.h
    Compiler.h
)

list(APPEND sources
    SystemUtils.cpp
    DXCLoader.cpp
    Compiler.cpp
)

add_library(HLSLCompiler ${headers} ${sources})

target_include_directories(HLSLCompiler
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(HLSLCompiler
    dxc
)

if(${HLSLCOMPILER_ENABLE_TEST})
    add_subdirectory(test)
endif()