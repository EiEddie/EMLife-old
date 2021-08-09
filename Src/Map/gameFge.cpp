#include"hMap.h"

GameFge::GameFge() :
		fgeCod({1, 1}),
		fgeCoin(0),
		fgeStar(0),
		ifWin(0) {
	std::srand(time(nullptr));
	demonPos = new int[mapDemon]();
	for(int i=0; i<mapDemon; i++) demonPos[i] = std::rand()%6;
}

GameFge::~GameFge() {
	delete[] demonPos;
}

void GameFge::FgeMove(SDL_Keycode dir) {
	/*移动人物*/
	
	switch(dir) {
		case SDLK_UP:
			if(mapMaze[fgeCod.y - 1][fgeCod.x] != 0) fgeCod.y--;
			break;
		case SDLK_DOWN:
			if(mapMaze[fgeCod.y + 1][fgeCod.x] != 0) fgeCod.y++;
			break;
		case SDLK_LEFT:
			if(mapMaze[fgeCod.y][fgeCod.x - 1] != 0) fgeCod.x--;
			break;
		case SDLK_RIGHT:
			if(mapMaze[fgeCod.y][fgeCod.x + 1] != 0) fgeCod.x++;
			break;
	}
}

void GameFge::FgeEat() {
	/*获取战利品*/
	
	switch(mapMaze[fgeCod.y][fgeCod.x]) {
		case 2:
			mapMaze[fgeCod.y][fgeCod.x] = 1;
			fgeCoin++;
			break;
		case 3:
			mapMaze[fgeCod.y][fgeCod.x] = 1;
			fgeStar++;
	}
}

void GameFge::FgeBehave(SDL_Keycode dir) {
	/*人物行为*/
	
	FgeMove(dir);
	FgeEat();
	if(fgeStar == mapStar) ifGetAllStar = true;
}

void GameFge::DemonMove() {
	for(int i=0; i<mapDemon; i++) {
		if(demonPos[i] == 5) demonPos[i] = -5;
		demonPos[i]++;
	}
}

void GameFge::FgeIfWin() {
	if(
			fgeCod.y == mapEnd.y
			&& fgeCod.x == mapEnd.x
			&& ifGetAllStar && ifWin == 0
			) ifWin = 1;
	for(int i=0; i<mapDemon; i++) {
		Cod demonCod = demonPoint[i][demonPos[i] < 0? -demonPos[i]: demonPos[i]];
		if(			fgeCod.y == demonCod.y
					&& fgeCod.x == demonCod.x
					&& ifWin == 0
				) ifWin = -1;
	}
}
