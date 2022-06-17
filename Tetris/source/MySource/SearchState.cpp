#include "SearchState.h"
#include "MinoMgr.h"
#include "Mino.h"
#include "AIMgr.h"
#include "MoveState.h"

using namespace Constant;
using namespace Enums;

SearchState::SearchState() :m_pCurMino(nullptr), m_point{0,0}
{
}

SearchState::~SearchState()
{
}


//�~�m�𐶐�������W����ɖ߂�
void SearchState::Update(MinoMgr* pObj)
{
	//�����邩���ׂ�DeleteSearch			����������������������������
	//switch
	//-------------------------------------------------------------------
	//�����邩���ׂ�DeleteSearch
	//�z�[���h�ŏ����邩���ׂ�
	//switch
	//-------------------------------------------------------------------
	//�����邩���ׂ�DeleteSearch
	//�z�[���h�ŏ����邩���ׂ�
	//���݂̃~�m�̍ō��]���𒲂ׂ�
	//�z�[���h�~�m�̍ō��]���𒲂ׂ�
	//���݂ƃz�[���h�̕]�����ׂĕ]�������������g�p����

	if (!DeleteSearch(pObj))
	{
		switch (pObj->GetCurMino()->GetMinoKind())
		{
		case I_MINO:
			YPieceImportant(pObj);
			break;

		case O_MINO:
			if (!OSearch(pObj))
			{
				YPieceImportant(pObj);
			}
			break;

		case S_MINO:
			ContactSearch(pObj);
			break;

		case Z_MINO:
			ContactSearch(pObj);
			break;

		case J_MINO:
			ContactSearch(pObj);
			break;

		case L_MINO:
			ContactSearch(pObj);
			break;

		case T_MINO:
			ContactSearch(pObj);
			break;
		}
	}
}

//���_Y�̍��W�d�� �v���g�^�C�v
void SearchState::YImportant(MinoMgr* pObj)
{
	//�j�󂳂ꂽ�|�C���^�Q�Ƃ�����
	m_pCurMino = pObj->GetCurMino();
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = pObj->GetSquware(i,j);
		}
	}
	int tetMino[4][4];
	bool isEnd = false;
	int cnt = 0;
	Angle angle = Angle::ANGLE_MAX;
	m_point[0] = -999;

	//�S�ẴA���O���𒲂ׂ�
	for (int h = 0; h < Angle::ANGLE_MAX; ++h)
	{
		m_pCurMino->Spin((Angle)h, pObj);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_pCurMino->GetTetMino(i, j);
			}
		}
		//�{�[�h�S�̂𒲂ׂ�
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4�̃~�m�̗̈�𒲂ׂ�
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//�g�O�͔�΂�
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//�s�[�X������Ƃ���̂݃T�[�`����
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//�s�[�X�S�Ƃ��N���A�Ȃ���W��ێ�
							++cnt;
							if (cnt > 3)
							{
								//�ł�Y���傫������
								if (m_point[0] < y)
								{
									angle = (Angle)h;
									m_point[0] = y;
									m_point[1] = x;
								}
							}
						}
					}
				}
			}
		}
	}

	
	MoveState* newState = new MoveState();
	newState->SetPoint(m_point[0], m_point[1]);
	newState->SetAngle(angle);
	AIMgr::GetInstance()->ChageState(newState, pObj);

}

//�ݒu����ꏊ��piece�̏�ɑ���piece������ΒT����蒼��
//�s�[�X���Y���W�d��
void SearchState::YPieceImportant(MinoMgr * pObj)
{
	m_pCurMino = pObj->GetCurMino();
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = pObj->GetSquware(i, j);
		}
	}
	int tetMino[4][4];
	int cnt = 0;
	int pieceCnt = 0;
	int pieceCntMax = 0;
	Angle angle = Angle::ANGLE_MAX;
	m_point[0] = -999;
	int maxY = 0;

	//�S�ẴA���O���𒲂ׂ�
	for (int h = 0; h < Angle::ANGLE_MAX; ++h)
	{
		m_pCurMino->Spin((Angle)h, pObj);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_pCurMino->GetTetMino(i, j);
			}
		}
		//�{�[�h�S�̂𒲂ׂ�
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4�̃~�m�̗̈�𒲂ׂ�
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//�g�O�͔�΂�
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -2) continue;
						//�s�[�X������Ƃ���̂݃T�[�`����
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//�s�[�X�S�Ƃ��N���A�Ȃ���W��ێ�
							++cnt;
						}
					}
					if (cnt > 3)
					{
						for (int m = 3; m > -1; --m)
						{
							pieceCnt = 0;

							for (int n = 0; n < 4; ++n)
							{
								if (tetMino[m][n] == 1)
								{
									//���W���O��Ɠ��������Ȃ炨����piece�̐�������ׂ�
									if (m + y == maxY)
									{
										if (pieceCnt > pieceCntMax)
										{
											pieceCntMax = pieceCnt;
											maxY = m + y;
											angle = (Angle)h;
											m_point[0] = y;
											m_point[1] = x;
										}
									}
									//���W���O���傫��������X�V����
									else if (m + y > maxY)
									{
										pieceCntMax = pieceCnt;
										maxY = m + y;
										angle = (Angle)h;
										m_point[0] = y;
										m_point[1] = x;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	MoveState* newState = new MoveState();
	newState->SetPoint(m_point[0], m_point[1]);
	newState->SetAngle(angle);
	AIMgr::GetInstance()->ChageState(newState, pObj);
	m_pCurMino = nullptr;
}

//�ݒu���ɐڐG���Ă���~�m�œ_�������ē_���������Ƃ���ɐݒu����B�_�����Ⴂ�ƃz�[���h����
void SearchState::ContactSearch(MinoMgr * pObj)
{
	m_pCurMino = pObj->GetCurMino();
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = pObj->GetSquware(i, j);
		}
	}
	int tetMino[4][4];
	int cnt = 0;
	int contactCnt = 0;
	int contactCntMax = 0;
	Angle angle = Angle::ANGLE_MAX;
	m_point[0] = -999;
	int maxY = 0;
	bool isCheck = false;

	//�S�ẴA���O���𒲂ׂ�
	for (int h = 0; h < Angle::ANGLE_MAX; ++h)
	{
		m_pCurMino->Spin((Angle)h, pObj);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_pCurMino->GetTetMino(i, j);
			}
		}
		//�{�[�h�S�̂𒲂ׂ�
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				isCheck = false;
				//4*4�̃~�m�̗̈�𒲂ׂ�
				for (int k = 0; k < 4 && !isCheck; ++k)
				{
					for (int l = 0; l < 4 && !isCheck; ++l)
					{
						//�g�O�͔�΂�
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//�s�[�X������Ƃ���̂݃T�[�`����
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								isCheck = true;
								continue;
							}
							//�s�[�X�S�Ƃ��N���A�Ȃ���W��ێ�
							++cnt;
						}

						if (cnt > 3)
						{
							contactCnt = 0;
							for (int m = 3; m > -1; --m)
							{
								for (int n = 3; n > -1; --n)
								{
									if (tetMino[m][n] == 1)
									{
										//if (m_Squware[y + m - 1][x + n] == SquwareState::Stop)	contactCnt += 2;	//��
										if (m_Squware[y + m + 1][x + n] == SquwareState::Stop)//��
										{
											contactCnt += 2;
										}
										if (m_Squware[y + m][x + n - 1] == SquwareState::Stop)		//��
										{
											if (x + n - 1 != 0)
											{
												contactCnt += 2;
											}
											else
											{
												++contactCnt;
											}
										}
										if (m_Squware[y + m][x + n + 1] == SquwareState::Stop)		//�E
										{
											if (x + n + 1 != BOARD_WIDTH - 1)
											{
												contactCnt += 2;
											}
											else
											{
												++contactCnt;
											}
										}
									}
								}
							}
							if (contactCnt > contactCntMax)
							{
								contactCntMax = contactCnt;
								angle = (Angle)h;
								m_point[0] = y;
								m_point[1] = x;
							}
						}
					}
				}
			}
		}
	}

	
	if (contactCntMax < 7)
	{
		if (pObj->HoldAI())
		{
			AIMgr::GetInstance()->ChageState(new SearchState(), pObj);
			m_pCurMino = nullptr;
			return;
		}
	}
	
	MoveState* newState = new MoveState();
	newState->SetPoint(m_point[0], m_point[1]);
	newState->SetAngle(angle);
	AIMgr::GetInstance()->ChageState(newState, pObj);
	m_pCurMino = nullptr;
}

//��������̂�T���B�Ȃ�ׂ��Ⴂ���W��piece���z�u�ł���悤�ɐݒ肷��
bool SearchState::DeleteSearch(MinoMgr * pObj)
{

	m_pCurMino = pObj->GetCurMino();
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = pObj->GetSquware(i, j);
		}
	}
	int tetMino[4][4];
	int cnt = 0;
	int pieceCnt = 0;
	int deleteLineCnt = 0;
	int deleteLineCntMax = 0;
	Angle angle = Angle::ANGLE_MAX;
	m_point[0] = -999;
	int maxY = 0;

	//�S�ẴA���O���𒲂ׂ�
	for (int h = 0; h < Angle::ANGLE_MAX; ++h)
	{
		m_pCurMino->Spin((Angle)h, pObj);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_pCurMino->GetTetMino(i, j);
			}
		}
		//�{�[�h�S�̂𒲂ׂ�
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4�̃~�m�̗̈�𒲂ׂ�
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//�g�O�͔�΂�
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//�s�[�X������Ƃ���̂݃T�[�`����
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//�s�[�X�S�Ƃ��N���A�Ȃ���W��ێ�
							++cnt;
						}
					}
					if (cnt > 3)
					{
						//-----�������P
						//��d���[�v������̂�
						//���W�̒Ⴂpiece�̍��W���m�ۂ���
						//��荂�x��AI�ɂ���
						for (int m = 3; m > -1; --m)
						{
							for (int n = 0; n < 4; ++n)
							{
								if (tetMino[m][n] == 1)
								{
									m_Squware[y + m][x + n] = SquwareState::TENTATIVE;
								}
							}
						}
						deleteLineCnt = 0;
						//������TENTATIVE+STOP��10�}�X����΍��W�擾
						for (int o = 3; o > -1; --o)
						{
							if (y + o >= BOARD_HEIGHT - 1) continue;
							pieceCnt = 0;
							for (int p = 1; p < BOARD_WIDTH - 1; ++p)
							{
								if (m_Squware[y + o][p] == SquwareState::TENTATIVE)
								{
									++pieceCnt;
									//���ɖ߂��Ă���
									m_Squware[y + o][p] = SquwareState::Non;
								}
								if (m_Squware[y + o][p] == SquwareState::Stop)
								{
									++pieceCnt;
								}
							}
							if (pieceCnt == 10)
							{
								++deleteLineCnt;
							}
						}

						if (deleteLineCnt > deleteLineCntMax)
						{
							angle = (Angle)h;
							m_point[0] = y;
							m_point[1] = x;
							deleteLineCntMax = deleteLineCnt;
						}
					}
				}
			}
		}
	}
	if (deleteLineCntMax > 0)
	{
		MoveState* newState = new MoveState();
		newState->SetPoint(m_point[0], m_point[1]);
		newState->SetAngle(angle);
		AIMgr::GetInstance()->ChageState(newState, pObj);
		m_pCurMino = nullptr;
		return true;
	}
	return false;
}

//O�~�m��p�̃T�[�`�֐����
bool SearchState::OSearch(MinoMgr * pObj)
{
	m_pCurMino = pObj->GetCurMino();
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = pObj->GetSquware(i, j);
		}
	}
	int tetMino[4][4];
	int cnt = 0;
	bool isEnd = false;
	m_point[0] = -999;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			tetMino[i][j] = m_pCurMino->GetTetMino(i, j);
		}
	}

		//�{�[�h�S�̂𒲂ׂ�
	for (int y = BOARD_HEIGHT - 2; y > 2 && !isEnd; --y)
	{
		for (int x = BOARD_WIDTH - 2; x > -2 && !isEnd; --x)
		{
			cnt = 0;
			//4*4�̃~�m�̗̈�𒲂ׂ�
			for (int k = 0; k < 4 && !isEnd; ++k)
			{
				for (int l = 0; l < 4 && !isEnd; ++l)
				{
					//�g�O�͔�΂�
					if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
					if ((x + l) >= (BOARD_WIDTH - 1)) continue;
					if ((x + l) <= -2) continue;
					//�s�[�X������Ƃ���̂݃T�[�`����
					if (tetMino[k][l] == 1)
					{
						if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
						{
							break;
						}
						//�s�[�X�S�Ƃ��N���A�Ȃ���W��ێ�
						++cnt;
					}
				}
				if (cnt > 3)
				{
					if (y + 3 != BOARD_HEIGHT - 1)
					{
						if (m_Squware[y + 3][x + 1] == SquwareState::Stop && m_Squware[y + 3][x + 2] == SquwareState::Stop)
						{
							m_point[0] = y;
							m_point[1] = x;
							isEnd = true;
							break;
						}
					}
					else
					{
						m_point[0] = y;
						m_point[1] = x;
						isEnd = true;
						break;
					}
				}
			}
		}
	}
	
	if (isEnd)
	{
		MoveState* newState = new MoveState();
		newState->SetPoint(m_point[0], m_point[1]);
		AIMgr::GetInstance()->ChageState(newState, pObj);
		m_pCurMino = nullptr;
		return true;
	}
	else if (pObj->HoldAI())
	{
		AIMgr::GetInstance()->ChageState(new SearchState(), pObj);
		m_pCurMino = nullptr;
		return true;
	}
	return false;
}

