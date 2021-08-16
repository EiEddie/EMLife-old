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
			mazeInf.xLength * 24,
			mazeInf.yLength * 24,
			SDL_WINDOW_SHOWN
	);
	
	drawRen = SDL_CreateRenderer(
			drawWindow,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	SDL_SetRenderDrawBlendMode(drawRen, SDL_BLENDMODE_BLEND);
	
	std::string imgPath = "./Img/";
	SDL_Surface *drawGameTemp;
	
	std::map<int, std::string> drawGameImgName = {
			{-2, "end"},
			{-1, "fge"},
			{0, 	"wall"},
			{2, 	"coin"},
			{3, 	"star"},
			{4, 	"demon"}
	};
	for(int i = -2; i <= 4; i++) {
		if(i == 1) continue;
		drawGameTemp = IMG_Load((imgPath+drawGameImgName[i]+".png").c_str());
		drawGameImg.insert(std::pair<int, SDL_Texture *>(
				i, SDL_CreateTextureFromSurface(drawRen, drawGameTemp)
		));
	}
	
	std::map<int, std::string> drawWallImgName = {
			{0, 	"wallStr"},
			{1, 	"wallCor"},
			{2, 	"wallEnd"},
			{3, 	"wallCros"},
			{4, 	"wallTri"}
	};
	for(int i = 0; i <= 4; i++) {
		drawGameTemp = IMG_Load((imgPath+drawWallImgName[i]+".png").c_str());
		drawWallImg.insert(std::pair<int, SDL_Texture *>(
				i, SDL_CreateTextureFromSurface(drawRen, drawGameTemp)
		));
	}
	
	SDL_FreeSurface(drawGameTemp);
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
