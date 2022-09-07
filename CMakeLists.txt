cmake_minimum_required(VERSION 3.13)

project(strmd)

enable_testing()

option(STRMD_NO_TESTS "Do not build test modules." OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)

add_compile_options("$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fPIC;-Wpedantic;-Wall;-std=c++11;-fshort-wchar>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:-W4>")

add_library(strmd INTERFACE)
target_include_directories(strmd INTERFACE .)

if (NOT STRMD_NO_TESTS)
	if (NOT TARGET utee)
		set(UTEE_NO_TESTS ON)
		add_subdirectory(libraries/utee)
	endif()
	add_subdirectory(tests)
endif()
