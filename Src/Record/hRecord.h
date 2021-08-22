#include"../hEMLife.h"

#ifndef EMLIFE_HRECORD_H
#define EMLIFE_HRECORD_H

#define TOTAL -1
#define LOSE   0
#define WIN    1

class GameRecord {
private:
	/**
	 * \brief 数据库
	 *
	 *   记录游戏数据
	 */
	sqlite3* gameDataBase;
	/** \brief 语句句柄 */
	sqlite3_stmt* stmt;
	/**
	 * \brief 执行命令
	 *
	 * \param command 命令
	 */
	void Execute(const std::string& command);
	
	/** \brief 游戏开始时间 */
	Uint32 timeBegin{};
	/** \brief 游戏结束时间 */
	Uint32 timeEnd{};
	/**
	 * \brief 是否记录
	 *
	 *   游戏结束时间是否已被记录
	 */
	bool ifRecordTime;
	
	/** \brief 获胜数目 */
	int numWin;
	/** \brief 总数 */
	int numTotal;
	/** \brief 获取胜负数目 */
	void SetResultNum();
	
	/**
	 * \brief 是否记录
	 *
	 *   游戏结果是否已被记录
	 */
	bool ifRecordGame;


public:
	/** \brief 设置开始时间 */
	void SetTimeBegin();
	/** \brief 设置结束时间 */
	void SetTimeEnd();
	/**
	 * \brief 获取本局游戏时间
	 *
	 * \return 游戏时间
	 */
	unsigned int GetGameTime();
	
	/**
	 * \brief 记录一条游戏信息
	 *
	 * \param ifWin 是否获胜
	 * \param numCoin 游戏中取得的金币数量
	 */
	void RecordGame(bool ifWin, int numCoin);
	/**
	 * \brief 重置记录
	 *
	 *   重置ifRecordGame为false
	 */
	void ResetRecord();
	
	GameRecord();
	~GameRecord();
};

#endif //EMLIFE_HRECORD_H
