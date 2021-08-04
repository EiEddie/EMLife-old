#include"../hEMLife.h"

#ifndef EMLIFE_HMAP_H
#define EMLIFE_HMAP_H

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
	void JoinVector(std::vector<Cod> &vectorName, const Cod &point);
	
	/*查找点*/
	bool SelectVector(const std::vector<Cod> &vectorName, const Cod &point);
	
	/*移动点*/
	Cod MovePoint(
			const Cod &oldPoint,
			int dir,
			int stepLength = 2
	) const;
	
	/*生成迷宫*/
	void GetMaze(int **maze);
	
	/*
	 * 修饰迷宫:
	 * 添加元素(Coin, Star, Demon)并指定终点
	 */
	void RetouchMaze(int **maze);

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
	Cod mapEnd{};
	
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

#endif //EMLIFE_HMAP_H
