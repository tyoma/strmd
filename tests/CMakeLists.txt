include_directories(${STRMD_SOURCE_DIR}/libraries/utee)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")

set(STRMD_TEST_SOURCES
	main.cpp
	DeserializerTests.cpp
	SerializerTests.cpp
	TypeTraitsTests.cpp
)

add_executable(strmd.tests ${STRMD_TEST_SOURCES})

target_link_libraries(strmd.tests utee)
