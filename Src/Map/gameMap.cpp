#include"hMap.h"

GameMap::GameMap() :
		mapCoin(100),
		mapStar(3),
		mapDemon(3),
		xLength(47),
		yLength(29) {
	std::srand(time(nullptr));
	
	demonPoint = new CodList[mapDemon]();
	
	mapMaze = new int* [yLength];
	for(int i = 0; i < yLength; i++) {
		mapMaze[i] = new int[xLength];
		for(int j = 0; j < xLength; j++) mapMaze[i][j] = 0;
	}
	GetMaze();
	GetAllRoadPoint();
	CodList roadPointTemp = roadPoint;
	
	SetEnd();
	CodList pointStar;
	SetStar(pointStar);
	SetCoin();
	pointStar.push_back(mapEnd);
	SetDemon(pointStar, roadPointTemp);
}

GameMap::~GameMap() {
	for(int i = 0; i < yLength; i++) delete[] mapMaze[i];
	delete[] mapMaze;
	
	delete[] demonPoint;
}

bool GameMap::SelectVector(const CodList& vectorName, const Cod& point) {
	/*检测point是否存在于vectorName中*/
	
	return std::any_of(vectorName.begin(), vectorName.end(), [point](Cod cd) {
		return (cd.x == point.x && cd.y == point.y);
	});
}

Cod GameMap::MovePoint(
		const Cod& oldPoint,
		const int dir,
		int stepLength,
		bool ifSide
) const {
	/*移动点*/
	
	Cod cdMin{};
	Cod cdMax{};
	
	if(!ifSide) {
		cdMin = {0, 0};
		cdMax = {yLength, xLength};
	} else {
		cdMin = {-1, -1};
		cdMax = {yLength+1, xLength+1};
	}
	const int step[4][2] = {
			{-1,  0},//上
			{1 ,  0},//下
			{0 , -1},//左
			{0 ,  1} //右
	};
	Cod newPoint = {
			oldPoint.y + step[dir][0]*stepLength,
			oldPoint.x + step[dir][1]*stepLength
	};
	if(
//			newPoint.y > 0 && newPoint.y < yLength
//			&& newPoint.x > 0 && newPoint.x < xLength
			newPoint > cdMin && newPoint < cdMax
			) return newPoint;
	else return oldPoint;
}

void GameMap::GetMaze() {
	/*生成迷宫*/
	
	//初始化迷宫, 将迷宫每个点设置为0且加入起点, 起点恒为(1, 1)
	CodList wallPoint;
	mapMaze[1][1] = 1;
	Cod cdBegin = {1, 1};
	wallPoint.push_back(cdBegin);
	
	const int order[24][4] = {
			{0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3},
			{0, 2, 3, 1}, {0, 3, 2, 1}, {0, 3, 1, 2},
			{1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 3, 0},
			{1, 2, 0, 3}, {1, 3, 0, 2}, {1, 3, 2, 0},
			{2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 3, 0},
			{2, 1, 0, 3}, {2, 3, 1, 0}, {2, 3, 0, 1},
			{3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 2, 0},
			{3, 1, 0, 2}, {3, 2, 1, 0}, {3, 2, 0, 1}
	};
	
	Cod cd1{};
	Cod cd2{};
	Cod temp{};
	while(!wallPoint.empty()) {
		//在wallPoint中随随机选取一点作为cd1, 并删除
		int numRandom = std::rand()%(wallPoint.size());
		cd1 = wallPoint[numRandom];
		wallPoint.erase(wallPoint.begin() + numRandom);
		mapMaze[cd1.y][cd1.x] = 1;
		
		//将cd1与其四周随机一个路点cd2打通
		numRandom = std::rand()%24;
		for(int i = 0; i < 4; i++) {
			cd2 = MovePoint(cd1, order[numRandom][i]);
			if(SelectVector(roadPoint, cd2)) {
				mapMaze[(cd2.y + cd1.y)/2][(cd2.x + cd1.x)/2] = 1;
				break;
			}
		}
		
		//将cd1加入roadPoint
		roadPoint.push_back(cd1);
		
		//加入cd1周围不在wallPoint中的墙点
		for(int i = 0; i < 4; i++) {
			temp = MovePoint(cd1, i);
			if(
					mapMaze[temp.y][temp.x] == 0
					&& !SelectVector(wallPoint, temp)
					) wallPoint.push_back(temp);
		}
	}
}

void GameMap::GetAllRoadPoint() {
	for(int i=1; i<yLength; i++) {
		for(int j=i%2+1; j<xLength; j+=2) {
			if(mapMaze[i][j] != 0) roadPoint.push_back({i, j});
		}
	}
}

void GameMap::SetEnd() {
	int num = std::rand()%roadPoint.size();
	mapEnd = roadPoint[num];
	mapMaze[mapEnd.y][mapEnd.x] = -2;
	roadPoint.erase(roadPoint.begin() + num);
}

void GameMap::SetStar(CodList& pointStar) {
	for(int i = 0; i < mapStar;) {
		int num = std::rand()%roadPoint.size();
		Cod temp = roadPoint[num];
		roadPoint.erase(roadPoint.begin() + num);
		if(temp.x > 5 || temp.y > 5) {
			pointStar.push_back(temp);
			mapMaze[temp.y][temp.x] = 3;
			i++;
		}
	}
}

void GameMap::SetCoin() {
	for(int i = 0; i < mapCoin;) {
		int num = std::rand()%roadPoint.size();
		Cod temp = roadPoint[num];
		roadPoint.erase(roadPoint.begin() + num);
		if(temp.x > 2 || temp.y > 2) {
			mapMaze[temp.y][temp.x] = 2;
			i++;
		}
	}
}

void GameMap::SetDemon(int** maze, CodList& path) {
	for(int i=0; i<mapDemon;) {
		if(path.empty()) return;
		int num = std::rand()%path.size();
		if(GetDemonPath(maze, path[num], demonPoint[i], 6)) i++;
		path.erase(path.begin() + num);
	}
}

void GameMap::SetDemon(const CodList& cdEnd, CodList path) {
	//深拷贝mapMaze
	int** maze = new int* [yLength];
	for(int i = 0; i < yLength; i++) {
		maze[i] = new int[xLength];
		for(int j = 0; j < xLength; j++) {
			int temp = mapMaze[i][j];
			if(temp == 0) maze[i][j] = 0;
			else maze[i][j] = 1;
		}
	}
	
	CodList way;
	FillMaze(maze, {1, 1});
	for(auto i: cdEnd) ReFillMaze(maze, i, way);
	
	for(auto &i: way) {
		path.erase(std::remove_if(
				path.begin(), path.end(), [i](Cod point) {
					return (point.x == i.x && point.y == i.y);
				}), path.end());
	}
	SetDemon(maze, path);
	
	for(int i = 0; i < yLength; i++) delete[] maze[i];
	delete[] maze;
}

void GameMap::FillMaze(int** maze, const Cod& cdBegin, int num) {
	if(maze[cdBegin.y][cdBegin.x] != 1 || num == 3) return;
	for(int i = 0; i < 4; i++) {
		Cod cdTemp = MovePoint(cdBegin, i, 1);
		maze[cdBegin.y][cdBegin.x] = num;
		FillMaze(maze, cdTemp, num - 1);
	}
}

void GameMap::ReFillMaze(int** maze, const Cod& cdBegin, CodList& way) {
	int temp = maze[cdBegin.y][cdBegin.x];
	if(temp == 0 || temp == 1 || temp == 2) return;
	for(int i = 0; i < 4; i++) {
		Cod cdTemp = MovePoint(cdBegin, i, 1);
		if(temp + 1 == maze[cdTemp.y][cdTemp.x]) ReFillMaze(maze, cdTemp, way);
	}
	maze[cdBegin.y][cdBegin.x] = 2;
	way.push_back(cdBegin);
}

void GameMap::ClearMaze(int** maze, bool (* fun)(int)) {
	for(int i = 0; i < yLength; i++) {
		for(int j = 0; j < xLength; j++) {
			if(fun(maze[i][j])) maze[i][j] = 1;
		}
	}
}

bool GameMap::GetDemonPath(int** maze, const Cod& cd, CodList& path, int num) {
	if(cd.x == 1 && cd.y == 1) return false;
	
	ClearMaze(maze, [](int num) {
		return (num != 0 && num != 1);
	});
	
	FillMaze(maze, cd, num+3);

	CodList point;
	int iMax = (cd.y + 6 < yLength)? (cd.y + 6): (yLength - 1);
	int jMax = (cd.x + 6 < xLength)? (cd.x + 6): (xLength - 1);
	for(int i = (cd.y - 6 > 0)? (cd.y - 6): 1; i < iMax; i++) {
		for(int j = (cd.x - 6 > 0)? (cd.x - 6): 1; j < jMax; j++) {
			if(maze[i][j] == 4) point.push_back({i, j});
		}
	}
	if(point.empty()) return false;
	
	for(auto i: point) {
		if(!SelectVector(path, i) && !(i.x == 1 && i.y == 1)) {
			ReFillMaze(maze, i, path);
			return true;
		}
	}
	return false;
}