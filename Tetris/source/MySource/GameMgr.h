#pragma once

#include "Score.h"

class GameMgr
{
public:
	static GameMgr* GetInstance()
	{
		static GameMgr instance;
		return &instance;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();

	Score* GetScore(int idx =0) { return &m_score[idx]; }

	bool GetGame()const { return m_isGame; }
	void SetGame(bool game) { m_isGame = game; }
	void SetBreakLine(int idx, int num) { m_breakLine[idx] = num; }
	void AddBreakLine(int idx, int add) { m_breakLine[idx] += add; }
	int GetBreakLine(int idx)const { return m_breakLine[idx]; }

private:
	GameMgr():m_time(0),m_isGame(true){}
	~GameMgr(){}

	Score m_score[2];
	bool m_isGame;
	float m_time;
	int m_tex;
	int m_point[2];
	int m_breakLine[2];
};
