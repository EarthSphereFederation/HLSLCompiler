add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

add_executable(HLSLCompilerTest main.cpp)
target_link_libraries(HLSLCompilerTest PRIVATE HLSLCompiler)