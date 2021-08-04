#include"hDraw.h"

GameDrawSet::GameDrawSet(unsigned int fps, GameFge *gameFge) :
		stepFps(1000 / fps),
		drawGameFge(gameFge) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	
	drawWindow = SDL_CreateWindow(
			"EMLife!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			drawGameFge->xLength * 16,
			drawGameFge->yLength * 16,
			SDL_WINDOW_SHOWN
	);
	
	drawRen = SDL_CreateRenderer(
			drawWindow,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	SDL_SetRenderDrawBlendMode(drawRen, SDL_BLENDMODE_BLEND);
	
	std::map<int, std::string> drawGameImgName = {
			{-2, "End"},
			{-1, "Fge"},
			{0, 	"Wall"},
			{2, 	"Coin"},
			{3, 	"Star"},
			{4, 	"Demon"}
	};
	std::string imgPath = "./GameImg/imgGame";
	for(int i = -2; i <= 4; i++) {
		if(i == 1) continue;
		SDL_Surface *drawGameTemp = IMG_Load((imgPath+drawGameImgName[i]+".png").c_str());
		drawGameImg.insert(std::pair<int, SDL_Texture *>(
						i, SDL_CreateTextureFromSurface(drawRen, drawGameTemp)
				));
		SDL_FreeSurface(drawGameTemp);
	}
}

GameDrawSet::~GameDrawSet() {
	for(auto i: drawGameImg) {
		SDL_DestroyTexture(i.second);
	}
	SDL_DestroyRenderer(drawRen);
	SDL_DestroyWindow(drawWindow);
	IMG_Quit();
	SDL_Quit();
}
