#include"hDraw.h"

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

int GameDraw::GetPointInf(const Cod& point) {
	int pointInf = 0;
	for(int i=0; i<4; i++) {
		Cod pointNew = drawGameFge->MovePoint(point, i, 1, true);
		if(
				pointNew != point
				&& drawGameFge->mapMaze[pointNew.y][pointNew.x] == 0
				) {
			pointInf = pointInf | 0b1;
		}
		pointInf = pointInf<<1;
	}
	
	pointInf = pointInf>>1;
//	int temp = 0;
//	for(int i=0; i<4; i++) {
//		int a = 0b1000>>i;
//		int b = pointInf & a;
//		b = b>>(3-i);
//		temp = temp | b;
//		temp = temp<<1;
//	}
	int res = 0;
	for (int i = 0; i < 4; i++) {
		// 获取 n 的最低位加到 res 上
		res = (res << 1) + (pointInf & 1);
		// n 右移一位
		pointInf >>= 1;
	}
	return res;
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
				double angle = 0;
				auto flip = SDL_FLIP_NONE;
				int wallNum = 0;
				
				switch(GetPointInf({i, j})) {
					case 0b0001:
						wallNum = 2;
						break;
					case 0b0010:
						wallNum = 2;
						angle = 180;
						break;
					case 0b0011:
						wallNum = 0;
						break;
					case 0b0100:
						wallNum = 2;
						angle = -90;
						break;
					case 0b0101:
						wallNum = 1;
						break;
					case 0b0110:
						wallNum = 1;
						angle = -90;
						break;
					case 0b0111:
						wallNum = 4;
						angle = -90;
						break;
					case 0b1000:
						wallNum = 2;
						angle = 90;
						break;
					case 0b1001:
						wallNum = 1;
						angle = 90;
						break;
					case 0b1010:
						wallNum = 1;
						angle = 180;
						break;
					case 0b1011:
						wallNum = 4;
						angle = 90;
						break;
					case 0b1100:
						angle = 90;
						break;
					case 0b1101:
						wallNum = 4;
						break;
					case 0b1110:
						wallNum = 4;
						angle = 180;
						break;
					case 0b1111:
						wallNum = 3;
						break;
				}
				
				SDL_RenderCopyEx(
						drawRen, drawWallImg[wallNum],
						nullptr, &drawElementCod,
						angle, nullptr, flip
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
