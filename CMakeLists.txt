cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
project(EMLife)

if(BITS MATCHES "x86")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
elseif(BITS MATCHES "x64")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
endif()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/dist")


if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "release")
	#不显示dos窗口
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "debug")
	add_definitions(-DDEBUG)
	message(STATUS "Add macro \"DEBUG\"")
endif()


find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(JsonCpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE SRC_FILE "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILE})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2_image)


execute_process(
		COMMAND ${CMAKE_COMMAND} -E
		copy_directory "${PROJECT_SOURCE_DIR}/res/assets" "${EXECUTABLE_OUTPUT_PATH}/assets"
)