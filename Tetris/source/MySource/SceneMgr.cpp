#include "SceneMgr.h"
#include "GameScene.h"
#include "DxLib.h"
#include "TitleScene.h"
#include "ResultScene.h"

void SceneMgr::Init()
{
	if (m_pSceneState == nullptr)
	{
		m_pSceneState = new TitleScene();
	}
}

void SceneMgr::Uninit()
{
	if (m_pSceneState != nullptr)
	{
		m_pSceneState->Uninit();
	}
}

void SceneMgr::Update()
{
	if (m_pSceneState != nullptr)
	{
		m_pSceneState->Update();
	}
}

void SceneMgr::Draw()
{
	if (m_pSceneState != nullptr)
	{
		m_pSceneState->Draw();
	}
}

void SceneMgr::ChageScene(Scene scene)
{
	m_Scene = scene;

	int c = 0, White;

	White = GetColor(255, 255, 255);    //�F�̎擾

	while (1)
	{
		ClearDrawScreen();                   // ����ʂ̃f�[�^��S�č폜
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SceneMgr::GetInstance()->Draw();
		if (c >= 0 && c < 256)
		{//c��0~256�̎�
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, c);
		}
		if (c >= 255 && c < 259)
		{
			m_pSceneState->Uninit();
			delete m_pSceneState;
			m_pSceneState = nullptr;
			switch (scene)
			{
			case SceneMgr::Title:
				m_pSceneState = new TitleScene();
				break;
			case SceneMgr::Game:
				m_pSceneState = new GameScene(m_type);
				break;
			case SceneMgr::Result:
				m_pSceneState = new ResultScene();
				break;
			default:
				break;
			}
		}
		if (c >= 256 && c < 400)
		{//c��256~400�̎�
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 255);
		}
		if (c >= 400 && c < 400 + 256)
		{//c��400~400+256�̎�
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 255 - (c - 400));
		}

		DrawBox(0, 0, 640, 480, White, TRUE);      //��ʑS�̂ɔ����l�p�`��`��

		c += 4;                                    //c���J�E���g�A�b�v

		if (c > 400 + 255) break;                  //�Â��Ȃ�I�������I��
		ScreenFlip();                              // ����ʃf�[�^��\��ʂ֔��f
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneMgr::ReStart()
{
	if (m_pSceneState != nullptr)
	{
		m_pSceneState->Init();
	}
}
