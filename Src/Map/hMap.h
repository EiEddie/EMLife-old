#include"../hEMLife.h"

#ifndef EMLIFE_HMAP_H
#define EMLIFE_HMAP_H

/** \brief 位置 */
struct Cod {
	int y; /**< 纵坐标 */
	int x; /**< 横坐标 */
	
	bool operator ==(const Cod& cd) {
		if(cd.x == x && cd.y == y) return true;
		else return false;
	}
	bool operator !=(const Cod& cd) {
		return !(*this == cd);
	}
	
	bool operator <=(const Cod& cd) {
		if(x <= cd.x && y <= cd.y) return true;
		else return false;
	}
	bool operator >=(const Cod& cd) {
		if(x >= cd.x && y >= cd.y) return true;
		else return false;
	}
	
	bool operator <(const Cod& cd) {
		if(x < cd.x && y < cd.y) return true;
		else return false;
	}
	bool operator >(const Cod& cd) {
		if(x > cd.x && y > cd.y) return true;
		else return false;
	}
	
	/**
	 * \brief 移动点
	 *
	 * \param dir 移动方向
	 * \param stepLength 移动距离
	 */
	bool Move(const int dir, int stepLength=1);
};


/** \brief 迷宫信息 */
static struct MazeInf {
	const int xLength; /**< 迷宫宽度 */
	const int yLength; /**< 迷宫长度 */
	
	const int coin;    /**< 金币数量 */
	const int star;    /**< 星星数量 */
	const int demon;   /**< 恶魔数量 */
	
	
	MazeInf(
			int xLength, int yLength,
			int coinNum, int starNum, int demonNum
	):
		xLength(xLength),
		yLength(yLength),
		coin(coinNum),
		star(starNum),
		demon(demonNum) {}
} mazeInf(47, 29, 100, 3, 3);


typedef std::vector<Cod> CodList;


/** \brief 生成游戏地图 */
class GameMap {
private:
	CodList roadPoint; /**< 所有路点 */
	/**
	 * \brief 添加所有路点
	 */
	void GetAllRoadPoint();
	
	/**
	 * \brief 查找点
	 *
	 * \param vectorName 待查vector
	 * \param point 待查点
	 * \return 点是否在vector内
	 */
	bool SelectVector(const CodList& vectorName, const Cod& point);
	
	/**
	 * \brief 移动点
	 *
	 * \param point 被移动点
	 * \param dir 移动方向
	 * \param stepLength 移动距离
	 */
	Cod MovePoint(
			Cod point,
			const int dir,
			int stepLength=2
	) const;
	
	/** \brief 生成迷宫 */
	void GetMaze();
	
	/** \brief 指定终点 */
	void SetEnd();
	
	/** \brief 添加元素Coin */
	void SetCoin();
	
	/** \brief 添加元素Star */
	void SetStar(CodList& pointStar);
	
	/**
	 * \brief 添加元素Demon
	 *
	 * \param maze 迷宫
	 * \param path 可移动位置
	 */
	void SetDemon(int** maze, CodList& path);
	/**
	 * \brief 添加元素Demon
	 *
	 * \param cdEnd 终点列表
	 * \param path 可移动位置
	 */
	void SetDemon(const CodList& cdEnd, CodList path);
	
	/**
	 * \brief 填充迷宫
	 *
	 *   为 ReFillMaze 方法做铺垫
	 *
	 * \param maze 被填充迷宫
	 * \param cdBegin 填充起点
	 * \param num 以什么数字填充
	 */
	void FillMaze(int** maze, const Cod& cdBegin, int num=-1);
	
	/**
	 * \brief 清理迷宫
	 *
	 *   去除 FillMaze 方法痕迹
	 *
	 * \param maze 被清理迷宫
	 * \param fun num为迷宫内某点的数字
	 * 返回true时将此点更改为1
	 */
	void ClearMaze(int** maze, bool (*fun)(int num));
	
	/**
	 * \brief 二次填充
	 *
	 *   在一次填充的基础上反向进行以寻路
	 *
	 * \param maze 被填充迷宫
	 * \param cdBegin 填充起点
	 * \param way 返回的路径
	 */
	void ReFillMaze(int** maze, const Cod& cdBegin, CodList& way);
	
	/**
	 * \brief 寻找demon行动路径
	 *
	 * \param maze 迷宫
	 * \param cd 开始位置
	 * \param path 可移动位置
	 * \param num 步数
	 */
	bool GetDemonPath(int** maze, const Cod& cd, CodList& path, int num);

public:
	CodList* demonPoint; /**< Demon活动点 */
	
	int** mapMaze;       /**< 迷宫地图 */
	Cod mapEnd{};        /**< 迷宫终点 */
	
	GameMap();
	~GameMap();
};


/** \brief 创建游戏人物 */
class GameFge: public GameMap {
private:
	/** \brief 移动人物 */
	void FgeMove(SDL_Keycode dir);
	
	/**
	 * \brief 获取战利品
	 *
	 *   Coin&Star
	 */
	void FgeEat();

public:
	/**
	 * \brief Demon位置
	 *
	 *   长度为 mapDemon
	 *   取值 -3~4, 若为负数则以相反数为准
	 */
	int* demonPos;
	
	Cod fgeCod; /**< 人物位置 */
	
	int fgeCoin;       /**< 人物金币数量 */
	int fgeStar;       /**< 人物星星数量 */
	
	/**
	 * \brief 是否获得足够星星
	 *
	 *   人物星星数量是否等于游戏星星数量
	 */
	bool ifGetAllStar = false;
	
	/**
	 * \brief 是否获胜
	 *
	 *   -1: 失败(触碰到Demon)
	 *   0: 游戏中
	 *   1: 胜利(已获得足够星星并触碰到终点)
	 */
	int ifWin = 0;
	
	/** \brief 判断是否获胜 */
	void FgeIfWin();
	
	/** \brief 人物行为 */
	void FgeBehave(SDL_Keycode dir);
	
	/** \brief 移动怪物 */
	void DemonMove();
	
	GameFge();
	~GameFge();
};

#endif //EMLIFE_HMAP_H
