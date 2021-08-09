#include"hGame.h"

Game::Game(const unsigned int fps) {
	gameFge = new GameFge();
	gameDraw = new GameDraw(fps, gameFge);
}

Game::~Game() {
	delete gameDraw;
	delete gameFge;
}

void Game::UpdateMap() {
	delete gameFge;
	gameFge = new GameFge();
	gameDraw->ChangeFge(gameFge);
}

bool Game::CheckKeyEvent(const SDL_Keycode &key) {
	switch(key) {
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			if(gameFge->ifWin == 0) gameFge->FgeBehave(key);
			//继续显示
			return true;
		case SDLK_y:
		case SDLK_n:
			if(gameFge->ifWin != 0) {
				if(gameDraw->Ask(key)) {
					UpdateMap();
					//继续显示
					return true;
				}
				//关闭窗口
				else return false;
			}
		default:
			//继续显示
			return true;
	}
}

void Game::EventLoop() {
	//是否退出
	bool ifQuit = false;
	//事件
	SDL_Event drawEvent;
	
	while(!ifQuit) {
		gameDraw->FpsManagerBegin();
		if(SDL_PollEvent(&drawEvent)) {
			if(drawEvent.type == SDL_QUIT) {
				ifQuit = true;
			} else if(drawEvent.type == SDL_KEYDOWN) {
				if(!CheckKeyEvent(drawEvent.key.keysym.sym)) return;
			}
		}
		gameFge->FgeIfWin();
		gameDraw->Show();
		if(gameDraw->fpsNum%30 == 0) gameFge->DemonMove();
		gameDraw->FpsManagerEnd();
	}
}