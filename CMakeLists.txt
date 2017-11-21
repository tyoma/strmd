cmake_minimum_required(VERSION 2.8)

project(STRMD)

include_directories(${STRMD_SOURCE_DIR})

add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(tests)
add_subdirectory(libraries/utee/src)
