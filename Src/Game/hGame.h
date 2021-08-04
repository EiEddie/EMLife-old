#include"../hEMLife.h"

#ifndef EMLIFE_HGAME_H
#define EMLIFE_HGAME_H

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

#endif //EMLIFE_HGAME_H
