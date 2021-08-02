#CMake版本
cmake_minimum_required(VERSION 3.19)
#C++编译版本
set(CMAKE_CXX_STANDARD 11)

#项目名称
project(EMLife)
#可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin)
#不显示dos窗口
set(CMAKE_CXX_FLAGS "-mwindows")
message(STATUS "Dos-Window: Invisible")

#引入头文件
include_directories(Lib/SDL2/include)
#链接库文件
link_directories(Lib/SDL2/lib)

#引入头文件
include_directories(Lib/SDL2_image/include)
#链接库文件
link_directories(Lib/SDL2_image/lib)

#添加编译文件
add_executable(EMLife
		#图标
		Build/Icon/icon.o

		Build/headEML.h
		Build/main.cpp
		Build/Class/GameFge.cpp
		Build/Class/GameMap.cpp
		Build/Class/GameDrawSet.cpp
		Build/Class/GameDrawWord.cpp
		Build/Class/GameDraw.cpp
		Build/Class/Game.cpp)

#将第三方库链接在一起
target_link_libraries(EMLife
		"SDL2main"
		"SDL2"
		"SDL2_image"
		)