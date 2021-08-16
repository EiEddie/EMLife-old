#include"hDraw.h"

void WallDrawInf::SetPointInf(int xLength ,int yLength, int **maze, const Cod& point) {
	std::map<int, WallDrawInf> wallInf = {
			{0b0001, {2, 0}},
			{0b0010, {2, 180}},
			{0b0011, {0, 0}},
			{0b0100, {2, -90}},
			{0b0101, {1, 0}},
			{0b0110, {1, -90}},
			{0b0111, {4, -90}},
			{0b1000, {2, 90}},
			{0b1001, {1, 90}},
			{0b1010, {1, 180}},
			{0b1011, {4, 90}},
			{0b1100, {0, 90}},
			{0b1101, {4, 0}},
			{0b1110, {4, 180}},
			{0b1111, {3, 0}}
	};
	
	WallDrawInf temp = wallInf[GetPointNum(xLength, yLength, maze, point)];
	this->num = temp.num;
	this->angle = temp.angle;
}


int WallDrawInf::GetPointNum(int xLength, int yLength, int **maze, const Cod& point) {
	int pointInf = 0;
	for(int i=3; i>=0; i--) {
		Cod pointNew = MovePoint(point, i, xLength, yLength, 1);
		if(
				pointNew != point
				&& maze[pointNew.y][pointNew.x] == 0
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
			drawGameFge->xLength*24,
			drawGameFge->yLength*24
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

//void GameDraw::SetDrawMap() {
//	drawMap = SDL_CreateTexture(
//			drawRen,
//			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
//			drawGameFge->xLength*24, drawGameFge->yLength*24
//	);
//	SDL_SetRenderTarget(drawRen, drawMap);
//	std::map<int, SDL_Texture *> wall = {
//
//	};
//
//	//temp:
//	SDL_Texture *temp = SDL_CreateTextureFromSurface(drawRen, IMG_Load("./Img/wall.png"));
//
//	SDL_Rect drawWallCod = {0, 0, 24, 24};
//	for(int i=0; i<drawGameFge->yLength; i++) {
//		for(int j = 0; j<drawGameFge->xLength; j++) {
//			if(drawGameFge->mapMaze[i][j] == 0) {
//				drawWallCod.y = i*24;
//				drawWallCod.x = j*24;
//				SDL_RenderCopyEx(drawRen, temp, nullptr, &drawWallCod, 0, nullptr, SDL_FLIP_NONE);
//			}
//		}
//	}
////	drawMap = SDL_CreateTextureFromSurface(drawRen, mapSurface);
//
//	//temp:
//	SDL_DestroyTexture(temp);
//
//	SDL_SetRenderTarget(drawRen, nullptr);
//}

void GameDraw::ShowMap() {
	SDL_Rect drawElementCod = {0, 0, 24, 24};
	for(int i=0; i<drawGameFge->yLength; i++) {
		for(int j = 0; j<drawGameFge->xLength; j++) {
			int temp = drawGameFge->mapMaze[i][j];
			drawElementCod.y = i*24;
			drawElementCod.x = j*24;
			if(temp == 0) {
				WallDrawInf wallInf;
				wallInf.SetPointInf(drawGameFge->xLength, drawGameFge->yLength, drawGameFge->mapMaze, {i, j});
				
				SDL_RenderCopyEx(
						drawRen, drawWallImg[wallInf.num],
						nullptr, &drawElementCod,
						wallInf.angle, nullptr, wallInf.flip
				);
			} else if((temp == -2 && drawGameFge->ifGetAllStar) || (temp != 1 && temp != -2)) {
				SDL_RenderCopy(drawRen, drawGameImg[temp], nullptr, &drawElementCod);
			}
		}
	}
}

void GameDraw::ShowDemon() {
	for(int i=0; i<drawGameFge->mapDemon; i++) {
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
