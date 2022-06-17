#include "MinoMgr.h"
#include "DxLib.h"
#include "input.h"
#include "SceneMgr.h"
#include <cstdlib>
#include "GameMgr.h"
#include "SearchState.h"
#include "AIMgr.h"
#include "EffectMgr.h"
#include "Constant.h"
#include "Board.h"

using namespace Constant;

int MinoMgr::m_playerNum = 0;

void MinoMgr::Init()
{
	//ボードの初期化
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			m_Squware[i][j] = SquwareState::Non;
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
	
	m_curMino = nullptr;
	m_holdMino = nullptr;
	m_sHandle = LoadSoundMem("Assets/System_2.mp3");
	m_isHold = true;

	//ミノの生成
	CreateNextMino(true);
	m_fallT = 0;
	m_moveT = 0;
	m_createT = 0;
	m_deleteT = 0;
	m_LineNum = 0;
	m_isDelete = false;
	m_deleteCnt = 0;
	m_endIdx = 0;
	m_combo.Init();
	if (m_actor == PLAYER)
	{
		m_linePoint[0] = 10;
		m_linePoint[1] = 450; 
		m_lineCr = GetColor(255, 0, 0);
		GameMgr::GetInstance()->GetScore()->SetPoint(10,410);
		GameMgr::GetInstance()->GetScore()->SetCr(GetColor(255, 0, 0));
	}
	else
	{
		m_linePoint[0] = 330;
		m_linePoint[1] = 450;
		m_lineCr = GetColor(0, 0, 255);
		m_combo.SetPoint(330, 430);
		m_combo.SetCr(GetColor(0, 0, 255));
		GameMgr::GetInstance()->GetScore(1)->SetPoint(330, 410);
		GameMgr::GetInstance()->GetScore(1)->SetCr(GetColor(0, 0, 255));
	}

	m_nextMinoIdx = 0;
	AIMgr::GetInstance()->SetAI(false);

	m_ForecastPoint[0] = 0;
	m_ForecastPoint[1] = 0;
}

void MinoMgr::Uninit()
{
	//現在のミノの解放
	if (m_curMino != nullptr)
	{
		delete m_curMino;
		m_curMino = nullptr;
	}
	//ホールドミノの解放
	if (m_holdMino != nullptr)
	{
		delete m_holdMino;
		m_holdMino = nullptr;
	}

	//Nextミノの解放
	for (int i = 0; i < 14; ++i)
	{
		delete m_pNextMino[i];
		m_pNextMino[i] = nullptr;
	}

	DeleteSoundMem(m_sHandle);
	m_LineNum = 0;
}

void MinoMgr::Update()
{
	if (DeleteBlock()) return;

	//一番上の止まっているミノがあればゲームオーバー
	for (int j = 1; j < BOARD_WIDTH - 1; ++j)
	{
		if (m_Squware[3][j] == SquwareState::Stop)
		{
			GameMgr::GetInstance()->SetGame(false);
		}
	}

	//現在のミノが存在しないならポインタを渡す
	if (m_curMino == nullptr)
	{
		CreateCurMino();
		return;
	}
	if (m_actor == ENEMY && m_player != 1)
	{
		AIMgr* AIPtr = AIMgr::GetInstance();
	
		if (AIPtr->GetAIState() == nullptr) AIPtr->SetAIState(new SearchState);
		
		if (m_curMino != nullptr)
		{
			AIPtr->GetAIState()->Update(this);
			m_fallT += 1.0f / 60.0f;
			//自由落下処理
			Fall();
			//落下予測地点計算
			ForecastMino();
		}
			return;
	}
	int keyA = CheckHitKey(KEY_INPUT_A )  || isButtonPress(XINPUT_BUTTON_DPAD_LEFT, m_player);
	int keyD = CheckHitKey(KEY_INPUT_D )  || isButtonPress(XINPUT_BUTTON_DPAD_RIGHT, m_player);
	int keyS = CheckHitKey(KEY_INPUT_S )  || isButtonPress(XINPUT_BUTTON_DPAD_DOWN, m_player);
	int keyW = isKeyTrigger(KEY_INPUT_W)  || isButtonTrigger(XINPUT_BUTTON_A, m_player);
	

	//---タイムの更新
	int level = m_LineNum / 10;
	if (level > 11) level = 11;
	float timeSpd = 130.0f - (float)level * 10.0f;

	m_fallT += 1.0f / timeSpd;

	if (Hold())	return;	//ホールドミノの処理を行う
	
	FallSpeedUp(keyS);
	
	if (keyW)
	{
		HardDrop();
		return;
	}
	m_moveT += 1.0f / 5.0f;

	//----入力移動処理
	if (m_moveT > 1)
	{
		//入力回転処理
		if (isKeyTrigger(KEY_INPUT_Q) || isButtonTrigger(XINPUT_BUTTON_Y, m_player))
		{
			LSpin();
		}
		else if (isKeyTrigger(KEY_INPUT_E) || isButtonTrigger(XINPUT_BUTTON_B, m_player))
		{
			RSpin();
		}
		else
		{
			LeftMove(keyA);
			RightMove(keyD);
		}
	}
	//自由落下処理
	Fall();
	//落下予測地点計算
	ForecastMino();
}

void MinoMgr::Draw()
{
	if (SceneMgr::GetInstance()->GetType() == GameType::Normal)
	{
		DrawPlayer();
	}
	else
	{
		if (m_actor == PLAYER)
			DrawPlayerVS();
		else
		{
			DrawEnemy();
		}
	}	
}

void MinoMgr::CreateNextMino(bool all)
{

	int num = -999;	//乱数用
	int cnt = 0;
	bool isEnd = false;

	if (!all)
	{	
		//被りがないかチェックするための配列
		MinoKind kinds[MinoKind::MAX_MINO] = { MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO,
											   MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO ,MinoKind::MAX_MINO };
		//もし7の時と０の時を作る
		if (m_nextMinoIdx == 7)
		{
			while (!isEnd)
			{
				num = rand() % 7;
				//被りがないか確認して生成する
				for (int i = 0; i < (int)MinoKind::MAX_MINO; ++i)
				{
					if (kinds[i] == num) break;
					if (kinds[i] == MinoKind::MAX_MINO)
					{
						kinds[i] = (MinoKind)num;
						m_pNextMino[i] = new Mino(((MinoKind)num));
						++cnt;
						if (cnt > 6)
						{
							isEnd = true;
						}
						break;
					}
				}
			}
		}
		else if(m_nextMinoIdx == 0)
		{
			while (!isEnd)
			{
				num = rand() % 7;
				//被りがないか確認して生成する
				for (int i = 0; i < (int)MinoKind::MAX_MINO; ++i)
				{
					if (kinds[i] == num) break;
					if (kinds[i] == MinoKind::MAX_MINO)
					{
						kinds[i] = (MinoKind)num;
						m_pNextMino[i + 7] = new Mino(((MinoKind)num));
						++cnt;
						if (cnt > 6)
						{
							isEnd = true;
						}
						//m_pNextMino[m_nextMinoIdx].push(new Mino((MinoKind)num));
						break;
					}
				}
			}
		}
	}
	//最初の初期化用
	else
	{
		//被りがないかチェックするための配列
		MinoKind kinds[MinoKind::MAX_MINO] = { MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO,
											   MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO ,MinoKind::MAX_MINO };
		while (!isEnd)
		{
			num = rand() % 7;
			//被りがないか確認して生成する
			for (int i = 0; i < (int)MinoKind::MAX_MINO; ++i)
			{
				if (kinds[i] == num) break;
				if (kinds[i] == MinoKind::MAX_MINO)
				{
					kinds[i] = (MinoKind)num;
					m_pNextMino[i] = new Mino(((MinoKind)num));
					++cnt;
					if (cnt > 6)
					{
						isEnd = true;
					}
					//m_pNextMino[m_nextMinoIdx].push(new Mino((MinoKind)num));
					break;
				}
			}
		}
		isEnd = false;
		cnt = 0;
		//被りがないかチェックするための配列
		MinoKind kinds2[MinoKind::MAX_MINO] = { MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO,
											   MinoKind::MAX_MINO, MinoKind::MAX_MINO, MinoKind::MAX_MINO ,MinoKind::MAX_MINO };
		while (!isEnd)
		{
			num = rand() % 7;
			//被りがないか確認して生成する
			for (int i = 7; i < (int)MinoKind::MAX_MINO * 2; ++i)
			{
				if (kinds2[i - 7] == num) break;
				if (kinds2[i - 7] == MinoKind::MAX_MINO)
				{
					kinds2[i - 7] = (MinoKind)num;
					m_pNextMino[i] = new Mino(((MinoKind)num));
					++cnt;
					if (cnt > 6)
					{
						isEnd = true;
					}
					break;
				}
			}
		}
	}
}

void MinoMgr::HardDrop()
{
	if (m_curMino == nullptr) return;
	int tetMino[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			tetMino[i][j] = m_curMino->GetTetMino(i, j);
		}
	}
	//m_curMino->GetTetMino(tetMino);
	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);

	int point[2];
	//m_mino.GetPoint(point);
	point[0] = m_mino.GetPoint(0);
	point[1] = m_mino.GetPoint(1);

	for (int y = 3; y > -1; --y)
	{
		for (int x = 3; x > -1; --x)
		{
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
			{
				m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
			}
			if(tetMino[y][x] == 1)
			{
				m_Squware[point[0] + y][point[1] + x] = SquwareState::Move;
				if (!m_playerBoard.expired())
				{
					m_playerBoard.lock()->SetCr(point[0] + y, point[1] + x, m_curMino->GetCr());
				}
			}
		}
	}
	m_curMino->SetPoint(point);

	StopBlock();
}

bool MinoMgr::Hold()
{
	if (!(isKeyTrigger(KEY_INPUT_C) || isButtonTrigger(XINPUT_BUTTON_X, m_player))) return false;

	if (m_isHold)
	{
		int curPoint[2];
		curPoint[0] = m_curMino->GetPoint(0);
		curPoint[1] = m_curMino->GetPoint(1);
		//まだ一回もホールドを使用してない場合
		if (m_holdMino == nullptr && m_curMino != nullptr)
		{
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; x++)
				{
					//if (Board::m_Squware[m_actor][curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
					if (!m_playerBoard.expired())
					{
						if (m_playerBoard.lock()->GetSquware(curPoint[0] + y,curPoint[1] + x) == SquwareState::Move)
						{
							m_playerBoard.lock()->SetCr(curPoint[0] + y,curPoint[1] + x, GetColor(0, 0, 0));
							m_playerBoard.lock()->SetSquware(curPoint[0] + y, curPoint[1] + x, SquwareState::Non);
						}
						if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
						{
							m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
						}
					}
				}
			}
			m_holdMino = m_curMino;
			m_curMino = nullptr;
			m_createT = 1;
		}
		else if (m_curMino != nullptr)
		{
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; x++)
				{
					if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move
						&& m_Squware[curPoint[0] + y][curPoint[1] + x] != SquwareState::Stop)
					{
						if (!m_playerBoard.expired())
						{
							m_playerBoard.lock()->SetCr(curPoint[0] + y, curPoint[1] + x, GetColor(0, 0, 0));
						}
						m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					}
				}
			}
			curPoint[0] = 0;
			curPoint[1] = 3;
			m_curMino->SetPoint(curPoint);
			m_curMino->SetAngle(Angle::ANGLE_0);
			std::swap(m_curMino, m_holdMino);
			m_curMino->Init(this);
		}
		m_isHold = false;
		return true;
	}
	return false;
}

bool MinoMgr::HoldAI()
{
	if (m_isHold)
	{
		int curPoint[2];
		curPoint[0] = m_curMino->GetPoint(0);
		curPoint[1] = m_curMino->GetPoint(1);
		//まだ一回もホールドを使用してない場合
		if (m_holdMino == nullptr && m_curMino != nullptr)
		{
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; x++)
				{
					if(!m_playerBoard.expired())
					if (m_playerBoard.lock()->GetSquware(curPoint[0] + y, curPoint[1] + x) == SquwareState::Move)
					{
						m_playerBoard.lock()->SetCr(curPoint[0] + y, curPoint[1] + x, GetColor(0, 0, 0));
						m_playerBoard.lock()->SetSquware(curPoint[0] + y, curPoint[1] + x, SquwareState::Non);
					}
					if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
					{
						m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					}
				}
			}
			m_holdMino = m_curMino;
			m_curMino = nullptr;
			m_createT = 1;
		}
		else if (m_curMino != nullptr)
		{
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; x++)
				{
					if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move
						&& m_Squware[curPoint[0] + y][curPoint[1] + x] != SquwareState::Stop)
					{
						if (!m_playerBoard.expired())
						{
							m_playerBoard.lock()->SetCr(curPoint[0] + y, curPoint[1] + x, GetColor(0, 0, 0));
						}
						m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					}
				}
			}
			curPoint[0] = 0;
			curPoint[1] = 3;
			m_curMino->SetPoint(curPoint);
			m_curMino->SetAngle(Angle::ANGLE_0);
			std::swap(m_curMino, m_holdMino);
			m_curMino->Init(this);
		}
		m_isHold = false;
		return true;
	}
	return false;
}

void MinoMgr::LSpin()
{
	if (m_curMino != nullptr)
	{
		switch (m_curMino->GetAngle())
		{
		case ANGLE_0:
			m_curMino->Spin(ANGLE_270, this);
			break;
		case ANGLE_90:
			m_curMino->Spin(ANGLE_0, this);
			break;
		case ANGLE_180:
			m_curMino->Spin(ANGLE_90, this);
			break;
		case ANGLE_270:
			m_curMino->Spin(ANGLE_180, this);
			break;
		}
	}
}

void MinoMgr::RSpin()
{
	if (m_curMino != nullptr)
	{
		switch (m_curMino->GetAngle())
		{
		case ANGLE_0:
			m_curMino->Spin(ANGLE_90, this);
			break;
		case ANGLE_90:
			m_curMino->Spin(ANGLE_180, this);
			break;
		case ANGLE_180:
			m_curMino->Spin(ANGLE_270, this);
			break;
		case ANGLE_270:
			m_curMino->Spin(ANGLE_0, this);
			break;
		}
	}
}

//AI用
void MinoMgr::LeftMove()
{
	if (m_curMino == nullptr) return;

	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);
	bool isMoveL = true;
	m_moveT = 0;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if ((curPoint[0] + y) < 0) continue;
			//動き中のミノの隣が止まっていたら動かせない
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move &&
				m_Squware[curPoint[0] + y][curPoint[1] + x - 1] == SquwareState::Stop)
			{
				isMoveL = false;
			}
		}
	}
	if (isMoveL)
	{
		//移動する処理
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if ((curPoint[0] + y) < 0) continue;
				if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
				{
					m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					m_Squware[curPoint[0] + y][curPoint[1] + x - 1] = SquwareState::Move;
				}
			}
		}
		//座標更新
		curPoint[1] -= 1;
		m_curMino->SetPoint(curPoint);
		m_ForecastPoint[1] -= 1;
	}
}

void MinoMgr::LeftMove(int key)
{
	if (!key) return;
	if (m_curMino == nullptr) return;

	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);
	bool isMoveL = true;
	m_moveT = 0;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if ((curPoint[0] + y) < 0) continue;
			//動き中のミノの隣が止まっていたら動かせない
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move &&
				m_Squware[curPoint[0] + y][curPoint[1] + x - 1] == SquwareState::Stop)
			{
				isMoveL = false;
			}
		}
	}
	if (isMoveL)
	{
		//移動する処理
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if ((curPoint[0] + y) < 0) continue;
				if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
				{
					m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					m_Squware[curPoint[0] + y][curPoint[1] + x - 1] = SquwareState::Move;
				}
			}
		}
		//座標更新
		curPoint[1] -= 1;
		m_curMino->SetPoint(curPoint);
		m_ForecastPoint[1] -= 1;
	}
}

//AI用
void MinoMgr::RightMove()
{
	if (m_curMino == nullptr) return;
	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);
	bool isMoveR = true;
	m_moveT = 0;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 3; x > -1; --x)
		{
			if ((curPoint[0] + y) < 0) continue;
			//動き中のミノの隣が止まっていなかったら動かせない
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move &&
				m_Squware[curPoint[0] + y][curPoint[1] + x + 1] == SquwareState::Stop)
			{
				isMoveR = false;
			}
		}
	}
	if (isMoveR)
	{
		//移動する処理
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 3; x > -1; --x)
			{
				if ((curPoint[0] + y) < 0) continue;
				if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
				{
					m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					m_Squware[curPoint[0] + y][curPoint[1] + x + 1] = SquwareState::Move;
				}
			}
		}
		//座標更新
		curPoint[1] += 1;
		m_curMino->SetPoint(curPoint);
		m_ForecastPoint[1] += 1;
	}
}
void MinoMgr::RightMove(int key)
{
	if (!key) return;
	if (m_curMino == nullptr) return;
	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);
	bool isMoveR = true;
	m_moveT = 0;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 3; x > -1; --x)
		{
			if ((curPoint[0] + y) < 0) continue;
			//動き中のミノの隣が止まっていなかったら動かせない
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move &&
				m_Squware[curPoint[0] + y][curPoint[1] + x + 1] == SquwareState::Stop)
			{
				isMoveR = false;
			}
		}
	}
	if (isMoveR)
	{
		//移動する処理
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 3; x > -1; --x)
			{
				if ((curPoint[0] + y) < 0) continue;
				if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
				{
					m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
					m_Squware[curPoint[0] + y][curPoint[1] + x + 1] = SquwareState::Move;
				}
			}
		}
		//座標更新
		curPoint[1] += 1;
		m_curMino->SetPoint(curPoint);
		m_ForecastPoint[1] += 1;
	}
}

void MinoMgr::Fall()
{
	if (m_curMino == nullptr) return;
	bool isMoveD = true;	//落下処理を行っていいかどうか

	//----落下処理
	if (m_fallT > 1)
	{
		int curPoint[2];
		curPoint[0] = m_curMino->GetPoint(0);
		curPoint[1] = m_curMino->GetPoint(1);
		//タイムを初期化
		m_fallT = 0;
		if (m_curMino == nullptr) return;

		for (int y = 3; y > -1; --y)
		{
			for (int x = 3; x > -1; --x)
			{
				if ((curPoint[0] + y) < 0) continue;
				//動き中のミノの下が止まっていなかったら動かせない
				if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move &&
					m_Squware[curPoint[0] + y + 1][curPoint[1] + x] == SquwareState::Stop)
				{
					isMoveD = false;
				}
			}
		}
		//移動する処理
		if (isMoveD)
		{
			for (int y = 3; y > -1; --y)
			{
				for (int x = 3; x > -1; --x)
				{
					if ((curPoint[0] + y) < 0) continue;
					if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
					{
						m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Non;
						m_Squware[curPoint[0] + y + 1][curPoint[1] + x] = SquwareState::Move;
					}
				}
			}
			curPoint[0] += 1;
			m_curMino->SetPoint(curPoint);
		}
		//下にブロックがあった場合動きを止める
		else
		{
			StopBlock();
		}
	}
}

void MinoMgr::ForecastMino()
{
	if (m_curMino == nullptr) return;

	m_mino = *m_curMino;

	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);

	int point[2];
	point[0] = m_mino.GetPoint(0);
	point[1] = m_mino.GetPoint(1);

	int tetMino[4][4];
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			tetMino[i][j] = m_curMino->GetTetMino(i, j);
		}
	}

	bool isEnd = true;
	//古いシャドーを消す
	for (int y = 3; y < BOARD_HEIGHT - 1; y++)
	{
		for (int x = 1; x < BOARD_WIDTH - 1; x++)
		{
			if (m_Squware[y][x] == SquwareState::Forecast)
			{
				m_Squware[y][x] = SquwareState::Non;
			}
		}
	}

	while (isEnd)
	{
		//下に止まっているミノがあればループ終了
		for (int y = 0; y < 4 && isEnd; y++)
		{
			for (int x = 0; x < 4 && isEnd; x++)
			{
				if (m_Squware[point[0] + y][point[1] + x] == SquwareState::Move &&
					m_Squware[point[0] + y + 1][point[1] + x] == SquwareState::Stop)
				{
					isEnd = false;
				}
			}
		}
		//一段下に移す
		for (int y = 3; y > -1 && isEnd; --y)
		{
			for (int x = 3; x > -1 && isEnd; --x)
			{
				if (m_Squware[point[0] + y][point[1] + x] == SquwareState::Move)
				{
					m_Squware[point[0] + y][point[1] + x] = SquwareState::Non;
					m_Squware[point[0] + y + 1][point[1] + x] = SquwareState::Move;
				}
			}
		}
		//pointを一つ下に落とす
		if (isEnd)
		{
			point[0] += 1;
			m_mino.SetPoint(point);
		}
	}

	//変換
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_Squware[point[0] + y][point[1] + x] == SquwareState::Move)
			{
				m_Squware[point[0] + y][point[1] + x] = SquwareState::Forecast;
			}
		}
	}
	//元に戻す
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (tetMino[y][x])
			{
				
				m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Move;
			}
		}
	}

	m_ForecastPoint[0] = point[0];
	m_ForecastPoint[1] = point[1];
}

void MinoMgr::StopBlock()
{
	PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);

	const int PIECESIZE = 20;

	int curPoint[2];
	curPoint[0] = m_curMino->GetPoint(0);
	curPoint[1] = m_curMino->GetPoint(1);

	for (int y = 3; y > -1; --y)
	{
		for (int x = 3; x > -1; --x)
		{
			if ((curPoint[0] + y) < 0) continue;
			if (m_Squware[curPoint[0] + y][curPoint[1] + x] == SquwareState::Move)
			{
				//色情報をボードに渡す
				if (!m_playerBoard.expired())
				{
					m_playerBoard.lock()->SetCr(curPoint[0] + y, curPoint[1] + x, m_curMino->GetCr());
				}
				//動いていたミノを止まっているミノにする
				m_Squware[curPoint[0] + y][curPoint[1] + x] = SquwareState::Stop;
				if (SceneMgr::GetInstance()->GetType() == GameType::Normal)
				{
					if (!m_effectMgr.expired())
					{
						m_effectMgr.lock()->CreateEffect(curPoint[1] * PIECESIZE + 200, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::DROP_ANIME);
					}
				}
				else
				{
					if (m_actor == PLAYER)
					{
						if (!m_effectMgr.expired())
						{
							m_effectMgr.lock()->CreateEffect(curPoint[1] * PIECESIZE + SQUWARE_LEFT, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::DROP_ANIME);
						}
					}
					else
					{
						if (!m_effectMgr.expired())
						{
							m_effectMgr.lock()->CreateEffect(curPoint[1] * PIECESIZE + SQUWARE_LEFT2, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::DROP_ANIME);
						}
					}
				}
			}
		}
	}

	//消す処理を行う
	int stopMinoCnt = 0;

	//横に１０個止まっているミノがあればその横一列消す
	for (int i = 3; i < BOARD_HEIGHT - 1; ++i)
	{
		stopMinoCnt = 0;
		for (int j = 1; j < BOARD_WIDTH - 1; ++j)
		{
			//横一列に１０個止まっているミノがあるか調べる
			if (m_Squware[i][j] == SquwareState::Stop)
			{
				++stopMinoCnt;
			}
			if (stopMinoCnt == 10)
			{
				//消す処理
				for (int k = 1; k < BOARD_WIDTH - 1; k++)
				{
					m_Squware[i][k] = SquwareState::Non;
					//色を消す
					if (!m_playerBoard.expired())
					{
						m_playerBoard.lock()->SetCr(i, k, GetColor(255, 255, 255));
					}
				}
				m_isDelete = true;
				//消したライン数を保持する
				++m_deleteCnt;
				m_endIdx = i;
				if (SceneMgr::GetInstance()->GetType() == GameType::Normal)
				{
					if (!m_effectMgr.expired())
					{
						m_effectMgr.lock()->CreateEffect(200, i * 20, EffectMgr::eEffect::WAVE);
					}
				}
				else
				{
					if (m_actor == PLAYER)
					{
						if (!m_effectMgr.expired())
						{
							m_effectMgr.lock()->CreateEffect(SQUWARE_LEFT, i * 20, EffectMgr::eEffect::WAVE);
						}
					}
					else
					{
						if (!m_effectMgr.expired())
						{
							m_effectMgr.lock()->CreateEffect(SQUWARE_LEFT2, i* 20, EffectMgr::eEffect::WAVE);
						}
					}
				}
			}
		}
	}

	Actor actor = Actor::MAX_ACTOR;

	if (m_actor == PLAYER)
	{
		actor = ENEMY;
	}
	else
	{
		actor = PLAYER;
	}

	int breakLine = GameMgr::GetInstance()->GetBreakLine((int)actor);
	if (breakLine > 0)
	{
		int num = 0;
		for (int h = 0; h < breakLine; ++h)
		{
			for (int i = 4; i < BOARD_HEIGHT - 1; ++i)
			{
				for (int j = 1; j < BOARD_WIDTH - 1; ++j)
				{
					if (m_Squware[i][j] == SquwareState::Stop)
					{
						if (!m_effectMgr.expired())
						{
							m_playerBoard.lock()->SetCr(i - 1, j, m_playerBoard.lock()->GetCr(i, j));
							m_playerBoard.lock()->SetCr(i, j, GetColor(255, 255, 255));
						}
						m_Squware[i][j] = SquwareState::Non;
						m_Squware[i - 1][j] = SquwareState::Stop;
					}
				}
			}
		}
		
		num = rand() % 10;
		for (int k = 0; k < breakLine; ++k)
		{
			for (int l = 0; l < 10; ++l)
			{
				if (num == l) continue;
				m_Squware[BOARD_HEIGHT - 2 - k][l + 1] = SquwareState::Stop;
				if (!m_effectMgr.expired())
				{
					m_playerBoard.lock()->SetCr(BOARD_HEIGHT - 2 - k, l + 1, GetColor(255, 255, 255));
				}
			}
		}
		GameMgr::GetInstance()->SetBreakLine(actor,0);
	}



	bool allErased = true;
	for (int i = 3; i < BOARD_HEIGHT - 1 && allErased; ++i)
	{
		for (int j = 1; j < BOARD_WIDTH - 1; ++j)
		{
			if (m_Squware[i][j] != SquwareState::Non)
			{
				allErased = false;
				break;
			}
		}
	}
	if (allErased)
	{
		if (m_actor == PLAYER)
		{
			GameMgr::GetInstance()->GetScore()->AddScore(1000);
		}
		else
		{
			GameMgr::GetInstance()->GetScore(1)->AddScore(1000);
		}
	}

	//盤面をBoardにコピーして渡す
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (!m_playerBoard.expired())
			{
				m_playerBoard.lock()->SetSquware( y, x, m_Squware[y][x]);
			}
		}
	}
	//現在のミノは用済みなのでデリート
	delete m_curMino;
	m_curMino = nullptr;
	//ホールドできるようにする
	m_isHold = true;
	if (m_deleteCnt > 0)
	{
		int deleteNum = 0;
		switch (m_deleteCnt)
		{
		case 2:
			deleteNum = 1;
			break;
		case 3:
			deleteNum = 2;
			break;
		case 4:
			deleteNum = 4;
		default:
			break;
		}
		m_combo.AddCombo();
		if (m_combo.GetCombo() > 3)
		{
			++deleteNum;
		}
		m_LineNum += m_deleteCnt;
		if (SceneMgr::GetInstance()->GetType() == GameType::Normal)
		{
			GameMgr::GetInstance()->GetScore()->AddScore(m_deleteCnt * 100 + m_combo.GetCombo() * 100);
			if (!m_effectMgr.expired())
			{
				m_effectMgr.lock()->CreateEffect(330, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::COMBO_PARTICLE, m_combo.GetCombo());
			}
		}
		else if (m_actor == PLAYER)
		{
			GameMgr::GetInstance()->GetScore()->AddScore(m_deleteCnt * 100 + m_combo.GetCombo() * 100);
			if (!m_effectMgr.expired())
			{
				m_effectMgr.lock()->CreateEffect(SQUWARE_LEFT + 100, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::COMBO_PARTICLE, m_combo.GetCombo());
			}
			GameMgr::GetInstance()->AddBreakLine(0, deleteNum);
		}
		else
		{
			GameMgr::GetInstance()->GetScore(1)->AddScore(m_deleteCnt * 100 + m_combo.GetCombo() * 100);
			if (!m_effectMgr.expired())
			{
				m_effectMgr.lock()->CreateEffect(SQUWARE_LEFT2 + 100, curPoint[0] * PIECESIZE + 40, EffectMgr::eEffect::COMBO_PARTICLE, m_combo.GetCombo());
			}
			GameMgr::GetInstance()->AddBreakLine(1, deleteNum);
		}
	}
	m_combo.Update();
}

bool MinoMgr::DeleteBlock()
{
	//消したライン数分上にあるミノを落とす
	//---消した後下に落とす処理
	if (m_isDelete)
	{
		m_deleteT += 1.0f / 20.0f;
		if (m_deleteT > 1)
		{
			m_isDelete = false;
		}
		else
		{
			return true;
		}
	}

	if (m_deleteT > 1)
	{
		m_deleteT = 0;
		for (int i = m_endIdx - 1; i > -1; --i)
		{
			for (int j = BOARD_WIDTH - 2; j > 0; --j)
			{
				if (m_Squware[i][j] == SquwareState::Stop)
				{
					//盤面からはみ出るかどうか
					if (i + m_deleteCnt < BOARD_HEIGHT - 1)
					{
						//下にずらす
						m_Squware[i + m_deleteCnt][j] = SquwareState::Stop;
						m_Squware[i][j] = SquwareState::Non;
						if (!m_playerBoard.expired())
						{
							m_playerBoard.lock()->SetCr(i + m_deleteCnt, j, m_playerBoard.lock()->GetCr( i, j));
							m_playerBoard.lock()->SetCr(i, j, GetColor(111, 0, 0));
						}
					}
					else
					{
						//下にずらす
						m_Squware[19][j] = SquwareState::Stop;
						m_Squware[i][j] = SquwareState::Non;
						if (!m_playerBoard.expired())
						{
							m_playerBoard.lock()->SetCr(19, j, m_playerBoard.lock()->GetCr(i,j));
							m_playerBoard.lock()->SetCr(i, j, GetColor(111, 0, 0));
						}
					}
				}
			}
		}
		//落下しきっていないミノを落とす
		bool isEnd = false;
		while (!isEnd)
		{
			int cnt = 0;
			for (int i = m_endIdx - 1; i > -1; --i)
			{
				for (int j = BOARD_WIDTH - 2; j > 0; --j)
				{
					if (m_Squware[i][j] == SquwareState::Stop && m_Squware[i + 1][j] == SquwareState::Non)
					{
						//※盤面の四隅に当たり判定用の止まっているミノを配置している
						//左側のミノの場合右下に止まっているミノがあれば止める
						if (j == 1)
						{
							if (m_Squware[i + 1][j + 1] == SquwareState::Stop)
							{
								continue;
							}
						}
						//右側のミノの場合左下に止まっているミノがあれば止める
						else if (j == BOARD_WIDTH - 2)
						{
							if (m_Squware[i + 1][j - 1] == SquwareState::Stop)
							{
								continue;
							}
						}
						//左下、右下にミノがあれば止める
						else if (m_Squware[i + 1][j + 1] == SquwareState::Stop ||
							m_Squware[i + 1][j - 1] == SquwareState::Stop)
						{
							continue;
						}
						//条件に当てはまらない場合落下
						m_Squware[i][j] = SquwareState::Non;
						m_Squware[i + 1][j] = SquwareState::Fall;
						if (!m_playerBoard.expired())
						{
							m_playerBoard.lock()->SetCr(i + 1, j, m_playerBoard.lock()->GetCr(i, j));
							m_playerBoard.lock()->SetCr(i, j, GetColor(111, 0, 0));
						}
						cnt++;
					}
				}
			}
			if (cnt == 0) isEnd = true;
		}
		for (int i = BOARD_HEIGHT - 2; i > -1; --i)
		{
			for (int j = BOARD_WIDTH - 2; j > 0; --j)
			{
				if (m_Squware[i][j] == SquwareState::Fall)
					m_Squware[i][j] = SquwareState::Stop;
			}
		}
		//盤面をBoardにコピーして渡す
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				if (!m_playerBoard.expired())
				{
					m_playerBoard.lock()->SetSquware(y, x, m_Squware[y][x]);
				}
			}
		}
		m_deleteCnt = 0;
		m_endIdx = 0;
		return true;
	}

	return false;
}

void MinoMgr::CreateCurMino()
{
	m_createT += 1.0f / 10.0f;
	if (m_createT > 1)
	{
		m_curMino = m_pNextMino[m_nextMinoIdx];
		m_pNextMino[m_nextMinoIdx] = nullptr;
		++m_nextMinoIdx;
		if (m_nextMinoIdx > 13)
		{
			m_nextMinoIdx = 0;
		}
		m_curMino->Init(this);
		ForecastMino();

		//NEXTミノが少なくなったら７つ生成
		if (m_nextMinoIdx == 0 || m_nextMinoIdx == 7)
		{
			CreateNextMino();
		}

		//タイムの初期化
		m_createT = 0;
		m_fallT = 0;
		m_moveT = 0;
	}
}

void MinoMgr::FallSpeedUp(bool key)
{
	if (!key) return;
	m_fallT += 1.0f / 3.0f;
}
void MinoMgr::DrawPlayer()
{
	const int PIECESIZE = 20;

	//---動いているミノの描画
	//一番下は処理しない
	for (int y = 3; y < BOARD_HEIGHT; ++y)
	{
		//左端と右端は処理しない
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_Squware[y][x] == SquwareState::Move)
			{
				if (m_curMino != nullptr)
				{
					DrawBox(200 + PIECESIZE * x, PIECESIZE * y, 218 + PIECESIZE * x, 18 + PIECESIZE * y, m_curMino->GetCr(), true);
				}
			}
		}
	}

	//シャドー
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 170);
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (m_Squware[m_ForecastPoint[0] + y][m_ForecastPoint[1] + x] == SquwareState::Forecast && m_curMino != nullptr)
			{
				DrawBox(200 + PIECESIZE * (m_ForecastPoint[1] + x),
						PIECESIZE * (m_ForecastPoint[0] + y),
						218 + PIECESIZE * (m_ForecastPoint[1] + x),
						18 + PIECESIZE * (m_ForecastPoint[0] + y),
						m_curMino->GetCr(), true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	//----nextミノの描画を行う
	//常に３個出す必要あり
	int drawNum = 0;										//一つのキューでDrawできる数 3個以上あるなら使われない
	int drawIdx = 0;										//drawするためのキューの要素数
	int drawCnt = 0;										//描画した回数のカウント
	int tetMino[4][4];
	int cnt = 0;
	int nextIdx = m_nextMinoIdx;

	while (cnt < 3)
	{
		if (m_pNextMino[nextIdx] != nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					tetMino[i][j] = m_pNextMino[nextIdx]->GetTetMino(i, j);
				}
			}
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					if (tetMino[y][x] == 1)
					{
						DrawBox(450 + 20 * x,
								100 + 20 * y + drawCnt * 100,
								468 + 20 * x,
								118 + 20 * y + drawCnt * 100,
								m_pNextMino[nextIdx]->GetCr(), true);
					}
				}
			}
			++cnt;
			++drawCnt;
		}
		++nextIdx;
		if (nextIdx > 13)
		{
			nextIdx = 0;
		}
	}

	//ホールドミノの描画を行う
	if (m_holdMino != nullptr)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_holdMino->GetTetMino(i, j);
			}
		}
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (tetMino[y][x] == 1)
				{
					DrawBox(100 + 20 * x,
							50 + 20 * y,
							118 + 20 * x,
							68 + 20 * y,
							m_holdMino->GetCr(), true);
				}
			}
		}
	}

	//ライン数の描画
	DrawFormatString(m_linePoint[0], m_linePoint[1], m_lineCr, "ライン:%d", m_LineNum);
	//Combo数の描画
	m_combo.Draw();
}

void MinoMgr::DrawPlayerVS()
{
	const int PIECESIZE = 20;

	//---動いているミノの描画
	//一番下は処理しない
	for (int y = 3; y < BOARD_HEIGHT; ++y)
	{
		//左端と右端は処理しない
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_Squware[y][x] == SquwareState::Move)
			{
				if (m_curMino != nullptr)
				{
					DrawBox(SQUWARE_LEFT + PIECESIZE * x, PIECESIZE * y, SQUWARE_LEFT + 18 + PIECESIZE * x, 18 + PIECESIZE * y, m_curMino->GetCr(), true);
				}
			}
		}
	}

	//シャドー
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 170);
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (m_Squware[m_ForecastPoint[0] + y][m_ForecastPoint[1] + x] == SquwareState::Forecast && m_curMino != nullptr)
			{
				DrawBox(SQUWARE_LEFT + PIECESIZE * (m_ForecastPoint[1] + x),
						PIECESIZE * (m_ForecastPoint[0] + y),
						SQUWARE_LEFT + 18 + PIECESIZE * (m_ForecastPoint[1] + x),
						18 + PIECESIZE * (m_ForecastPoint[0] + y),
						m_curMino->GetCr(), true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	//----nextミノの描画を行う
	//常に３個出す必要あり
	int drawNum = 0;										
	int drawIdx = 0;										//drawするためのキューの要素数
	int drawCnt = 0;										//描画した回数のカウント
	int tetMino[4][4];
	int cnt = 0;
	int nextIdx = m_nextMinoIdx;

	while (cnt < 2)
	{
		if (m_pNextMino[nextIdx] != nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					tetMino[i][j] = m_pNextMino[nextIdx]->GetTetMino(i, j);
				}
			}
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					if (tetMino[y][x] == 1)
					{
						DrawBox(10 + 20 * x,
								200 + 20 * y + drawCnt * 100,
								28 + 20 * x,
								218 + 20 * y + drawCnt * 100,
								m_pNextMino[nextIdx]->GetCr(), true);
					}
				}
			}
			++cnt;
			++drawCnt;
		}
		++nextIdx;
		if (nextIdx > 13)
		{
			nextIdx = 0;
		}
	}

	//ホールドミノの描画を行う
	if (m_holdMino != nullptr)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_holdMino->GetTetMino(i, j);
			}
		}
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (tetMino[y][x] == 1)
				{
					DrawBox(50 + 20 * x,
							10 + 20 * y,
							68 + 20 * x,
							28 + 20 * y,
							m_holdMino->GetCr(), true);
				}
			}
		}
	}

	//ライン数の描画
	DrawFormatString(m_linePoint[0], m_linePoint[1], m_lineCr, "ライン:%d", m_LineNum);
	//Combo数の描画
	m_combo.Draw();
}

void MinoMgr::DrawEnemy()
{
	const int PIECESIZE = 20;

	//---動いているミノの描画
	//一番下は処理しない
	for (int y = 3; y < BOARD_HEIGHT; ++y)
	{
		//左端と右端は処理しない
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_Squware[y][x] == SquwareState::Move)
			{
				if (m_curMino != nullptr)
				{
					DrawBox(SQUWARE_LEFT2 + PIECESIZE * x, PIECESIZE * y,
							SQUWARE_LEFT2 + 18 + PIECESIZE * x,
							18 + PIECESIZE * y,
							m_curMino->GetCr(), true);
				}
			}
		}
	}

	//シャドー
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 170);
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (m_Squware[m_ForecastPoint[0] + y][m_ForecastPoint[1] + x] == SquwareState::Forecast && m_curMino != nullptr)
			{
				DrawBox(SQUWARE_LEFT2 + PIECESIZE * (m_ForecastPoint[1] + x),
						PIECESIZE * (m_ForecastPoint[0] + y),
						SQUWARE_LEFT2 + 18 + PIECESIZE * (m_ForecastPoint[1] + x),
						18 + PIECESIZE * (m_ForecastPoint[0] + y),
						m_curMino->GetCr(), true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	//----nextミノの描画を行う
//常に３個出す必要あり
	int drawNum = 0;
	int drawIdx = 0;										//drawするためのキューの要素数
	int drawCnt = 0;										//描画した回数のカウント
	int tetMino[4][4];
	int cnt = 0;
	int nextIdx = m_nextMinoIdx;
	if (SceneMgr::GetInstance()->GetType() == GameType::PVE)
	{
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 70);
	}
	while (cnt < 2)
	{
		if (m_pNextMino[nextIdx] != nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					tetMino[i][j] = m_pNextMino[nextIdx]->GetTetMino(i, j);
				}
			}
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					if (tetMino[y][x] == 1)
					{
						DrawBox(350 + 20 * x,
								200 + 20 * y + drawCnt * 100,
								368 + 20 * x,
								218 + 20 * y + drawCnt * 100,
								m_pNextMino[nextIdx]->GetCr(), true);
					}
				}
			}
			++cnt;
			++drawCnt;
		}
		++nextIdx;
		if (nextIdx > 13)
		{
			nextIdx = 0;
		}
	}

#pragma region "ホールドミノの描画を行う"
	//ホールドミノの描画を行う
	if (m_holdMino != nullptr)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				tetMino[i][j] = m_holdMino->GetTetMino(i, j);
			}
		}
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (tetMino[y][x] == 1)
				{
					DrawBox(350 + 20 * x,
							10 + 20 * y,
							368 + 20 * x,
							28 + 20 * y,
							m_holdMino->GetCr(), true);
				}
			}
		}
	}
#pragma endregion

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	//ライン数の描画
	DrawFormatString(m_linePoint[0], m_linePoint[1], m_lineCr, "ライン:%d", m_LineNum);
	//Combo数の描画
	m_combo.Draw();
}
