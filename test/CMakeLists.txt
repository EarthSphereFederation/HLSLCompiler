add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(HLSLCompilerTestCli main.cpp)
target_link_libraries(HLSLCompilerTestCli PRIVATE HLSLCompiler)

# config vulkan
if (WIN32)
    set(Vulkan_INCLUDE_DIRS "$ENV{VULKAN_SDK}/Include")
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VULKAN_LIBRARY "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
        set(VULKAN_STATIC_LIBRARY "$ENV{VULKAN_SDK}/Lib/vkstatic.1.lib")
    else()
        set(VULKAN_LIBRARY "$ENV{VULKAN_SDK}/Lib32/vulkan-1.lib")
        set(VULKAN_STATIC_LIBRARY "$ENV{VULKAN_SDK}/Lib32/vkstatic.1.lib")
    endif()
elseif (APPLE)
    set(CMAKE_FIND_FRAMEWORK NEVER)
    find_library(VULKAN_LIBRARY MoltenVK)
    set(CMAKE_FIND_FRAMEWORK ONLY)
    find_library(VULKAN_STATIC_LIBRARY MoltenVK)
    find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
        "${VULKAN_LIBRARY}/Headers")
else()
    find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS
        "$ENV{VULKAN_SDK}/include")
    find_library(VULKAN_LIBRARY NAMES vulkan HINTS
        "$ENV{VULKAN_SDK}/lib")
endif()
include_directories(${Vulkan_INCLUDE_DIRS})
list(APPEND LIBS ${VULKAN_LIBRARY})

find_package(glfw3 CONFIG REQUIRED)
add_executable(HLSLCompilerTestTriangle triangles.cpp)
target_link_libraries(HLSLCompilerTestTriangle PRIVATE HLSLCompiler ${VULKAN_LIBRARY} glfw)
