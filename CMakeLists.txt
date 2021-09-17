cmake_minimum_required(VERSION 2.8)

project(STRMD)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)

if (NOT TARGET utee)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")
endif()

include_directories(.)

set(GCC_CLANG_COMPILER_OPTIONS -fPIC -fshort-wchar -Wpedantic -Wall -Wextra -Wno-long-long)

add_compile_options(
	"$<$<CXX_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:C>:${GCC_CLANG_COMPILER_OPTIONS}>>"
	"$<$<CXX_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:${GCC_CLANG_COMPILER_OPTIONS}>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/W4>"
)

add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(tests)
add_subdirectory(libraries/utee)
