#include "TitleScene.h"
#include "DxLib.h"
#include "input.h"
#include "SceneMgr.h"

TitleScene::TitleScene(): m_drawT(0),m_choice(0),m_choiceT(0)
{
	PlaySoundFile("Assets/Title.mp3", DX_PLAYTYPE_LOOP);
	m_tex[TitleScene::TITLE] = LoadGraph("Assets/Tetris.png");
	m_tex[TitleScene::ENTER] = LoadGraph("Assets/Enter.png");
	m_tex[TitleScene::A_BUTTON] = LoadGraph("Assets/AButton.png");
	m_tex[TitleScene::BG] = LoadGraph("Assets/TitleBG.png");
	m_tex[TitleScene::Normal] = LoadGraph("Assets/1P.png");
	m_tex[TitleScene::PVE] = LoadGraph("Assets/1PvsCPU.png");
	m_tex[TitleScene::PVP] = LoadGraph("Assets/1Pvs2P.png");
	m_tex[TitleScene::CHOICE] = LoadGraph("Assets/orange.png");

	m_sHandle = LoadSoundMem("Assets/System_1.mp3");
	m_point[0][0] = 275;
	m_point[0][1] = 375;
	m_point[0][2] = 375;
	m_point[0][3] = 425;
	m_point[1][0] = 255;
	m_point[1][1] = 375;
	m_point[1][2] = 395;
	m_point[1][3] = 425;
	m_titlePoint[0] = 220;
	m_titlePoint[1] = 150;
	SceneMgr::GetInstance()->SetType(GameType::Normal);
	m_choicePoint[0] = 100;//左上
	m_choicePoint[1] = 310;
	m_choicePoint[2] = 220;//右上
	m_choicePoint[3] = 300;
	m_choicePoint[4] = 250;//右下
	m_choicePoint[5] = 360;
	m_choicePoint[6] = 130;//左下
	m_choicePoint[7] = 370;
	//----定数増やして仕様変更に対応しやすくしたり、マジックナンバーを消す
	for (int i = 0; i < MAXRAND; i++)
	{
		m_choiceRand[i] = 0;
	}
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_tex[TitleScene::BG]);
	DeleteGraph(m_tex[TitleScene::TITLE]);
	DeleteGraph(m_tex[TitleScene::ENTER]);
	DeleteGraph(m_tex[TitleScene::PVE]);
	DeleteGraph(m_tex[TitleScene::PVP]);
	DeleteGraph(m_tex[TitleScene::Normal]);
	DeleteGraph(m_tex[TitleScene::CHOICE]);
	DeleteSoundMem(m_sHandle);
}

void TitleScene::Init()
{
	PlaySoundFile("Assets/Title.mp3", DX_PLAYTYPE_LOOP);
	m_tex[TitleScene::TITLE] = LoadGraph("Assets/Tetris.png");
	m_tex[TitleScene::ENTER] = LoadGraph("Assets/Enter.png");
	m_tex[TitleScene::A_BUTTON] = LoadGraph("Assets/AButton.png");
	m_tex[TitleScene::BG] = LoadGraph("Assets/TitleBG.png");
	m_sHandle = LoadSoundMem("Assets/System_1.mp3");
}

void TitleScene::Uninit()
{
	if (CheckSoundFile())
	{
		StopSoundFile();
	}
}

void TitleScene::Update()
{
	m_drawT += 1.0f / 15.0f;

	InputSearch();

	if (isKeyTrigger(KEY_INPUT_RIGHT) || isButtonTrigger(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_choice += 1;
		if (m_choice > 2)
		{
			m_choice = 0;
		}
		if (m_choice == 0) { SceneMgr::GetInstance()->SetType(GameType::Normal); }
		if (m_choice == 1) { SceneMgr::GetInstance()->SetType(GameType::PVE); }
		if (m_choice == 2) { SceneMgr::GetInstance()->SetType(GameType::PVP); }
	}
	else if (isKeyTrigger(KEY_INPUT_LEFT) || isButtonTrigger(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_choice -= 1;
		if (m_choice < 0)
		{
			m_choice = 2;
		}
		if (m_choice == 0) { SceneMgr::GetInstance()->SetType(GameType::Normal); }
		if (m_choice == 1) { SceneMgr::GetInstance()->SetType(GameType::PVE); }
		if (m_choice == 2) { SceneMgr::GetInstance()->SetType(GameType::PVP); }
	}


	if (isKeyTrigger(KEY_INPUT_RETURN) || isButtonTrigger(XINPUT_BUTTON_A))
	{
		if (SceneMgr::GetInstance()->GetScene() != SceneMgr::Scene::Game)
		{
			PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Game);
		}
	}
}

void TitleScene::Draw()
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
	DrawModiGraph(0, 0, 640, 0, 640, 480, 0, 480, m_tex[TitleScene::BG], false);
	DrawGraph(m_titlePoint[0], m_titlePoint[1], m_tex[TitleScene::TITLE], true);

	DrawModiGraph(140, 300, 190, 300, 190, 370, 140, 370 ,m_tex[TitleScene::Normal], true);
	DrawModiGraph(260, 300, 380, 300, 380, 370, 260, 370 ,m_tex[TitleScene::PVE], true);
	DrawModiGraph(410, 300, 560, 300, 560, 370, 410, 370 ,m_tex[TitleScene::PVP], true);

	if (m_drawT > 1 && m_drawT < 2)
	{
		if (GetType())
		{
			DrawModiGraph(m_point[0][0],
						  m_point[0][1],
						  m_point[0][2],
						  m_point[0][1],
						  m_point[0][2],
						  m_point[0][3],
						  m_point[0][0],
						  m_point[0][3],
						  m_tex[TitleScene::ENTER], true);
		}
		else
		{
			DrawModiGraph(m_point[1][0],
						  m_point[1][1],
						  m_point[1][2],
						  m_point[1][1],
						  m_point[1][2],
						  m_point[1][3],
						  m_point[1][0],
						  m_point[1][3],
						  m_tex[TitleScene::A_BUTTON], true);
		}
	}
	else if(m_drawT >= 2)
	{
		m_drawT = 0;
	}

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 150);

	DrawModiGraph(m_choicePoint[0] + m_choiceRand[0] + m_choice * 150,
				  m_choicePoint[1] + m_choiceRand[1],
				  m_choicePoint[2] + m_choiceRand[2] + m_choice * 150,
				  m_choicePoint[3] + m_choiceRand[3],
				  m_choicePoint[4] + m_choiceRand[4] + m_choice * 150,
				  m_choicePoint[5] + m_choiceRand[5],
				  m_choicePoint[6] + m_choiceRand[6] + m_choice * 150,
				  m_choicePoint[7] + m_choiceRand[7],
				  m_tex[TitleScene::CHOICE], true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
}

