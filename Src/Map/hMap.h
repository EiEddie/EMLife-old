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

typedef std::vector<Cod> CodList;

/*生成游戏地图*/
class GameMap {
private:
	/*所有路点*/
	CodList roadPoint;
	
	/*加入点*/
	void JoinVector(CodList &vectorName, const Cod &point);
	
	void JoinVector(CodList &vectorName, int point[2]);
	
	/*查找点*/
	bool SelectVector(const CodList &vectorName, const Cod &point);
	
	/*移动点*/
	Cod MovePoint(
			const Cod &oldPoint,
			int dir,
			int stepLength = 2
	) const;
	
	/*生成迷宫*/
	void GetMaze();
	
	/*指定终点*/
	void SetEnd();
	
	/*添加元素Coin*/
	void SetCoin();
	
	/*添加元素Star*/
	void SetStar(CodList &pointStar);
	
	/*添加元素Demon*/
	void SetDemon(int **maze, CodList &pointDemon);
	
	/*
	 * 填充迷宫:
	 * 为 ReFillMaze 方法做铺垫
	 */
	void FillMaze(int **maze, const Cod &cdBegin, int num = -1);
	
	/*
	 * 清理迷宫:
	 * 去除 FillMaze 方法痕迹
	 * num为迷宫内某点的数字
	 * 当fun返回true时将此点更改为1
	 */
	void ClearMaze(int **maze, bool (*fun)(int num));
	
	/*
	 * 二次填充:
	 * 在一次填充的基础上反向进行以寻路
	 */
	void ReFillMaze(int **maze, const Cod &cdBegin, CodList &way);
	
	/*寻路*/
	void SearchWay(
			int **maze,
			CodList &way,
			const Cod &cdBegin,
			const CodList &cdEnd
	);
	
	/*寻找demon行动路径*/
	bool GetDemonPath(int **maze, const Cod &cd, CodList &path, int num);

protected:
	/*金币数量*/
	const int mapCoin;
	/*星星数量*/
	const int mapStar;
	/*恶魔数量*/
	const int mapDemon;

public:
	/*Demon活动点*/
	CodList *demonPoint;
	
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
class GameFge : public GameMap {
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
