#include"hDraw.h"

void WallDrawInf::SetPointInf(int **maze, const Cod& point) {
	WallInf temp = wallInf[GetPointNum(maze, point)];
	this->num = temp.num;
	this->angle = temp.angle;
}


int WallDrawInf::GetPointNum(int **maze, const Cod& point) {
	int pointInf = 0;
	for(int i=3; i>=0; i--) {
		Cod temp = point;
		if(
				temp.Move(i, 1)
				&& maze[temp.y][temp.x] == 0
				) {
			pointInf = pointInf | 0b1;
		}
		pointInf = pointInf<<1;
	}
	
	return pointInf>>1;
}


GameDraw::GameDraw(unsigned int fps, GameFge *gameFge):
		GameDrawWord(fps, gameFge),
		fpsNum(0),
		drawScreenRect({
			0, 0,
			mazeInf.xLength*24,
			mazeInf.yLength*24
		}) {}

void GameDraw::FpsManagerBegin() {
	timeBegin = SDL_GetTicks();
	if(fpsNum > 60) fpsNum = 0;
	fpsNum++;
}

void GameDraw::FpsManagerEnd() const {
	Uint32 timeDelta = SDL_GetTicks() - timeBegin;
	if(timeDelta < stepFps) {
		SDL_Delay(stepFps - timeDelta);
	}
}

void GameDraw::ShowMap() {
	SDL_Rect drawElementCod = {0, 0, 24, 24};
	for(int i=0; i<mazeInf.yLength; i++) {
		for(int j = 0; j<mazeInf.xLength; j++) {
			int temp = drawGameFge->mapMaze[i][j];
			drawElementCod.y = i*24;
			drawElementCod.x = j*24;
			if(temp == 0) {
				wallDrawInf.SetPointInf(drawGameFge->mapMaze, {i, j});
				
				SDL_RenderCopyEx(
						drawRen, drawWallImg[wallDrawInf.num],
						nullptr, &drawElementCod,
						wallDrawInf.angle, nullptr, wallDrawInf.flip
				);
			} else if((temp == -2 && drawGameFge->ifGetAllStar) || (temp != 1 && temp != -2)) {
				SDL_RenderCopy(drawRen, drawGameImg[temp], nullptr, &drawElementCod);
			}
		}
	}
}

void GameDraw::ShowDemon() {
	for(int i=0; i<mazeInf.demon; i++) {
		int drawDemonPos = drawGameFge->demonPos[i];
		Cod drawDemonCod = drawGameFge->demonPoint[i][drawDemonPos<0? -drawDemonPos: drawDemonPos];
		SDL_Rect drawDemon = {
				drawDemonCod.x*24,
				drawDemonCod.y*24,
				24, 24
		};
		SDL_RenderCopy(drawRen, drawGameImg[4], nullptr, &drawDemon);
	}
}

void GameDraw::Show(bool ifFlipFge) {
	SDL_SetRenderDrawColor(
			drawRen,
			0xff, 0xfa,
			0xf0, 0xff
	);
	SDL_RenderClear(drawRen);
	//显示地图
	ShowMap();
	//显示人物
	SDL_Rect drawFge = {drawGameFge->fgeCod.x*24, drawGameFge->fgeCod.y*24, 24, 24};
	SDL_RenderCopyEx(
			drawRen,
			drawGameImg[-1],
			nullptr, &drawFge,
			0, nullptr,
			ifFlipFge? SDL_FLIP_HORIZONTAL: SDL_FLIP_NONE
	);
	//显示怪物
	ShowDemon();
	if(drawGameFge->ifWin == 1) ShowWin();
	else if(drawGameFge->ifWin == -1) ShowLose();
	SDL_RenderPresent(drawRen);
}

bool GameDraw::Ask(SDL_Keycode ifQuit) {
	if(ifQuit == SDLK_y) return true;
	else return false;
}

void GameDraw::ShowWin() {
	SetScreenDarken(0xff, 0xa0, 0x7a);
	std::string strWin = "You Win!";
	ShowEnd(strWin);
}

void GameDraw::ShowLose() {
	SetScreenDarken(0x19, 0x19, 0x70);
	std::string strLose = "You lose!";
	ShowEnd(strLose);
}

void GameDraw::SetScreenDarken(int r, int g, int b) {
	SDL_SetRenderDrawColor(
			drawRen,
			r, g, b, 0xcc
	);
	SDL_RenderFillRect(drawRen, &drawScreenRect);
}

void GameDraw::ShowEnd(const std::string& str) {
	ShowWord(
			str, 4,
			SCREEN_CENTER, SCREEN_CENTER-18*4
	);
	ShowWord(
			"Y: Play Again",
			2,
			SCREEN_CENTER, SCREEN_CENTER
	);
	ShowWord(
			"N: Quit",
			2,
			SCREEN_CENTER, SCREEN_CENTER+18*2
	);
}

void GameDraw::ChangeFge(GameFge *gameFge) {
	drawGameFge = gameFge;
}
