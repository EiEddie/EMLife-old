#pragma once
#include<vector>
#include<map>
#include<iostream>
#include<ctime>
extern "C" {
#include<SDL.h>
#include<SDL_image.h>
}

#define SCREEN_CENTER -1

/*位置*/
struct Cod {
	/*纵坐标*/
	int y;
	/*横坐标*/
	int x;
};


/*生成游戏地图*/
class GameMap {
private:
	/*加入点*/
	void JoinVector(std::vector<Cod> &vectorName, const int point[2]);
	
	/*查找点*/
	bool SelectVector(const std::vector<Cod> &vectorName, const int point[2]);
	
	/*移动点*/
	void MovePoint(
			int newPoint[2],
			const int oldPoint[2],
			int dir,
			int stepLength = 2
	) const;
	
	/*生成迷宫*/
	void GetMaze(int *maze[]);
	
	/*
	 * 修饰迷宫:
	 * 添加元素(Coin, Star, Demon)并指定终点
	 */
	void RetouchMaze(int mazeEnd[2], int *maze[]);

protected:
	/*金币数量*/
	const int mapCoin;
	/*星星数量*/
	const int mapStar;
	/*恶魔数量*/
	const int mapDemon;

public:
	/*迷宫宽度*/
	const int xLength;
	/*迷宫长度*/
	const int yLength;
	
	/*迷宫地图*/
	int **mapMaze;
	/*迷宫终点*/
	int mapEnd[2] = {0};
	
	GameMap();
	~GameMap();
};


/*创建游戏人物*/
class GameFge: public GameMap {
private:
	/*移动人物*/
	void FgeMove(SDL_Keycode dir);
	
	/*
	 * 获取战利品:
	 * Coin&Star
	 */
	void FgeEat();
	
	/*人物攻击*/
	void FgeAttack();

public:
	/*人物位置*/
	struct Cod fgeCod;
	/*人物金币数量*/
	int fgeCoin;
	/*人物星星数量*/
	int fgeStar;
	/*
	 * 是否获得足够星星:
	 * 人物星星数量是否等于游戏星星数量
	 */
	bool ifGetAllStar = false;
	/*
	 * 是否获胜:
	 * -1: 失败(触碰到Demon)
	 * 0: 游戏中
	 * 1: 胜利(已获得足够星星并触碰到终点)
	 */
	int ifWin = 0;
	
	/*人物行为*/
	void FgeBehave(SDL_Keycode dir);
	
	GameFge();
};


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


class Game {
private:
	/*游戏人物*/
	GameFge *gameFge;
	/*显示游戏 刷新屏幕*/
	GameDraw *gameDraw;
	/*
	 * 按键监视:
	 * true: 继续显示(刷新屏幕)
	 * false: 关闭窗口
	 */
	bool CheckKeyEvent(const SDL_Keycode& key);
	/*
	 * 更新地图:
	 * 仅用于游戏结束时
	 */
	void UpdateMap();

public:
	/*事件循环*/
	void EventLoop();
	
	Game(const unsigned int fps);
	~Game();
};