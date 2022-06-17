#pragma once

#include "Constant.h"
#include "Enum.h"

using namespace Enums;

class Board
{
public:
	Board(int x);
	~Board();

	void Draw();

	void SetCr(int idx1, int idx2, int cr)
	{
		m_Cr[idx1][idx2] = cr;
	}
	int GetCr(int idx1, int idx2)
	{
		return m_Cr[idx1][idx2];
	}
	SquwareState GetSquware(int idx1, int idx2)const
	{
		return m_Squware[idx1][idx2];
	}
	void SetSquware(int idx1, int idx2, SquwareState state)
	{
		m_Squware[idx1][idx2] = state;
	}
private:
	SquwareState m_Squware[Constant::BOARD_HEIGHT][Constant::BOARD_WIDTH];
	int m_Cr[Constant::BOARD_HEIGHT][Constant::BOARD_WIDTH];
	int m_BoardLeft;
};