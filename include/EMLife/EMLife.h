/**
 * Copyright (c) 2021 EiEddie.
 *
 *     _/_/_/_/  _/  _/_/_/_/        _/        _/  _/
 *    _/            _/          _/_/_/    _/_/_/        _/_/
 *   _/_/_/    _/  _/_/_/    _/    _/  _/    _/  _/  _/_/_/_/
 *  _/        _/  _/        _/    _/  _/    _/  _/  _/
 * _/_/_/_/  _/  _/_/_/_/    _/_/_/    _/_/_/  _/    _/_/_/
 *
 */

#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <fstream>

#include <sqlite3.h>
#include <json/json.h>
extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

/**
 * \brief 生成地图与人物相关
 */
#include "map.h"

/**
 * \brief 绘制相关
 */
#include "draw.h"

/**
 * \brief 获取与记录游戏信息相关
 */
#include "record.h"

/**
 * \brief 游戏上层相关
 */
#include "game.h"
