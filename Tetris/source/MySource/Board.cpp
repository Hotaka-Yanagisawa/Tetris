#include "Board.h"
#include "DxLib.h"

using namespace Constant;

Board::Board(int x) :m_BoardLeft(x)
{
	//ƒ{[ƒh‚Ì‰Šú‰»
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = SquwareState::Non;
			m_Cr[i][i] = GetColor(255, 0, 0);
		}
	}

	//ˆê”Ô‰º‚É“–‚½‚è”»’è—p‚Ì~‚Ü‚Á‚Ä‚éƒ~ƒm‚ğ”z’u
	for (int j = 0; j < BOARD_WIDTH; ++j)
	{
		m_Squware[BOARD_HEIGHT - 1][j] = SquwareState::Stop;
	}

	//—¼’[cˆê—ñ‚É“–‚½‚è”»’è—p‚Ì~‚Ü‚Á‚Ä‚¢‚éƒ~ƒm‚ğ”z’u
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		m_Squware[i][0] = SquwareState::Stop;
		m_Squware[i][BOARD_WIDTH - 1] = SquwareState::Stop;
	}
}

Board::~Board()
{
}


void Board::Draw()
{
	const int PIECESIZE = 20;

	//”Õ‚Ì˜g‚Ì•`‰æ
	for (int i = 3; i < BOARD_HEIGHT - 1; ++i)
	{
		for (int j = 1; j < BOARD_WIDTH - 1; ++j)
		{
			DrawBox(PIECESIZE * j + m_BoardLeft,
					PIECESIZE * i,
					PIECESIZE * j + PIECESIZE + m_BoardLeft,
					PIECESIZE * i + PIECESIZE,
					GetColor(122, 122, 122) , false);
		}
	}

	//ƒQ[ƒ€ƒI[ƒo[”»’è‚ÌêŠÔ˜g‚Å•\¦
	for (int j = 1; j < BOARD_WIDTH - 1; j++)
	{
		DrawBox(PIECESIZE * j + m_BoardLeft,
				60,
				PIECESIZE * j + PIECESIZE + m_BoardLeft,
				PIECESIZE + 60,
				GetColor(255, 0, 0), false);
	}

	//~‚Ü‚Á‚Ä‚¢‚éƒ~ƒm‚Ì•`‰æ
	for (int i = 3; i < BOARD_HEIGHT - 1; ++i)
	{
		for (int j = 1; j < BOARD_WIDTH - 1; ++j)
		{
			if (m_Squware[i][j] == SquwareState::Stop)
			{
				DrawBox(PIECESIZE * j + m_BoardLeft,
						PIECESIZE * i,
						PIECESIZE * j + m_BoardLeft + 18,
						PIECESIZE * i + 18,
						m_Cr[i][j], true);
			}
		}
	}
}
