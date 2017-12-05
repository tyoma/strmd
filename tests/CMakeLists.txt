include_directories(${STRMD_SOURCE_DIR}/libraries/utee)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")

set(STRMD_TEST_SOURCES
	DeserializerTests.cpp
	PackerTests.cpp
	SerializerTests.cpp
	TypeTraitsTests.cpp
)

add_library(strmd.tests SHARED ${STRMD_TEST_SOURCES})

target_link_libraries(strmd.tests utee)
