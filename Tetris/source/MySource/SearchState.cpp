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


//ミノを生成する座標を上に戻す
void SearchState::Update(MinoMgr* pObj)
{
	//消せるか調べるDeleteSearch			←←←←←←←←←←←←現在
	//switch
	//-------------------------------------------------------------------
	//消せるか調べるDeleteSearch
	//ホールドで消せるか調べる
	//switch
	//-------------------------------------------------------------------
	//消せるか調べるDeleteSearch
	//ホールドで消せるか調べる
	//現在のミノの最高評価を調べる
	//ホールドミノの最高評価を調べる
	//現在とホールドの評価を比べて評価が高い方を使用する

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

//原点Yの座標重視 プロトタイプ
void SearchState::YImportant(MinoMgr* pObj)
{
	//破壊されたポインタ参照しそう
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

	//全てのアングルを調べる
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
		//ボード全体を調べる
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4のミノの領域を調べる
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//枠外は飛ばす
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//ピースがあるところのみサーチする
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//ピース４つともクリアなら座標を保持
							++cnt;
							if (cnt > 3)
							{
								//最もYが大きいもの
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

//設置する場所のpieceの上に他のpieceがあれば探索やり直し
//ピース一つのY座標重視
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

	//全てのアングルを調べる
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
		//ボード全体を調べる
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4のミノの領域を調べる
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//枠外は飛ばす
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -2) continue;
						//ピースがあるところのみサーチする
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//ピース４つともクリアなら座標を保持
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
									//座標が前回と同じ高さならおけるpieceの数を見比べる
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
									//座標が前より大きかったら更新する
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

//設置時に接触しているミノで点数をつけて点数が高いところに設置する。点数が低いとホールドする
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

	//全てのアングルを調べる
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
		//ボード全体を調べる
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				isCheck = false;
				//4*4のミノの領域を調べる
				for (int k = 0; k < 4 && !isCheck; ++k)
				{
					for (int l = 0; l < 4 && !isCheck; ++l)
					{
						//枠外は飛ばす
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//ピースがあるところのみサーチする
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								isCheck = true;
								continue;
							}
							//ピース４つともクリアなら座標を保持
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
										//if (m_Squware[y + m - 1][x + n] == SquwareState::Stop)	contactCnt += 2;	//上
										if (m_Squware[y + m + 1][x + n] == SquwareState::Stop)//下
										{
											contactCnt += 2;
										}
										if (m_Squware[y + m][x + n - 1] == SquwareState::Stop)		//左
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
										if (m_Squware[y + m][x + n + 1] == SquwareState::Stop)		//右
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

//消せるものを探す。なるべく低い座標にpieceが配置できるように設定する
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

	//全てのアングルを調べる
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
		//ボード全体を調べる
		for (int y = BOARD_HEIGHT - 2; y > 2; --y)
		{
			for (int x = BOARD_WIDTH - 2; x > -2; --x)
			{
				cnt = 0;
				//4*4のミノの領域を調べる
				for (int k = 0; k < 4; ++k)
				{
					for (int l = 0; l < 4; ++l)
					{
						//枠外は飛ばす
						if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
						if ((x + l) >= (BOARD_WIDTH - 1)) continue;
						if ((x + l) <= -1) continue;
						//ピースがあるところのみサーチする
						if (tetMino[k][l] == 1)
						{
							if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
							{
								break;
							}
							//ピース４つともクリアなら座標を保持
							++cnt;
						}
					}
					if (cnt > 3)
					{
						//-----ここ改善
						//二重ループ一個消すのと
						//座標の低いpieceの座標を確保して
						//より高度なAIにする
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
						//ここでTENTATIVE+STOPで10マスあれば座標取得
						for (int o = 3; o > -1; --o)
						{
							if (y + o >= BOARD_HEIGHT - 1) continue;
							pieceCnt = 0;
							for (int p = 1; p < BOARD_WIDTH - 1; ++p)
							{
								if (m_Squware[y + o][p] == SquwareState::TENTATIVE)
								{
									++pieceCnt;
									//元に戻しておく
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

//Oミノ専用のサーチ関数作る
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

		//ボード全体を調べる
	for (int y = BOARD_HEIGHT - 2; y > 2 && !isEnd; --y)
	{
		for (int x = BOARD_WIDTH - 2; x > -2 && !isEnd; --x)
		{
			cnt = 0;
			//4*4のミノの領域を調べる
			for (int k = 0; k < 4 && !isEnd; ++k)
			{
				for (int l = 0; l < 4 && !isEnd; ++l)
				{
					//枠外は飛ばす
					if ((y + k) >= (BOARD_HEIGHT - 1)) continue;
					if ((x + l) >= (BOARD_WIDTH - 1)) continue;
					if ((x + l) <= -2) continue;
					//ピースがあるところのみサーチする
					if (tetMino[k][l] == 1)
					{
						if (m_Squware[y + k][x + l] == SquwareState::Stop || m_Squware[y + k - 1][x + l] == SquwareState::Stop)
						{
							break;
						}
						//ピース４つともクリアなら座標を保持
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

