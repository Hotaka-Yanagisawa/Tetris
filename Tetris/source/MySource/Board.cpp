#include "Board.h"
#include "DxLib.h"

using namespace Constant;

Board::Board(int x) :m_BoardLeft(x)
{
	//�{�[�h�̏�����
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = SquwareState::Non;
			m_Cr[i][i] = GetColor(255, 0, 0);
		}
	}

	//��ԉ��ɓ����蔻��p�̎~�܂��Ă�~�m��z�u
	for (int j = 0; j < BOARD_WIDTH; ++j)
	{
		m_Squware[BOARD_HEIGHT - 1][j] = SquwareState::Stop;
	}

	//���[�c���ɓ����蔻��p�̎~�܂��Ă���~�m��z�u
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

	//�Ղ̘g�̕`��
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

	//�Q�[���I�[�o�[����̏ꏊ�Ԙg�ŕ\��
	for (int j = 1; j < BOARD_WIDTH - 1; j++)
	{
		DrawBox(PIECESIZE * j + m_BoardLeft,
				60,
				PIECESIZE * j + PIECESIZE + m_BoardLeft,
				PIECESIZE + 60,
				GetColor(255, 0, 0), false);
	}

	//�~�܂��Ă���~�m�̕`��
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
