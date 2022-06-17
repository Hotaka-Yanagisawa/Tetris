#include "ResultScene.h"
#include "DxLib.h"
#include "GameMgr.h"
#include "input.h"
#include "SceneMgr.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "main.h"


ResultScene::ResultScene(): m_timer(0),m_choiceT(0),m_choice(0)
{
	m_tex[ResultScene::TITLE] = LoadGraph("Assets/Title.png");
	m_tex[ResultScene::RETRY] = LoadGraph("Assets/Retry.png");
	m_tex[ResultScene::END] = LoadGraph("Assets/End.png");
	m_tex[ResultScene::BG] = LoadGraph("Assets/ResultBG.png");
	m_tex[ResultScene::CHOICE_1] = LoadGraph("Assets/orange.png");
	m_tex[ResultScene::CHOICE_2] = LoadGraph("Assets/orange_2.png");
	m_sHandle = LoadSoundMem("Assets/Jingle.mp3");
	m_choicePoint[0] = 190;//左上
	m_choicePoint[1] = 410;
	m_choicePoint[2] = 230;//右上
	m_choicePoint[3] = 400;
	m_choicePoint[4] = 260;//右下
	m_choicePoint[5] = 420;
	m_choicePoint[6] = 210;//左下
	m_choicePoint[7] = 430;

	//----定数増やして仕様変更に対応しやすくしたり、マジックナンバーを消す
	for (int i = 0; i < MAXRAND; i++)
	{
		m_choiceRand[i] = 0;
	}
}

ResultScene::~ResultScene()
{
	for (int i = 0; i < ResultScene::MAX; i++)
	{
		DeleteGraph(m_tex[i]);
	}
	DeleteSoundMem(m_sHandle);
}

void ResultScene::Init()
{
	m_tex[ResultScene::TITLE] = LoadGraph("Assets/Title.png");
	m_tex[ResultScene::RETRY] = LoadGraph("Assets/Retry.png");
	m_tex[ResultScene::END] = LoadGraph("Assets/End.png");
	m_tex[ResultScene::BG] = LoadGraph("Assets/ResultBG.png");
	m_tex[ResultScene::CHOICE_1] = LoadGraph("Assets/orange.png");
	m_tex[ResultScene::CHOICE_2] = LoadGraph("Assets/orange_2.png");
	m_sHandle = LoadSoundMem("Assets/Jingle.mp3");
}

void ResultScene::Uninit()
{
}

void ResultScene::Update()
{
	if (m_timer <= 2)
	{
		m_timer += 1.0f / 60.0f;
	}
	if (m_timer >= 1 && !GameMgr::GetInstance()->GetGame())
	{
		GameMgr::GetInstance()->GetScore()->SetPoint(-999, -999);
		GameMgr::GetInstance()->GetScore(1)->SetPoint(-999,-999);
		GameMgr::GetInstance()->SetGame(true);
		PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);
	}
	else if(m_timer >= 2 && m_timer < 3)
	{
		m_timer = 999;
		GameMgr::GetInstance()->GetScore()->SetPoint(260, 180);
		if (SceneMgr::GetInstance()->GetType() != GameType::Normal)
		{
			GameMgr::GetInstance()->GetScore(1)->SetPoint(260, 220);
		}
	}

	if (isKeyTrigger(KEY_INPUT_RIGHT) || isButtonTrigger(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_choice += 1;
		if (m_choice > 2)
		{
			m_choice = 0;
		}
	}
	else if (isKeyTrigger(KEY_INPUT_LEFT) || isButtonTrigger(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_choice -= 1;
		if (m_choice < 0)
		{
			m_choice = 2;
		}
	}
	if (isKeyTrigger(KEY_INPUT_RETURN) || isButtonTrigger(XINPUT_BUTTON_A))
	{
		switch (m_choice)
		{
		case 0:
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Game);
			break;
		case 1:
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Title);
			break;
		case 2:
			SetGame(false);
			break;
		}
	}
}

void ResultScene::Draw()
{
	if (m_choiceT < 1)
	{
		m_choiceT += 1.0f / 10.0f;
	}
	else
	{
		m_choiceT = 0;

		for (int i = 0; i < 16; i++)
		{
			m_choiceRand[i] = (rand() % 6) - 3;
		}
	}

	//DrawModiGraph(0, 0, 640, 0, 640, 480, 0, 480, m_tex[ResultScene::BG], false);
	DrawModiGraph(200, 390, 250, 390, 250, 440, 200, 440, m_tex[ResultScene::RETRY], true);
	DrawModiGraph(290, 390, 340, 390, 340, 440, 290, 440, m_tex[ResultScene::TITLE], true);
	DrawModiGraph(380, 390, 430, 390, 430, 440, 380, 440, m_tex[ResultScene::END], true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 150);

	DrawModiGraph(m_choicePoint[0] + m_choiceRand[0] + m_choice * 90,
		m_choicePoint[1] + m_choiceRand[1],
		m_choicePoint[2] + m_choiceRand[2] + m_choice * 90,
		m_choicePoint[3] + m_choiceRand[3],
		m_choicePoint[4] + m_choiceRand[4] + m_choice * 90,
		m_choicePoint[5] + m_choiceRand[5],
		m_choicePoint[6] + m_choiceRand[6] + m_choice * 90,
		m_choicePoint[7] + m_choiceRand[7],
		m_tex[ResultScene::CHOICE_1], true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	if (GameMgr::GetInstance()->GetGame())
	{
		GameMgr::GetInstance()->GetScore()->Draw();
		GameMgr::GetInstance()->GetScore(1)->Draw();
	}
}
