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

#include"Map/hMap.h"
#include"Draw/hDraw.h"
#include"Record/hRecord.h"
#include"Game/hGame.h"