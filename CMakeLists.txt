#CMake版本
cmake_minimum_required(VERSION 3.19)
#C++编译版本
set(CMAKE_CXX_STANDARD 11)

#项目名称
project(EMLife)
#可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#	#不显示dos窗口
#	set(CMAKE_CXX_FLAGS "-mwindows")
#	message(STATUS "Dos-Window: Invisible")
#endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	#链接库
	link_libraries(SDL2)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	#引入头文件
	include_directories(
			Lib/SDL2-2.0.14/x86_64-w64-mingw32/include/SDL2
	)
	#链接库文件
	link_directories(
			Lib/SDL2-2.0.14/x86_64-w64-mingw32/lib
	)

	#引入头文件
	include_directories(
			Lib/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2
	)
	#链接库文件
	link_directories(
			Lib/SDL2_image-2.0.5/x86_64-w64-mingw32/lib
	)
endif()
#引入头文件
include_directories(
		Lib/SQLite3/include
)
#链接库文件
link_directories(
		Lib/SQLite3/lib
)



set(FILE
		Src/hEMLife.h
		Src/main.cpp

		Src/Map/hMap.h
		Src/Map/gameFge.cpp
		Src/Map/gameMap.cpp

		Src/Draw/hDraw.h
		Src/Draw/gameDrawSet.cpp
		Src/Draw/gameDrawWord.cpp
		Src/Draw/gameDraw.cpp

		Src/Game/hGame.h
		Src/Game/game.cpp

		Src/Record/hRecord.h
		Src/Record/gameRecord.cpp)

if(CMAKE_BUILD_TYPE MATCHES "Release")
	#图标
	list(APPEND FILE Src/Icon/icon.o)
endif()

#添加编译文件
add_executable(EMLife ${FILE})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	#将第三方库链接在一起
	target_link_libraries(EMLife
			"SDL2main"
			"SDL2"
			"SDL2_image"
			"sqlite3"
	)
endif()