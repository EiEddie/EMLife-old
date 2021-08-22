#include"hRecord.h"

GameRecord::GameRecord():
		ifRecordTime(false), ifRecordGame(false),
		gameDataBase(nullptr), stmt(nullptr) {
	sqlite3_open_v2(
			"gameData.db", &gameDataBase,
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
			nullptr
	);
	
	Execute(
			"CREATE TABLE gameData("
			"id           INT PRIMARY KEY     NOT NULL,"
			"time         INT                 NOT NULL,"
			"coin         INT                 NOT NULL"
			");"
	);
	
	Execute(
			"CREATE TABLE resultData("
			"type         TEXT PRIMARY KEY    NOT NULL,"
			"num          INT                 NOT NULL"
			");"
	);
	Execute("INSERT INTO resultData VALUES ('total', 0);");
	Execute("INSERT INTO resultData VALUES ('win', 0);");
	
	SetResultNum();
}

GameRecord::~GameRecord() {
	sqlite3_close_v2(gameDataBase);
}

void GameRecord::Execute(const std::string& command) {
	int result = sqlite3_prepare_v2(gameDataBase, command.c_str(), -1, &stmt, nullptr);
	if(result == SQLITE_OK) sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void GameRecord::SetResultNum() {
	sqlite3_prepare_v2(gameDataBase, "SELECT num FROM resultData" , -1, &stmt, nullptr);
	
	for(int i=0; sqlite3_step(stmt) == SQLITE_ROW; i++) {
		if(i == 0) numTotal = sqlite3_column_int(stmt, 0);
		else if(i == 1) numWin = sqlite3_column_int(stmt, 0);
	}
	
	sqlite3_finalize(stmt);
}

void GameRecord::SetTimeBegin() {
	timeBegin = SDL_GetTicks();
	ifRecordTime = false;
}

void GameRecord::SetTimeEnd() {
	if(!ifRecordTime) {
		timeEnd = SDL_GetTicks();
		ifRecordTime = true;
	}
}

void GameRecord::RecordGame(bool ifWin, int numCoin) {
	if(ifRecordGame) return;
	
	numTotal++;
	const std::string sentenceBase = "UPDATE resultData SET num=";
	std::string sentence = sentenceBase + std::to_string(numTotal);
	sentence += " WHERE type='total';";
	Execute(sentence);
	
	if(ifWin) {
		numWin++;
		sentence = sentenceBase + std::to_string(numWin);
		sentence += " WHERE type='win';";
		Execute(sentence);
		
		std::string sentenceBase1 = "INSERT INTO gameData VALUES (";
		Execute(
				sentenceBase1
				+ std::to_string(numWin) + ", "
				+ std::to_string(timeEnd-timeBegin) + ", "
				+ std::to_string(numCoin) + ");"
		);
	}
	
	ifRecordGame = true;
}

void GameRecord::ResetRecord() {
	ifRecordGame = false;
}

unsigned int GameRecord::GetGameTime() {
	return timeEnd-timeBegin;
}
