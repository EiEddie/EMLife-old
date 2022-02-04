#include <EMLife/EMLife.h>

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

bool Game::CheckKeyEvent(const SDL_Keycode& key) {
	switch(key) {
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			if(key == SDLK_LEFT)
				gameDraw->ifFlipFge = true;
			else if(key == SDLK_RIGHT)
				gameDraw->ifFlipFge = false;
			
			if(gameFge->ifWin == 0)
				gameFge->FgeBehave(key);
			//继续显示
			
			return true;
		case SDLK_y:
		case SDLK_n:
			if(gameFge->ifWin != 0) {
				if(gameDraw->Ask(key)) {
					UpdateMap();
					//继续显示
					return true;
				} else { //关闭窗口
					return false;
				}
			}
		case SDLK_r:
			UpdateMap();
			//继续显示
			return true;
		default:
			//继续显示
			return true;
	}
}

void Game::EventLoop() {
	///\brief 是否退出
	bool ifQuit = false;
	///\brief 事件
	SDL_Event drawEvent;
	
	bool keyDown = false;
	
	while(!ifQuit) {
		gameDraw->FpsManagerBegin();

		if(SDL_PollEvent(&drawEvent))
			switch(drawEvent.type) {
			case SDL_QUIT:
				ifQuit = true;
				break;
			case SDL_KEYDOWN:
				if(!CheckKeyEvent(drawEvent.key.keysym.sym))
					ifQuit = true;
				break;
			}
		
		gameFge->FgeIfWin();
		
		if(gameFge->ifWin != 0) {
			gameRecord.SetTimeEnd();
			gameRecord.RecordGame(gameFge->ifWin == 1, gameFge->fgeCoin);
			gameDraw->SetGameTime(gameRecord.GetGameTime());
		}
		
		gameDraw->Show();
		if(gameDraw->fpsNum%30 == 0) gameFge->DemonMove();
		gameDraw->FpsManagerEnd();
	}
}
