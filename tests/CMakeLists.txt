cmake_minimum_required(VERSION 3.13)

set(STRMD_TEST_SOURCES
	ContainerDeserializationTests.cpp
	CrossSerializationTests.cpp
	DeserializerTests.cpp
	HelpersTests.cpp
	PackerTests.cpp
	RecursiveTypesSupport.cpp
	SerializerTests.cpp
	TypeTraitsExTests.cpp
	TypeTraitsTests.cpp
	VersionedDeSerializationTests.cpp
	VersionedSerializationTests.cpp
)

add_compile_options(
	"$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-std=c++03>"
	"$<$<CXX_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:-fPIC;-fshort-wchar;-Wpedantic;-Wall;-Wextra;-Wno-long-long>>"
	"$<$<CXX_COMPILER_ID:MSVC>:-W4>"
)
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

set_property(SOURCE RecursiveTypesSupport.cpp TypeTraitsExTests.cpp APPEND PROPERTY COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-std=c++11>")

add_library(strmd.tests SHARED ${STRMD_TEST_SOURCES})
target_link_libraries(strmd.tests strmd)

add_utee_test(strmd.tests)
