#include <EMLife/EMLife.h>

MazeInf mazeInf(47, 29, 100, 3, 3);

GameInput::GameInput(const std::string& path) {
	SetRoot(path);
	inf = new MazeInf(
			root["maze"].get("length", 47).asInt(),
			root["maze"].get("width", 29).asInt(),
			root["game"].get("coin", 100).asInt(),
			root["game"].get("star", 3).asInt(),
			root["game"].get("demon", 3).asInt()
	);
}

GameInput::~GameInput() {
	delete inf;
}

void GameInput::SetRoot(const std::string& path) {
	std::ifstream fileIn(path, std::ifstream::binary);
	fileIn >> root;
	fileIn.close();
}

void GameInput::SetMazeInf() {
	::mazeInf.coin = inf->coin;
	::mazeInf.demon = inf->demon;
	::mazeInf.star = inf->star;
	
	::mazeInf.xLength = inf->xLength;
	::mazeInf.yLength = inf->yLength;
}
