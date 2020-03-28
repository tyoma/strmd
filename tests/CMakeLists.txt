cmake_minimum_required(VERSION 2.8)

include(test)

set(STRMD_TEST_SOURCES
	ContainerDeserializationTests.cpp
	CrossSerializationTests.cpp
	DeserializerTests.cpp
	PackerTests.cpp
	SerializerTests.cpp
	TypeTraitsExTests.cpp
	TypeTraitsTests.cpp
)

add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-std=c++03>")
set_property(SOURCE TypeTraitsExTests.cpp APPEND PROPERTY COMPILE_OPTIONS "$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-std=c++11>")

add_library(strmd.tests SHARED ${STRMD_TEST_SOURCES})
