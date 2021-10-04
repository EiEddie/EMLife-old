#include"hRecord.h"

GameInput::GameInput(const std::string& path) {
	jsonRead = new std::unique_ptr<Json::CharReader>(readBuilder.newCharReader());
	
	std::string json = GetSetting(path);
	(*jsonRead)->parse(json.c_str(), json.c_str() + json.length(), &root, &errs);
	
	mazeInf = new MazeInf(
			root["maze"]["length"].asInt(),
			root["maze"]["width"].asInt(),
			root["game"]["coin"].asInt(),
			root["game"]["star"].asInt(),
			root["game"]["demon"].asInt()
	);
}

GameInput::~GameInput() {
	delete mazeInf;
	delete jsonRead;
}

std::string GameInput::GetSetting(const std::string& path) {
	std::ifstream fileIn(path, std::ios::in);
	std::string text(
			(std::istreambuf_iterator<char>(fileIn)),
			(std::istreambuf_iterator<char>())
	);
	fileIn.close();
	return text;
}

void GameInput::SetMazeInf() {
	::mazeInf.coin = mazeInf->coin;
	::mazeInf.demon = mazeInf->demon;
	::mazeInf.star = mazeInf->star;
	
	::mazeInf.xLength = mazeInf->xLength;
	::mazeInf.yLength = mazeInf->yLength;
}
