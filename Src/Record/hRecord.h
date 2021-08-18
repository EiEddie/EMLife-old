#include"../hEMLife.h"

#ifndef EMLIFE_HRECORD_H
#define EMLIFE_HRECORD_H

#define TOTAL -1
#define LOSE 0
#define WIN 1

class GameRecord {
private:
	/*
	 * 数据库:
	 * 记录游戏数据
	 */
	sqlite3* gameDataBase;
	/*语句句柄*/
	sqlite3_stmt* stmt;
	/*执行语句*/
	void Execute(const std::string& sentence);
	
	/*游戏开始时间*/
	Uint32 timeBegin{};
	/*游戏结束时间*/
	Uint32 timeEnd{};
	/*
	 * 是否记录:
	 * 游戏结束时间是否已被记录
	 */
	bool ifRecordTime;
	
	/*获胜数目*/
	int numWin;
	/*总数*/
	int numTotal;
	/*获取胜负数目*/
	void SetResultNum();

public:
	void SetTimeBegin();
	void SetTimeEnd();
	
	/*记录一条游戏信息*/
	void RecordGame(bool ifWin);
	
	GameRecord();
	~GameRecord();
};

#endif //EMLIFE_HRECORD_H
