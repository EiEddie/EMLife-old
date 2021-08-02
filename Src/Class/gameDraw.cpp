#include"../headEML.h"

GameDraw::GameDraw(unsigned int fps, GameFge *gameFge):
		GameDrawWord(fps, gameFge),
		drawScreenRect({
			0, 0,
			drawGameFge->xLength*16,
			drawGameFge->yLength*16
		}) {}

void GameDraw::FpsManagerBegin() {
	timeBegin = SDL_GetTicks();
}

void GameDraw::FpsManagerEnd() const {
	Uint32 timeDelta = SDL_GetTicks() - timeBegin;
	if(timeDelta < stepFps) {
		SDL_Delay(stepFps - timeDelta);
	}
}

void GameDraw::SetMap() {
	SDL_Rect drawElementCod = {0, 0, 16, 16};
	for(int i=0; i<drawGameFge->yLength; i++) {
		for(int j = 0; j<drawGameFge->xLength; j++) {
			int temp = drawGameFge->mapMaze[i][j];
			if((temp == -2 && drawGameFge->ifGetAllStar) || (temp != 1 && temp != -2)) {
				drawElementCod.y = i*16;
				drawElementCod.x = j*16;
				SDL_RenderCopy(drawRen, drawGameImg[temp], nullptr, &drawElementCod);
			}
		}
	}
}

void GameDraw::Show() {
	SDL_SetRenderDrawColor(
			drawRen,
			0xff, 0xff,
			0xff, 0xff
	);
	SDL_RenderClear(drawRen);
	SetMap();
	SDL_Rect drawFgeCod = {drawGameFge->fgeCod.x*16, drawGameFge->fgeCod.y*16, 16, 16};
	SDL_RenderCopy(drawRen, drawGameImg[-1], nullptr, &drawFgeCod);
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
			SCREEN_CENTER,
			(int)((drawGameFge->yLength*16-GetStrWidth(str, 4))/2)-18*4
	);
	ShowWord(
			"Y: Play Again",
			2,
			SCREEN_CENTER,SCREEN_CENTER
	);
	ShowWord(
			"N: Quit",
			2,
			SCREEN_CENTER,
			(int)((drawGameFge->yLength*16-GetStrWidth(str, 2))/2)+18*2
	);
}

void GameDraw::ChangeFge(GameFge *gameFge) {
	drawGameFge = gameFge;
}
