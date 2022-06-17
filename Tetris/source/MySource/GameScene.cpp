#include "GameScene.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "input.h"
#include "MinoMgr.h"
#include "GameMgr.h"
#include "main.h"
#include "TitleScene.h"
#include "EffectMgr.h"
#include "Board.h"
#include "Constant.h"

using namespace Constant;


//---------------------------
//マジックナンバーを消していく
//---------------------------

GameScene::GameScene(GameType type) : m_choice(0), m_choiceT(0), m_Board{nullptr, nullptr}, m_minoMgrP(nullptr), m_minoMgrE(nullptr), m_effectMgr(nullptr)
{


	if (m_effectMgr == nullptr)
	{
		m_effectMgr = std::make_shared<EffectMgr>();
	}

	switch (type)
	{
		//1P
	case GameType::Normal:
		if (m_minoMgrP == nullptr)
		{
			if (m_Board[0] == nullptr)
			{
				m_Board[0] = std::make_shared<Board>(SQUWARE_SINGLE_LEFT);
			}
			m_minoMgrP = new MinoMgr(Actor::PLAYER, 0, m_effectMgr, m_Board[0]);
			m_minoMgrP->Init();
			MinoMgr::SetPlayerNum(1);
			GameMgr::GetInstance()->GetScore()->SetExist(true);
		}
		break;
		//vsCPU
	case GameType::PVE:
		if (m_minoMgrP == nullptr)
		{
			if (m_Board[0] == nullptr)
			{
				m_Board[0] = std::make_shared<Board>(SQUWARE_LEFT);
			}
			m_minoMgrP = new MinoMgr(Actor::PLAYER, 0, m_effectMgr, m_Board[0]);
			m_minoMgrP->Init();
			GameMgr::GetInstance()->GetScore()->SetExist(true);
		}
		if (m_minoMgrE == nullptr)
		{
			if (m_Board[1] == nullptr)
			{
				m_Board[1] = std::make_shared<Board>(SQUWARE_LEFT2);
			}
			m_minoMgrE = new MinoMgr(Actor::ENEMY, 0, m_effectMgr, m_Board[1]);
			m_minoMgrE->Init();
			GameMgr::GetInstance()->GetScore(1)->SetExist(true);
		}
		MinoMgr::SetPlayerNum(1);
		break;
		//vs2P
	case GameType::PVP:
		if (m_minoMgrP == nullptr)
		{
			if (m_Board[0] == nullptr)
			{
				m_Board[0] = std::make_shared<Board>(SQUWARE_LEFT);
			}
			m_minoMgrP = new MinoMgr(Actor::PLAYER,0, m_effectMgr, m_Board[0]);
			m_minoMgrP->Init();
			GameMgr::GetInstance()->GetScore()->SetExist(true);
		}	
		if (m_minoMgrE == nullptr)
		{
			if (m_Board[1] == nullptr)
			{
				m_Board[1] = std::make_shared<Board>(SQUWARE_LEFT2);
			}
			m_minoMgrE = new MinoMgr(Actor::ENEMY,1, m_effectMgr, m_Board[1]);
			m_minoMgrE->Init();
			GameMgr::GetInstance()->GetScore(1)->SetExist(true);
		}
		MinoMgr::SetPlayerNum(2);
		break;
	}
	m_tex[GameScene::PLAY] = LoadGraph("Assets/play.png");
	m_tex[GameScene::TITLE] = LoadGraph("Assets/Title.png");
	m_tex[GameScene::RETRY] = LoadGraph("Assets/Retry.png");
	m_tex[GameScene::END] = LoadGraph("Assets/End.png");
	m_tex[GameScene::CHOICE_1] = LoadGraph("Assets/orange.png");
	PlaySoundFile("Assets/Game.mp3", DX_PLAYTYPE_LOOP);
	
	m_point[0] = 30;
	m_point[1] = 150;
	m_point[2] = 80;
	m_point[3] = 180;

	m_choicePoint[0] = 20;//左上
	m_choicePoint[1] = 160;
	m_choicePoint[2] = 60;//右上
	m_choicePoint[3] = 150;
	m_choicePoint[4] = 90;//右下
	m_choicePoint[5] = 170;
	m_choicePoint[6] = 40;//左下
	m_choicePoint[7] = 180;
	
	for (int i = 0; i < MAXRAND; i++)
	{
		m_choiceRand[i] = 0;
	}
	GameMgr::GetInstance()->Init();
}

GameScene::~GameScene()
{

}


void GameScene::Init()
{
	m_tex[GameScene::PLAY] = LoadGraph("Assets/play.png");
	m_tex[GameScene::TITLE] = LoadGraph("Assets/Title.png");
	m_tex[GameScene::RETRY] = LoadGraph("Assets/Retry.png");
	m_tex[GameScene::END] = LoadGraph("Assets/End.png");
	m_tex[GameScene::CHOICE_1] = LoadGraph("Assets/orange.png");
	PlaySoundFile("Assets/Game.mp3", DX_PLAYTYPE_LOOP);
	PlaySoundFile("Assets/Game.mp3", DX_PLAYTYPE_LOOP);
	if (m_effectMgr != nullptr)
	{
		m_effectMgr->Init();
	}
}

void GameScene::Uninit()
{
	for (int i = 0; i < GameScene::MAX; i++)
	{
		DeleteGraph(m_tex[i]);
	}
	if (CheckSoundFile())
	{
		StopSoundFile();
	}

	if (m_minoMgrP != nullptr)
	{
		m_minoMgrP->Uninit();
		delete m_minoMgrP;
		m_minoMgrP = nullptr;
	}
	if (m_minoMgrE != nullptr)
	{
		m_minoMgrE->Uninit();
		delete m_minoMgrE;
		m_minoMgrE = nullptr;
	}

	GameMgr::GetInstance()->Uninit();
}

void GameScene::Update()
{
	SceneMgr * pSceneMgr = SceneMgr::GetInstance();

	//ポーズするか
	if (isKeyTrigger(KEY_INPUT_P) || isButtonTrigger(XINPUT_BUTTON_START))
	{
		if (pSceneMgr->GetScene() == SceneMgr::Scene::Game)
		{
			pSceneMgr->SetScene(SceneMgr::Scene::Non);
		}
		else if (pSceneMgr->GetScene() == SceneMgr::Scene::Non)
		{
			pSceneMgr->SetScene(SceneMgr::Scene::Game);
		}
	}

	//ポーズ中ならリターン
	if (pSceneMgr->GetScene() == SceneMgr::Scene::Non)
	{
		return;
	}

	if (GameMgr::GetInstance()->GetGame())
	{
		if (m_minoMgrP != nullptr)
		{
			m_minoMgrP->Update();
		}
		if (m_minoMgrE != nullptr)
		{
			m_minoMgrE->Update();
		}

		if (m_effectMgr != nullptr)
		{
			m_effectMgr->Update();
		}
	}
	GameMgr::GetInstance()->Update();

	
}

void GameScene::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_Board[i] != nullptr)
		{
			m_Board[i]->Draw();
		}
	}
	
	if  (SceneMgr::GetInstance()->GetScene() != SceneMgr::Scene::Non)
	{
		if (m_minoMgrP != nullptr)
		{
			m_minoMgrP->Draw();
		}
		if (m_minoMgrE != nullptr)
		{
			m_minoMgrE->Draw();
		}
		if (m_effectMgr != nullptr)
			m_effectMgr->Draw();
		GameMgr::GetInstance()->Draw();
	}
	else
	{
		Pause();
	}
}

void GameScene::Pause()
{
	if (isKeyTrigger(KEY_INPUT_DOWN) || isButtonTrigger(XINPUT_BUTTON_DPAD_DOWN))
	{
		m_choice += 1;
		if (m_choice > 3)
		{
			m_choice = 0;
		}
	}
	else if (isKeyTrigger(KEY_INPUT_UP) || isButtonTrigger(XINPUT_BUTTON_DPAD_UP))
	{
		m_choice -= 1;
		if (m_choice < 0)
		{
			m_choice = 3;
		}
	}
	if (isKeyTrigger(KEY_INPUT_RETURN) || isButtonTrigger(XINPUT_BUTTON_A))
	{
		switch (m_choice)
		{
		case 0:
			SceneMgr::GetInstance()->SetScene(SceneMgr::Scene::Game);
			break;
		case 1:
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Game);
			return;
		case 2:
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Title);
			return;
		case 3:
			SetGame(false);
			return;
		}
	}

	if (m_choiceT < 1)
	{
		m_choiceT += 1.0f / 10.0f;
	}
	else
	{
		m_choiceT = 0;

		for (int i = 0; i < 8; i++)
		{
			m_choiceRand[i] = (rand() % 6) - 3;
		}
	}
	//ポーズ中の遷移先の描画
	for (int i = 0; i < 4; i++)
	{
		DrawModiGraph(m_point[0],
					  m_point[1] + ADDPOINT * i,
					  m_point[2],
					  m_point[1] + ADDPOINT * i,
					  m_point[2],
					  m_point[3] + ADDPOINT * i,
					  m_point[0],
					  m_point[3] + ADDPOINT * i,
					  m_tex[i], true);
	}

	//選択強調の描画
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 150);

	DrawModiGraph(m_choicePoint[0] + m_choiceRand[0],
		m_choicePoint[1] + m_choiceRand[1] + m_choice * ADDPOINT,
		m_choicePoint[2] + m_choiceRand[2],
		m_choicePoint[3] + m_choiceRand[3] + m_choice * ADDPOINT,
		m_choicePoint[4] + m_choiceRand[4],
		m_choicePoint[5] + m_choiceRand[5] + m_choice * ADDPOINT,
		m_choicePoint[6] + m_choiceRand[6],
		m_choicePoint[7] + m_choiceRand[7] + m_choice * ADDPOINT,
		m_tex[GameScene::CHOICE_1], true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
}


