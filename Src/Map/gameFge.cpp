#include"hMap.h"

GameFge::GameFge() :
		fgeCod({1, 1}),
		fgeCoin(0),
		fgeStar(0),
		ifWin(0) {
	std::srand(time(nullptr));
	demonPos = new int[mapDemon]();
	for(int i=0; i<mapDemon; i++) demonPos[i] = std::rand()%5;
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

void GameFge::FgeAttack() {
	/*人物攻击*/
	
	bool n = false;
	if(fgeCoin >= 5) {
		for(int i = fgeCod.y - 2; i <= fgeCod.y + 2; i++) {
			for(int j = fgeCod.x - 2; j <= fgeCod.x + 2; j++) {
				if(mapMaze[i][j] == 4) {
					mapMaze[i][j] = 1;
					n = true;
				}
			}
		}
	}
	if(n) {
		fgeCoin -= 5;
	}
}

void GameFge::FgeBehave(SDL_Keycode dir) {
	/*人物行为*/
	
	if(dir == SDLK_e) {
		FgeAttack();
		return;
	}
	FgeMove(dir);
	FgeEat();
	if(fgeStar == mapStar) ifGetAllStar = true;
	if(
			fgeCod.y == mapEnd.y
			&& fgeCod.x == mapEnd.x
			&& ifGetAllStar
			)
		ifWin = 1;
	if(mapMaze[fgeCod.y][fgeCod.x] == 4) {
		ifWin = -1;
	}
}
