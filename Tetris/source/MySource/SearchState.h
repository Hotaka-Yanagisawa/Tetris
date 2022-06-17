#pragma once

#include "AIStateBase.h"
//#include "Board.h"
#include "Constant.h"
#include "Enum.h"

class Mino;

class SearchState : public AIStateBase
{
public:
	SearchState();
	virtual ~SearchState();

	virtual void Update(MinoMgr*) override;

private:

	void YImportant(MinoMgr* pObj);
	void YPieceImportant(MinoMgr* pObj);
	void ContactSearch(MinoMgr* pObj);
	bool DeleteSearch(MinoMgr* pObj);
	bool OSearch(MinoMgr* pObj);

	Enums::SquwareState m_Squware[Constant::BOARD_HEIGHT][Constant::BOARD_WIDTH];
	Mino* m_pCurMino;
	int m_point[2];
};