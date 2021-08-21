#pragma once
#include<vector>
#include<map>
#include<iostream>
#include<ctime>
#include<algorithm>

#include<sqlite3.h>
extern "C" {

#ifdef _WIN32

#include<SDL.h>
#include<SDL_image.h>

#elif __linux__

#include<SDL2/SDl.h>
#include<SDL2/SDL_image.h>

#endif

}

/*
 * 生成地图与人物相关
 */
#include"Map/hMap.h"

/*
 * 绘制相关
 */
#include"Draw/hDraw.h"

/*
 * 获取与记录游戏信息相关
 */
#include"Record/hRecord.h"

/*
 * 游戏上层相关
 */
#include"Game/hGame.h"
