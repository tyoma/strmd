cmake_minimum_required(VERSION 2.8)

project(STRMD)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)

if (NOT TARGET utee)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")
endif()

include_directories(.)

add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fPIC>")
add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fshort-wchar>")
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(tests)
add_subdirectory(libraries/utee)
