#include"hGame.h"

Game::Game(const unsigned int fps) {
	gameFge = new GameFge();
	gameDraw = new GameDraw(fps, gameFge);
	gameRecord.SetTimeBegin();
}

Game::~Game() {
	delete gameDraw;
	delete gameFge;
}

void Game::UpdateMap() {
	delete gameFge;
	gameFge = new GameFge();
	gameDraw->ChangeFge(gameFge);
	gameRecord.ResetRecord();
	gameRecord.SetTimeBegin();
}

bool Game::CheckKeyEvent(const SDL_Keycode& key, bool& ifFlipFge) {
	switch(key) {
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			if(key == SDLK_LEFT) ifFlipFge = true;
			else if(key == SDLK_RIGHT) ifFlipFge = false;
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
	//是否翻转人物
	bool ifFlipFge = false;
	
	while(!ifQuit) {
		gameDraw->FpsManagerBegin();
		if(SDL_PollEvent(&drawEvent)) {
			if(drawEvent.type == SDL_QUIT) {
				ifQuit = true;
			} else if(drawEvent.type == SDL_KEYDOWN) {
				if(!CheckKeyEvent(drawEvent.key.keysym.sym, ifFlipFge)) return;
			}
		}
		gameFge->FgeIfWin();
		
		if(gameFge->ifWin != 0) {
			gameRecord.SetTimeEnd();
			gameRecord.RecordGame(gameFge->ifWin == 1, gameFge->fgeCoin);
			gameDraw->SetGameTime(gameRecord.GetGameTime());
		}
		
		gameDraw->Show(ifFlipFge);
		if(gameDraw->fpsNum%30 == 0) gameFge->DemonMove();
		gameDraw->FpsManagerEnd();
	}
}
