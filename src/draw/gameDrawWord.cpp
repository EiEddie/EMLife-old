#include <EMLife/EMLife.h>

GameDrawWord::GameDrawWord(unsigned int fps, GameFge *gameFge):
		GameDrawSet(fps, gameFge) {
	SDL_Surface* font = IMG_Load("./assets/img/font.png");
	charaWidth = font->w/16;
	charaHeight = font->h/6;
	
	/// \brief 开始符号: '!'
	char charaNum = '!';
	/**
	 * \brief 字符charaNum在Font图片中的位置
	 *
	 *   第一行第一个为Space(空格)，略过
	 *   最后一行最后一个为空，略过
	 */
	SDL_Rect charaCod = {16, 0, charaWidth, charaHeight};
	
	for(int i=0; i<6; i++) {
		for(int j=0; j<16; j++) {
			if((i == 5 && j == 15)||(i == 0 && j == 0)) continue;
			SDL_Surface* charaCropFirst = CropCharaFirst(font, &charaCod);
			Font charaCropSecond = CropCharaSecond(charaCropFirst);
			drawChara.insert(
					std::pair<char, Font>(
							charaNum, charaCropSecond
					));
			SDL_FreeSurface(charaCropFirst);
			charaCod.x += charaWidth;
			charaNum++;
		}
		charaCod.x = 0;
		charaCod.y += charaHeight;
	}
	
	SDL_FreeSurface(font);
}

GameDrawWord::~GameDrawWord() {
	for(auto i: drawChara) {
		SDL_DestroyTexture(i.second.chara);
	}
}

void GameDrawWord::ShowWord(const std::string &str, float amp, int x, int y) {
	if(x < 0) x += (int)((mazeInf.xLength*24-GetStrLength(str, amp))/2) - SCREEN_CENTER;
	if(y < 0) y += (int)((mazeInf.yLength*24-GetStrWidth(str, amp))/2) - SCREEN_CENTER;

	SDL_Rect drawCharaCod = {x, y, 0, (int)(16*amp)};
	for(char i: str) {
		if(i <= '~' && i >= '!') {
			/**
			 * 如果是可见字符(\u0021: '!' 到 \u007e: '~', 不包括Space(空格)):
			 * 后移 该字符宽度+2px
			 */
			drawCharaCod.w = (int)((float)drawChara[i].width*amp);
			SDL_RenderCopy(drawRen, drawChara[i].chara, nullptr, &drawCharaCod);
			drawCharaCod.x += (int)((float)drawChara[i].width*amp);
			drawCharaCod.x += (int)(2*amp);
		} else if(i == '\n') {
			/**
			 * 如果是换行符(\u000a: '\n'):
			 * 下移 18px
			 */
			drawCharaCod.x = x;
			drawCharaCod.y += (int)(18*amp);
		} else if(i == ' ') {
			/**
			 * 如果是空格(\u0020: ' '):
			 * 后移 6px
			 */
			drawCharaCod.x += (int)(6*amp);
		}
	}
}

unsigned int GameDrawWord::GetStrLength(const std::string& str, float amp) {
	int length = 0;
	for(char i: str) {
		if(i <= '~' && i >= '!') {
			length += (int)((float)(drawChara[i].width)*amp);
			length += (int)(2*amp);
		}else if(i == ' ') {
			length += (int)(6*amp);
		}
	}
	return length-(int)(2*amp);
}

unsigned int GameDrawWord::GetStrWidth(const std::string& str, float amp) {
	int strWidth = (int)(18*amp);
	for(char i: str) {
		if(i == '\n') {
			strWidth += (int)(18*amp);
		}
	}
	return strWidth;
}

void GameDrawWord::GetCharaSize(SDL_Surface* chara, int width[2], int height[2]) const {
	int widthNum = 0;
	int heightNum = 0;
	bool ifWidthFirst = true;
	bool ifHeightFirst = true;
	Uint32* pixel = (Uint32*)chara->pixels;
	for(int i=0; i<charaHeight; i++) {
		for(int j=0; j<charaWidth; j++) {
			if(pixel[i+charaHeight*j] != 0x00000000) {
				widthNum = i;
				if(ifWidthFirst) width[0] = widthNum;
				ifWidthFirst = false;
			} if(pixel[j+charaWidth*i] != 0x00000000) {
				heightNum = i;
				if(ifHeightFirst) height[0] = heightNum;
				ifHeightFirst = false;
			}
		}
	}
	width[1] = widthNum;
	height[1] = heightNum;
}

SDL_Surface *GameDrawWord::CropCharaFirst(SDL_Surface* font, SDL_Rect* cod) {
	/**
	 * \brief 一次裁剪
	 *
	 *   将94个基本可见ASCII字符(/u0021: '!' 到 /u007e: '~', 不包括Space(空格))
	 *   从外部加载的Font图片中剪裁为16*16的单元
	 */
	
	///\brief 被裁剪下来的单个字符
	SDL_Surface* chara = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			charaWidth, charaHeight, 32,
			0xff000000, 0x00ff0000,
			0x0000ff00, 0x000000ff
	);
	SDL_UpperBlit(font, cod, chara, nullptr);
	return chara;
}

Font GameDrawWord::CropCharaSecond(SDL_Surface* chara) {
	///\brief 字符宽度信息
	int width[2] = {0};
	///\brief 字符高度信息
	int height[2] = {0};
	///\brief 裁剪后字符
	Font charaTex{};
	///\brief 临时存放字符
	SDL_Surface* charaCrop;
	///\brief 字符charaNum在Font图片中的位置
	SDL_Rect charaCod = {0, 0, charaWidth, charaHeight};
	
	GetCharaSize(chara, width, height);
	charaTex.width = width[1] - width[0] + 1;
	charaTex.height = height[1] - height[0] + 1;
	charaCod.x = width[0];
	charaCod.w = width[1]-width[0]+1;
	
	charaCrop = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			width[1]-width[0]+1, charaHeight, 32,
			0xff000000, 0x00ff0000,
			0x0000ff00, 0x000000ff
	);
	SDL_UpperBlit(chara, &charaCod, charaCrop, nullptr);
	
	charaTex.chara = SDL_CreateTextureFromSurface(drawRen, charaCrop);
	SDL_FreeSurface(charaCrop);
	return charaTex;
}
