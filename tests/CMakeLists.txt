cmake_minimum_required(VERSION 2.8)

include(test)

add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-std=c++11>")

set(STRMD_TEST_SOURCES
	DeserializerTests.cpp
	PackerTests.cpp
	SerializerTests.cpp
	TypeTraitsTests.cpp
)

add_library(strmd.tests SHARED ${STRMD_TEST_SOURCES})
