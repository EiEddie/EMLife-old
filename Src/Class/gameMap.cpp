#include"../headEML.h"

GameMap::GameMap() :
		mapCoin(100),
		mapStar(3),
		mapDemon(3),
		xLength(41),
		yLength(33) {
	mapMaze = new int *[yLength];
	for(int i = 0; i < yLength; i++) {
		mapMaze[i] = new int[xLength];
		for(int j = 0; j < xLength; j++) mapMaze[i][j] = 0;
	}
	GetMaze(mapMaze);
	RetouchMaze(mapEnd, mapMaze);
}

GameMap::~GameMap() {
	for(int i = 0; i < yLength; i++) delete[] mapMaze[i];
	delete[] mapMaze;
}

void GameMap::JoinVector(std::vector<Cod> &vectorName, const int point[2]) {
	/*将数组point加入vectorName中*/
	
	Cod k{};
	k.y = point[0];
	k.x = point[1];
	vectorName.push_back(k);
}

bool GameMap::SelectVector(const std::vector<Cod> &vectorName, const int point[2]) {
	/*检测数组point是否存在于vectorName中*/
	
	for(auto i: vectorName) {
		if(
				i.y == point[0]
				&& i.x == point[1]
				)
			return true;
	}
	return false;
}

void GameMap::MovePoint(
		int newPoint[2],
		const int oldPoint[2],
		const int dir,
		int stepLength
) const {
	/*移动点*/
	const int step[4][2] = {
			{-1, 0 },//上
			{1 , 0 },//下
			{0 , -1},//左
			{0 , 1 } //右
	};
	newPoint[0] = oldPoint[0] + step[dir][0] * stepLength;
	newPoint[1] = oldPoint[1] + step[dir][1] * stepLength;
	if(
			newPoint[0] > 0 && newPoint[0] < yLength
			&& newPoint[1] > 0 && newPoint[1] < xLength
			)
		return;
	newPoint[0] = oldPoint[0];
	newPoint[1] = oldPoint[1];
}

void GameMap::GetMaze(int *maze[]) {
	/*生成迷宫*/
	
	//初始化迷宫, 将迷宫每个点设置为0且加入起点, 起点恒为(1, 1)
	std::vector<Cod> wallPoint;
	std::vector<Cod> roadPoint;
	maze[1][1] = 1;
	int cdBegin[2] = {1, 1};
	JoinVector(wallPoint, cdBegin);
	
	const int order[24][4] = {
			{0, 1, 2, 3},
			{0, 1, 3, 2},
			{0, 2, 1, 3},
			{0, 2, 3, 1},
			{0, 3, 2, 1},
			{0, 3, 1, 2},
			{1, 0, 2, 3},
			{1, 0, 3, 2},
			{1, 2, 3, 0},
			{1, 2, 0, 3},
			{1, 3, 0, 2},
			{1, 3, 2, 0},
			{2, 0, 1, 3},
			{2, 0, 3, 1},
			{2, 1, 3, 0},
			{2, 1, 0, 3},
			{2, 3, 1, 0},
			{2, 3, 0, 1},
			{3, 0, 1, 2},
			{3, 0, 2, 1},
			{3, 1, 2, 0},
			{3, 1, 0, 2},
			{3, 2, 1, 0},
			{3, 2, 0, 1}
	};
	
	//在wallPoint中随随机选取一点作为cd1, 并删除
	int cd1[2];
	int cd2[2];
	int temp[2];
	std::srand(time(nullptr));
	while(!wallPoint.empty()) {
		int numRandom = std::rand() % (wallPoint.size());
		cd1[0] = wallPoint[numRandom].y;
		cd1[1] = wallPoint[numRandom].x;
		wallPoint.erase(wallPoint.begin() + numRandom);
		
		//将cd1与其四周随机一个路点cd2打通
		maze[cd1[0]][cd1[1]] = 1;
		numRandom = std::rand() % 24;
		for(int i = 0; i < 4; i++) {
			MovePoint(cd2, cd1, order[numRandom][i]);
			if(SelectVector(roadPoint, cd2)) {
				maze[(cd2[0] + cd1[0]) / 2][(cd2[1] + cd1[1]) / 2] = 1;
				break;
			}
		}
		JoinVector(roadPoint, cd1);
		
		//加入cd1周围不在wallPoint中的墙点
		for(int i = 0; i < 4; i++) {
			MovePoint(temp, cd1, i);
			if(
					maze[temp[0]][temp[1]] == 0
					&& !SelectVector(wallPoint, temp)
					) {
				JoinVector(wallPoint, temp);
			}
		}
	}
}

void GameMap::RetouchMaze(int mazeEnd[2], int *maze[]) {
	/*修饰迷宫, 添加coin,star并指定终点*/
	
	//添加除入口外所有路点至roadPoint
	std::vector<Cod> roadPoint;
	int temp[2];
	for(int i = 0; i < yLength; i++) {
		for(int j = 0; j < xLength; j++) {
			if(maze[i][j] != 0 && i != 1 && j != 1) {
				temp[0] = i;
				temp[1] = j;
				JoinVector(roadPoint, temp);
			}
		}
	}
	
	//添加指定数目的coin与star, 设定终点
	std::srand(time(nullptr));
	for(int i = 0; i <= mapCoin + mapStar + mapDemon; i++) {
		int numRand = std::rand() % roadPoint.size();
		if(!i) {
			mazeEnd[0] = roadPoint[numRand].y;
			mazeEnd[1] = roadPoint[numRand].x;
			maze[roadPoint[numRand].y][roadPoint[numRand].x] = -2;
		} else if(i <= mapCoin) {
			maze[roadPoint[numRand].y][roadPoint[numRand].x] = 2;
		} else if(i <= mapCoin + mapStar) {
			maze[roadPoint[numRand].y][roadPoint[numRand].x] = 3;
		} else if(i <= mapCoin + mapStar + mapDemon) {
			maze[roadPoint[numRand].y][roadPoint[numRand].x] = 4;
		}
		roadPoint.erase(roadPoint.begin() + numRand);
	}
}
