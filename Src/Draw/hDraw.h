#include"../hEMLife.h"

#ifndef EMLIFE_HDRAW_H
#define EMLIFE_HDRAW_H

/*游戏渲染设置*/
class GameDrawSet {
protected:
	/*
	 * 人物:
	 * 地图数据&游戏动作
	 */
	GameFge *drawGameFge = nullptr;
	/*窗口*/
	SDL_Window *drawWindow = nullptr;
	/*渲染器*/
	SDL_Renderer *drawRen = nullptr;
	/*
	 * 单帧持续时间:
	 * 即周期,帧率的倒数(ms)
	 */
	const unsigned int stepFps;
	
	/*
	 * 各元素图像:
	 * End: -2, Fge: -1
	 * Wall: 0, Coin: 2
	 * Star: 3, Demon: 4
	 */
	std::map<int, SDL_Texture*> drawGameImg;

public:
	GameDrawSet(unsigned int fps, GameFge *gameFge);
	~GameDrawSet();
};


/*存放字体中单个字符数据*/
struct Font {
	/*字符材质*/
	SDL_Texture *chara;
	/*字符宽度*/
	unsigned int charaWidth;
	/*字符高度*/
	unsigned int charaHeight;
};


/*显示文字*/
class GameDrawWord: protected GameDrawSet {
private:
	/*
	 * 所有字符:
	 * 94个基本可见ASCII字符(\u0021: '!' 到 \u007e: '~', 不包括Space(空格))
	 */
	std::map<char, Font> drawChara;
	
	/*
	 * 获取字符宽度:
	 * cod[0]为该字符字面框左边线在16*16单元格内距左边框距离(px)
	 * cod[1]为该字符字面框右边线在16*16单元格内距左边框距离(px)
	 * cod[1]-cod[0]+1即为字符宽度
	 */
	void GetCharaWidth(SDL_Surface* chara, int width[2]);
	
	/*
	 * 一次裁剪:
	 * 将字符从font.png中剪裁为16*16的单元
	 */
	SDL_Surface *CropCharaFirst(SDL_Surface* font, SDL_Rect *cod);
	
	/*
	 * 二次裁剪:
	 * 将字符按字面框从单元中剪裁出来
	 */
	Font CropCharaSecond(SDL_Surface *chara);

protected:
	/*
	 * 在屏幕上显示语句:
	 * amp为放大倍率
	 */
	void ShowWord(const std::string &str, float amp=1, int x=0, int y=0);
	/*
	 * 获取语句长度(px):
	 * amp为放大倍率
	 */
	unsigned int GetStrLength(const std::string &str, float amp=1);
	/*
	 * 获取语句宽度(px)
	 * amp为放大倍率
	 */
	unsigned int GetStrWidth(const std::string &str, float amp=1);

public:
	GameDrawWord(unsigned int fps, GameFge *gameFge);
	~GameDrawWord();
};


/*显示游戏图像*/
class GameDraw: private GameDrawWord {
private:
	/*
	 * 帧率调节:
	 * 单帧开始时间
	 */
	Uint32 timeBegin{};
	/*
	 * 屏幕绘制矩形:
	 * 与窗口显示大小等长宽的矩形
	 */
	SDL_Rect drawScreenRect;
	
	/*
	 * 绘制地图:
	 * 绘制迷宫及其内部元素
	 */
	void SetMap();
	/*
	 * 使窗口变暗:
	 * 令窗口以指定颜色半透明
	 */
	void SetScreenDarken(int r, int g, int b);
	
	/*
	 * 显示结束页面:
	 * 胜利或失败
	 */
	void ShowEnd(const std::string& str);
	/*显示获胜页面*/
	void ShowWin();
	/*显示失败页面*/
	void ShowLose();

public:
	/*
	 * 帧率调节器:
	 * 开始
	 */
	void FpsManagerBegin();
	/*
	 * 帧率调节器:
	 * 结束
	 */
	void FpsManagerEnd() const;
	/*
	 * 显示:
	 * 显示游戏所有元素并刷新窗口
	 */
	void Show();
	/*
	 * 是否开始新一局:
	 * Y以开始新一局
	 * N以退出
	 */
	bool Ask(SDL_Keycode i);
	/*
	 * 更改人物:
	 * 更改渲染所使用的地图及人物
	 */
	void ChangeFge(GameFge *gameFge);
	
	GameDraw(unsigned int fps, GameFge *gameFge);
};

#endif //EMLIFE_HDRAW_H
