#include"../hEMLife.h"

#ifndef EMLIFE_HDRAW_H
#define EMLIFE_HDRAW_H

#define SCREEN_CENTER -1073741824

struct WallInf {
	/**
	 * \brief 墙点种类
	 *
	 *   直: 0, 弯: 1
	 *   末端: 2, 十字: 3, 三岔: 4
	 */
	int num;
	/** \brief 旋转角度 */
	double angle;
	/** \brief 翻转状态 */
	SDL_RendererFlip flip;
	
	WallInf(int num=0, double angle=0, SDL_RendererFlip flip=SDL_FLIP_NONE):
			num(num),
			angle(angle),
			flip(flip) {}
};

/** \brief 绘制墙点信息 */
static struct WallDrawInf: public WallInf {
private:
	/**
	 * \brief 获取地图贴图上某点标识
	 *
	 * \param maze 迷宫
	 * \param point 此点坐标
	 * \return 标识
	 */
	int GetPointNum(int** maze, const Cod& point);
	
	/**
	 * \brief 墙的绘制信息
	 *
	 *   根据墙的标识确定绘制时的编号与旋转角度
	 */
	std::map<int, WallInf> wallInf = {
			{ 0b0001, { 2, 0   }},
			{ 0b0010, { 2, 180 }},
			{ 0b0011, { 0, 0   }},
			{ 0b0100, { 2, -90 }},
			{ 0b0101, { 1, 0   }},
			{ 0b0110, { 1, -90 }},
			{ 0b0111, { 4, -90 }},
			{ 0b1000, { 2, 90  }},
			{ 0b1001, { 1, 90  }},
			{ 0b1010, { 1, 180 }},
			{ 0b1011, { 4, 90  }},
			{ 0b1100, { 0, 90  }},
			{ 0b1101, { 4, 0   }},
			{ 0b1110, { 4, 180 }},
			{ 0b1111, { 3, 0   }}
	};

public:
	/**
	 * \brief 设置地图贴图上某点绘制信息
	 * \param maze 迷宫
	 * \param point 此点坐标
	 */
	void SetPointInf(int** maze, const Cod& point);
	
	WallDrawInf(int num=0, double angle=0, SDL_RendererFlip flip=SDL_FLIP_NONE):
			WallInf(num, angle, flip) {}
} wallDrawInf;


/** \brief 游戏渲染设置 */
class GameDrawSet {
protected:
	/**
	 * \brief 人物
	 *
	 *   地图数据&游戏动作
	 */
	GameFge* drawGameFge = nullptr;
	/** \brief 窗口 */
	SDL_Window* drawWindow = nullptr;
	/** \brief 渲染器 */
	SDL_Renderer* drawRen = nullptr;

	/**
	 * \brief 单帧持续时间
	 *
	 *   即周期,帧率的倒数(ms)
	 */
	const unsigned int stepFps;
	
	/**
	 * \brief 各元素图像
	 *
	 *   End: -2, Fge: -1
	 *   Wall: 0, Coin: 2
	 *   Star: 3, Demon: 4
	 */
	std::map<int, SDL_Texture*> drawGameImg;
	/**
	 * \brief 墙的各种状态图像
	 *
	 *   直: 0, 弯: 1
	 *   末端: 2, 十字: 3, 三岔: 4
	 */
	std::map<int, SDL_Texture*> drawWallImg;

public:
	/**
	 * \param fps 帧率
	 * \param gameFge 显示的人物类
	 */
	GameDrawSet(unsigned int fps, GameFge* gameFge);
	~GameDrawSet();
};


/** \brief 存放字体中单个字符数据 */
struct Font {
	/** \brief 字符材质 */
	SDL_Texture* chara;
	/** \brief 字符宽度 */
	unsigned int width;
	/** \brief 字符高度 */
	unsigned int height;
	
	Font(SDL_Texture* chara=nullptr, unsigned int width=16, unsigned int height=16):
		chara(chara),
		width(width),
		height(height) {}
};


/** \brief 显示文字 */
class GameDrawWord: protected GameDrawSet {
private:
	/** \brief 字符宽度 */
	int charaWidth;
	/** \brief 字符高度 */
	int charaHeight;
	
	/**
	 * \brief 所有字符
	 *
	 *   94个基本可见ASCII字符(/u0021: '!' 到 /u007e: '~', 不包括Space(空格))
	 */
	std::map<char, Font> drawChara;
	
	/**
	 * \brief 获取字符尺寸
	 *
	 *   width[0]为该字符字面框左边线在16*16单元格内距左边框距离(px)
	 *   width[1]为该字符字面框右边线在16*16单元格内距左边框距离(px)
	 *   width[1]-width[0]+1即为字符宽度
	 *   字符高度同理
	 *
	 * \param chara 字符单元
	 * \param width 宽度信息
	 * \param height 高度信息
	 */
	void GetCharaSize(SDL_Surface* chara, int width[2], int height[2]) const;
	
	/**
	 * \brief 一次裁剪
	 *
	 *   将字符从font.png中剪裁为16*16的单元
	 *
	 * \param font 位图字体
	 * \param cod 裁剪位置
	 */
	SDL_Surface* CropCharaFirst(SDL_Surface* font, SDL_Rect* cod);
	
	/**
	 * \brief 二次裁剪
	 *
	 *   将字符按字面框从单元中剪裁出来
	 *
	 * \param chara 字符单元
	 * \return 字符数据
	 */
	Font CropCharaSecond(SDL_Surface* chara);

protected:
	/**
	 * \brief 在屏幕上显示语句
	 *
	 * \param str 语句
	 * \param amp 放大倍率
	 * \param x, y 显示位置
	 */
	void ShowWord(const std::string& str, float amp=1, int x=0, int y=0);

	/**
	 * \brief 获取语句长度
	 *
	 * \param str 语句
	 * \param amp 放大倍率
	 * \return 语句的显示长度(px)
	 */
	unsigned int GetStrLength(const std::string& str, float amp=1);
	/**
	 * \brief 获取语句宽度
	 *
	 * \param str 语句
	 * \param amp 放大倍率
	 * \return 语句的显示宽度(px)
	 */
	unsigned int GetStrWidth(const std::string& str, float amp=1);

public:
	/**
	 * \param fps 帧率
	 * \param gameFge 显示的人物类
	 */
	GameDrawWord(unsigned int fps, GameFge* gameFge);
	~GameDrawWord();
};


/** \brief 显示游戏图像 */
class GameDraw: private GameDrawWord {
private:
	/**
	 * \brief 帧率调节
	 *
	 *   单帧开始时间
	 */
	Uint32 timeBegin{};
	
	/**
	 * \brief 屏幕绘制矩形
	 *
	 *   与窗口显示大小等长宽的矩形
	 */
	SDL_Rect drawScreenRect;
	
	/** \brief 游戏时长 */
	unsigned int gameTime;
	
	/**
	 * \brief 绘制地图
	 *
	 *   绘制迷宫及其内部元素
	 */
	void ShowMap();
	
	/**
	 * \brief 绘制怪物
	 */
	void ShowDemon();
	
	/**
	 * \brief 使窗口变暗
	 *
	 *   令窗口以指定颜色半透明化
	 *
	 * \param r, g, b 颜色分量
	 */
	void SetScreenDarken(int r, int g, int b);
	
	/**
	 * \brief 显示结束页面
	 *
	 * \param str 结束时显示在屏幕上的语句
	 */
	void ShowEnd(const std::string& str);
	/**
	 * \brief 显示获胜页面
	 */
	void ShowWin();
	/**
	 * \brief 显示失败页面
	 */
	void ShowLose();

public:
	/**
	 * \brief 当前帧
	 *
	 *   当前帧的序号
	 */
	int fpsNum;
	/** \brief 是否翻转人物 */
	bool ifFlipFge;
	
	/**
	 * \brief 帧率调节器
	 *
	 *   开始
	 */
	void FpsManagerBegin();
	/**
	 * \brief 帧率调节器
	 *
	 *   结束
	 */
	void FpsManagerEnd() const;
	
	/**
	 * \brief 显示
	 *
	 *   显示游戏所有元素并刷新窗口
	 */
	void Show();
	
	/**
	 * \brief 游戏结束后检测按键以判断是否开始新一局
	 *
	 *   Y: 开始新一局
	 *   N: 退出
	 *
	 * \param key 按键键码
	 * \return 是否开始新一局
	 */
	bool Ask(SDL_Keycode key);
	
	/**
	 * \brief 更改人物
	 *
	 *   更改渲染所使用的地图及人物
	 *
	 * \param gameFge 替换的人物类
	 */
	void ChangeFge(GameFge* gameFge);
	
	/**
	 * \brief 设置游戏时长
	 *
	 * \param time 游戏时长
	 */
	void SetGameTime(unsigned int time);
	
	/**
	 * \param fps 帧率
	 * \param gameFge 显示的人物类
	 */
	GameDraw(unsigned int fps, GameFge* gameFge);
};

#endif //EMLIFE_HDRAW_H
