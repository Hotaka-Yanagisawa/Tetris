#pragma once
#include <memory>
//#include "Board.h"
#include "Combo.h"
#include "Mino.h"
#include "Enum.h"
#include "Constant.h"

using namespace Enums;

enum MinoKind;
class EffectMgr;
class Board;
class EnemyBoard;

class MinoMgr
{
public:
	MinoMgr(Actor actor, int player, std::shared_ptr<EffectMgr> Effect, std::shared_ptr<Board> Board)
	:m_curMino(nullptr)
	,m_actor(actor)
	,m_holdMino(nullptr)
	,m_createT(0)
	,m_fallT(0)
	,m_moveT(0)
	,m_nextMinoIdx(0)
	,m_sHandle(0)
	,m_isHold(false)
	,m_LineNum(0)
	,m_effectMgr(Effect)
	,m_player(player)
	,m_playerBoard(Board)
	{}
	~MinoMgr() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();


	void CreateNextMino(bool all = false);

	void SetSquware(int idx1, int idx2, SquwareState state)
	{
		m_Squware[idx1][idx2] = state;
	}
	void SetSquware(SquwareState Squware[Constant::BOARD_HEIGHT][Constant::BOARD_WIDTH])
	{
		for (int i = 0; i < Constant::BOARD_HEIGHT; i++)
		{
			for (int j = 0; j < Constant::BOARD_WIDTH; j++)
			{
				m_Squware[i][j] = Squware[i][j];
			}
		}
	}
	SquwareState GetSquware(int idx1, int idx2) const
	{
		//”ÍˆÍŠO‚ðÈ‚­
		return m_Squware[idx1][idx2];
	}

	Mino* GetCurMino() const { return m_curMino; }
	void LeftMove();
	void RightMove();
	void HardDrop();
	void Fall();
	bool HoldAI();
	static void SetPlayerNum(int num) { m_playerNum = num; }
private:

	SquwareState m_Squware[Constant::BOARD_HEIGHT][Constant::BOARD_WIDTH];
	Mino* m_curMino;
	Mino* m_holdMino;
	Mino* m_pNextMino[14];
	bool Hold();
	void LSpin();
	void RSpin();
	void LeftMove(int);
	void RightMove(int);
	void ForecastMino();
	void StopBlock();
	bool DeleteBlock();
	void CreateCurMino();
	void FallSpeedUp(bool key);
	void DrawPlayer();
	void DrawPlayerVS();
	void DrawEnemy();

	float m_createT;
	float m_fallT;
	float m_moveT;
	float m_deleteT;
	int m_nextMinoIdx;
	int m_sHandle;
	int m_LineNum;
	int m_ForecastPoint[2];
	int m_deleteCnt;
	int m_endIdx;
	int m_linePoint[2];
	int m_lineCr;
	int m_player;
	static int m_playerNum;
	bool m_isHold;
	bool m_isDelete;
	Actor m_actor;
	Mino m_mino;
	std::weak_ptr<EffectMgr> m_effectMgr;
	std::weak_ptr<Board> m_playerBoard;
	//std::weak_ptr<EnemyBoard> m_enemyBoard;
	Combo m_combo;

	//enum
	//{
	//	1
	//	2
	//};

	//list[max];
	//list[1]->addObject()
};
