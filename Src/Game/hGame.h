#include"../hEMLife.h"
#ifndef EMLIFE_HGAME_H
#define EMLIFE_HGAME_H

class Game {
private:
	/** \brief 记录游戏数据 */
	GameRecord gameRecord;
	
	/** \brief 游戏人物 */
	GameFge* gameFge;
	/** \brief 显示游戏 刷新屏幕 */
	GameDraw* gameDraw;
	
	/**
	 * \brief 按键监视
	 *
	 * \param key 按键键码
	 * \return 是否继续显示
	 */
	bool CheckKeyEvent(const SDL_Keycode& key);
	
	/**
	 * \brief 更新地图
	 *
	 *   仅用于游戏结束时
	 */
	void UpdateMap();

public:
	/**
	 * \brief 事件循环
	 */
	void EventLoop();
	
	/**
	 * \param fps 帧率
	 */
	Game(unsigned int fps);
	~Game();
};

#endif //EMLIFE_HGAME_H
