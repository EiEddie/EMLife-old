#include"hMap.h"

GameMap::GameMap() :
		mapCoin(100),
		mapStar(3),
		mapDemon(3),
		xLength(41),
		yLength(33) {
	std::srand(time(nullptr));
	
	demonPoint = new CodList[mapDemon]();
	
	mapMaze = new int* [yLength];
	for(int i = 0; i < yLength; i++) {
		mapMaze[i] = new int[xLength];
		for(int j = 0; j < xLength; j++) mapMaze[i][j] = 0;
	}
	GetMaze();
	SetEnd();
	CodList pointStar;
	SetStar(pointStar);
	SetCoin();
	
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
	for(auto i: pointStar) ReFillMaze(maze, i, way);
	ClearMaze(maze, [](int num) {
		return (num < 0);
	});
	
	SetDemon(maze, way);
	
	for(int i = 0; i < yLength; i++) delete[] maze[i];
	delete[] maze;

}

GameMap::~GameMap() {
	for(int i = 0; i < yLength; i++) delete[] mapMaze[i];
	delete[] mapMaze;
	
	delete[] demonPoint;
}

void GameMap::JoinVector(CodList& vectorName, const Cod& point) {
	/*将坐标point加入vectorName中*/
	
	vectorName.push_back(point);
}

void GameMap::JoinVector(CodList& vectorName, int point[2]) {
	/*将数组point加入vectorName中*/
	
	vectorName.push_back({point[0], point[1]});
}

bool GameMap::SelectVector(const CodList& vectorName, const Cod& point) {
	/*检测point是否存在于vectorName中*/
	
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
		const Cod& oldPoint,
		const int dir,
		int stepLength
) const {
	/*移动点*/
	const int step[4][2] = {
			{-1, 0},//上
			{1,  0},//下
			{0,  -1},//左
			{0,  1} //右
	};
	Cod newPoint = {
			oldPoint.y + step[dir][0]*stepLength,
			oldPoint.x + step[dir][1]*stepLength
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
	CodList wallPoint;
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
	while(!wallPoint.empty()) {
		int numRandom = std::rand()%(wallPoint.size());
		cd1 = {wallPoint[numRandom].y, wallPoint[numRandom].x};
		wallPoint.erase(wallPoint.begin() + numRandom);
		
		//将cd1与其四周随机一个路点cd2打通
		mapMaze[cd1.y][cd1.x] = 1;
		numRandom = std::rand()%24;
		for(int i = 0; i < 4; i++) {
			cd2 = MovePoint(cd1, order[numRandom][i]);
			if(SelectVector(roadPoint, cd2)) {
				mapMaze[(cd2.y + cd1.y)/2][(cd2.x + cd1.x)/2] = 1;
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

void GameMap::SetEnd() {
	int num = std::rand()%roadPoint.size();
	mapEnd = roadPoint[num];
	mapMaze[mapEnd.y][mapEnd.x] = -2;
	roadPoint.erase(roadPoint.begin() + num);
}

void GameMap::SetStar(CodList& pointStar) {
	for(int i = 0; i < mapStar; i++) {
		int num = std::rand()%roadPoint.size();
		Cod temp = roadPoint[num];
		pointStar.push_back(temp);
		mapMaze[temp.y][temp.x] = 3;
		roadPoint.erase(roadPoint.begin() + num);
	}
}

void GameMap::SetCoin() {
	for(int i = 0; i < mapCoin; i++) {
		int num = std::rand()%roadPoint.size();
		Cod temp = roadPoint[num];
		mapMaze[temp.y][temp.x] = 2;
		roadPoint.erase(roadPoint.begin() + num);
	}
}

void GameMap::SetDemon(int** maze, CodList& pointDemon) {
	for(int i=0; i<mapDemon;) {
		if(pointDemon.empty()) return;
		int num = std::rand()%pointDemon.size();
		if(GetDemonPath(maze, pointDemon[num], demonPoint[i], 5)) i++;
		pointDemon.erase(pointDemon.begin() + num);
	}
}

void GameMap::FillMaze(int** maze, const Cod& cdBegin, int num) {
	if(maze[cdBegin.y][cdBegin.x] != 1 || num == 2) return;
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
	FillMaze(maze, cd, num);
	CodList point;
	int iMax = (cd.y + 5 < yLength)? (cd.y + 5): (yLength - 1);
	int jMax = (cd.x + 5 < xLength)? (cd.x + 5): (xLength - 1);
	for(int i = (cd.y - 5 > 0)? (cd.y - 5): 1; i < iMax; i++) {
		for(int j = (cd.x - 5 > 0)? (cd.x - 5): 1; j < jMax; j++) {
			if(maze[i][j] == 3) point.push_back({i, j});
		}
	}
	if(point.empty()) return false;
	ReFillMaze(maze, point[std::rand()%point.size()], path);
	return true;
}