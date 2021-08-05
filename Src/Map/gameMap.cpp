#include"hMap.h"

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
	GetMaze();
//	RetouchMaze();
	FillMaze({1, 1});
	ReFillMaze({31, 39});
	ReFillMaze({1, 39});
	ClearMaze([](int num) {
		return num != 0 && num != 2;
	});
}

GameMap::~GameMap() {
	for(int i = 0; i < yLength; i++) delete[] mapMaze[i];
	delete[] mapMaze;
}

void GameMap::JoinVector(std::vector<Cod> &vectorName, const Cod &point) {
	/*将数组point加入vectorName中*/
	
	vectorName.push_back(point);
}

bool GameMap::SelectVector(const std::vector<Cod> &vectorName, const Cod &point) {
	/*检测数组point是否存在于vectorName中*/
	
	for(auto i: vectorName) {
		if(
				i.y == point.y
				&& i.x == point.x
				)
			return true;
	}
	return false;
}

Cod GameMap::MovePoint(
		const Cod &oldPoint,
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
	Cod newPoint = {
			oldPoint.y + step[dir][0] * stepLength,
			oldPoint.x + step[dir][1] * stepLength
	};
	if(
			newPoint.y > 0 && newPoint.y < yLength
			&& newPoint.x > 0 && newPoint.x < xLength
			)
		return newPoint;
	else return oldPoint;
}

void GameMap::GetMaze() {
	/*生成迷宫*/
	
	//初始化迷宫, 将迷宫每个点设置为0且加入起点, 起点恒为(1, 1)
	std::vector<Cod> wallPoint;
	std::vector<Cod> roadPoint;
	mapMaze[1][1] = 1;
	Cod cdBegin = {1, 1};
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
	Cod cd1{};
	Cod cd2{};
	Cod temp{};
	std::srand(time(nullptr));
	while(!wallPoint.empty()) {
		int numRandom = std::rand() % (wallPoint.size());
		cd1 = {wallPoint[numRandom].y, wallPoint[numRandom].x};
		wallPoint.erase(wallPoint.begin() + numRandom);
		
		//将cd1与其四周随机一个路点cd2打通
		mapMaze[cd1.y][cd1.x] = 1;
		numRandom = std::rand() % 24;
		for(int i = 0; i < 4; i++) {
			cd2 = MovePoint(cd1, order[numRandom][i]);
			if(SelectVector(roadPoint, cd2)) {
				mapMaze[(cd2.y + cd1.y) / 2][(cd2.x + cd1.x) / 2] = 1;
				break;
			}
		}
		JoinVector(roadPoint, cd1);
		
		//加入cd1周围不在wallPoint中的墙点
		for(int i = 0; i < 4; i++) {
			temp = MovePoint(cd1, i);
			if(
					mapMaze[temp.y][temp.x] == 0
					&& !SelectVector(wallPoint, temp)
					) {
				JoinVector(wallPoint, temp);
			}
		}
	}
}

void GameMap::RetouchMaze() {
	/*修饰迷宫, 添加coin,star并指定终点*/
	
	//添加除入口外所有路点至roadPoint
	std::vector<Cod> roadPoint;
	Cod temp;
	for(int i = 0; i < yLength; i++) {
		for(int j = 0; j < xLength; j++) {
			if(mapMaze[i][j] != 0 && i != 1 && j != 1) {
				temp = {i, j};
				JoinVector(roadPoint, temp);
			}
		}
	}
	
	//添加指定数目的coin与star, 设定终点
	std::srand(time(nullptr));
	for(int i = 0; i <= mapCoin + mapStar + mapDemon; i++) {
		int numRand = std::rand() % roadPoint.size();
		if(!i) {
			mapEnd = {roadPoint[numRand].y, roadPoint[numRand].x};
			mapMaze[roadPoint[numRand].y][roadPoint[numRand].x] = -2;
		} else if(i <= mapCoin) {
			mapMaze[roadPoint[numRand].y][roadPoint[numRand].x] = 2;
		} else if(i <= mapCoin + mapStar) {
			mapMaze[roadPoint[numRand].y][roadPoint[numRand].x] = 3;
		} else if(i <= mapCoin + mapStar + mapDemon) {
			mapMaze[roadPoint[numRand].y][roadPoint[numRand].x] = 4;
		}
		roadPoint.erase(roadPoint.begin() + numRand);
	}
}

void GameMap::FillMaze(Cod cdBegin, int num) {
	if(mapMaze[cdBegin.y][cdBegin.x] != 1) return;
	for(int i=0; i<4; i++) {
		Cod cdTemp = MovePoint(cdBegin, i, 1);
		mapMaze[cdBegin.y][cdBegin.x] = num;
		FillMaze(cdTemp, num-1);
	}
}

void GameMap::ReFillMaze(Cod cdBegin) {
	int temp = mapMaze[cdBegin.y][cdBegin.x];
	if(temp == 0 || temp == 1 || temp == 2) return;
	for(int i = 0; i < 4; i++) {
		Cod cdTemp = MovePoint(cdBegin, i, 1);
		if(temp + 1 == mapMaze[cdTemp.y][cdTemp.x]) ReFillMaze(cdTemp);
	}
	mapMaze[cdBegin.y][cdBegin.x] = 2;
}

void GameMap::ClearMaze(bool (*fun)(int)) {
	for(int i = 0; i < yLength; i++) {
		for(int j = 0; j < xLength; j++) {
			if(fun(mapMaze[i][j])) mapMaze[i][j] = 1;
		}
	}
}
