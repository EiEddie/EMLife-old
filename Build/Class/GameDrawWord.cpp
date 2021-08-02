#include"../headEML.h"

GameDrawWord::GameDrawWord(unsigned int fps, GameFge *gameFge) :
		GameDrawSet(fps, gameFge) {
	SDL_Surface *font = IMG_Load("./GameImg/font.png");
	
	//开始符号: '!'
	char charaNum = '!';
	/*
	 * 字符charaNum在Font图片中的位置
	 * 第一行第一个为Space(空格)，略过
	 * 最后一行最后一个为空，略过
	 */
	SDL_Rect charaCod = {16, 0, 16, 16};
	
	for(int i = 0; i < 6; i++) {
		for(int j = 0; j < 16; j++) {
			if((i == 5 && j == 15)||(i == 0 && j == 0)) continue;
			SDL_Surface *charaCropFirst = CropCharaFirst(font, &charaCod);
			Font charaCropSecond = CropCharaSecond(charaCropFirst);
			drawChara.insert(
					std::pair<char, Font>(
							charaNum, charaCropSecond
					));
			SDL_FreeSurface(charaCropFirst);
			charaCod.x += 16;
			charaNum++;
		}
		charaCod.x = 0;
		charaCod.y += 16;
	}
	
	SDL_FreeSurface(font);
}

GameDrawWord::~GameDrawWord() {
	for(auto i: drawChara) {
		SDL_DestroyTexture(i.second.chara);
	}
}

void GameDrawWord::ShowWord(const std::string &str, float amp, int x, int y) {
	if(x == SCREEN_CENTER) x = (int)((drawGameFge->xLength*16-GetStrLength(str, amp))/2);
	if(y == SCREEN_CENTER) y = (int)((drawGameFge->yLength*16-GetStrWidth(str, amp))/2);
	SDL_Rect drawCharaCod = {x, y, 0, (int)(16*amp)};
	for(char i: str) {
		if(i <= '~' && i >= '!') {
			/*
			 * 如果是可见字符(\u0021: '!' 到 \u007e: '~', 不包括Space(空格)):
			 * 后移 该字符宽度+2px
			 */
			drawCharaCod.w = (int)((float)drawChara[i].charaWidth*amp);
			SDL_RenderCopy(drawRen, drawChara[i].chara, nullptr, &drawCharaCod);
			drawCharaCod.x += (int)((float)drawChara[i].charaWidth*amp);
			drawCharaCod.x += (int)(2*amp);
		} else if(i == '\n') {
			/*
			 * 如果是换行符(\u000a: '\n'):
			 * 下移 18px
			 */
			drawCharaCod.x = x;
			drawCharaCod.y += (int)(18*amp);
		} else if(i == ' ') {
			/*
			 * 如果是空格(\u0020: ' '):
			 * 后移 6px
			 */
			drawCharaCod.x += (int)(6*amp);
		}
	}
}

unsigned int GameDrawWord::GetStrLength(const std::string &str, float amp) {
	int length = 0;
	for(char i: str) {
		if(i <= '~' && i >= '!') {
			length += (int)((float)(drawChara[i].charaWidth)*amp);
			length += (int)(2*amp);
		}else if(i == ' ') {
			length += (int)(6*amp);
		}
	}
	return length-(int)(2*amp);
}

unsigned int GameDrawWord::GetStrWidth(const std::string &str, float amp) {
	int strWidth = (int)(18*amp);
	for(char i: str) {
		if(i == '\n') {
			strWidth += (int)(18*amp);
		}
	}
	return strWidth;
}

void GameDrawWord::GetCharaWidth(SDL_Surface *chara, int width[2]) {
	int num = 0;
	bool ifFirst = true;
	Uint32 *pixel = (Uint32 *)chara->pixels;
	for(int i=0; i<16; i++) {
		for(int j=0; j<16; j++) {
			if(pixel[i+16*j] != 0x00000000) {
				num = i;
				if(ifFirst) width[0] = num;
				ifFirst = false;
			}
		}
	}
	width[1] = num;
}

SDL_Surface *GameDrawWord::CropCharaFirst(SDL_Surface *font, SDL_Rect *cod) {
	/*
	 * 一次裁剪:
	 * 将94个基本可见ASCII字符(\u0021: '!' 到 \u007e: '~', 不包括Space(空格))
	 * 从外部加载的Font图片中剪裁为16*16的单元
	 */
	
	//被裁剪下来的单个字符
	SDL_Surface *chara = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			16, 16, 32,
			0xff000000, 0x00ff0000,
			0x0000ff00, 0x000000ff
	);
	SDL_UpperBlit(font, cod, chara, nullptr);
	return chara;
}

Font GameDrawWord::CropCharaSecond(SDL_Surface *chara) {
	/*
	 * 二次裁剪:
	 * 将字符按字面框从单元中剪裁出来
	 */
	
	//字符宽度信息
	int width[2] = {0};
	//裁剪后字符
	Font charaTex = {nullptr, 16, 16};
	//临时存放字符
	SDL_Surface *charaCrop = nullptr;
	//字符charaNum在Font图片中的位置
	SDL_Rect charaCod = {0, 0, 16, 16};
	
	GetCharaWidth(chara, width);
	charaTex.charaWidth = width[1]-width[0]+1;
	charaCod.x = width[0];
	charaCod.w = width[1]-width[0]+1;
	
	charaCrop = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			width[1]-width[0]+1, 16, 32,
			0xff000000, 0x00ff0000,
			0x0000ff00, 0x000000ff
	);
	SDL_UpperBlit(chara, &charaCod, charaCrop, nullptr);
	
	charaTex.chara = SDL_CreateTextureFromSurface(drawRen, charaCrop);
	SDL_FreeSurface(charaCrop);
	return charaTex;
}