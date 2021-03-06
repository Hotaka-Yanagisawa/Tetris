#include "Board.h"
#include "DxLib.h"

using namespace Constant;

Board::Board(int x) :m_BoardLeft(x)
{
	//ボードの初期化
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = SquwareState::Non;
			m_Cr[i][i] = GetColor(255, 0, 0);
		}
	}

	//一番下に当たり判定用の止まってるミノを配置
	for (int j = 0; j < BOARD_WIDTH; ++j)
	{
		m_Squware[BOARD_HEIGHT - 1][j] = SquwareState::Stop;
	}

	//両端縦一列に当たり判定用の止まっているミノを配置
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

	//盤の枠の描画
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

	//ゲームオーバー判定の場所赤枠で表示
	for (int j = 1; j < BOARD_WIDTH - 1; j++)
	{
		DrawBox(PIECESIZE * j + m_BoardLeft,
				60,
				PIECESIZE * j + PIECESIZE + m_BoardLeft,
				PIECESIZE + 60,
				GetColor(255, 0, 0), false);
	}

	//止まっているミノの描画
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
