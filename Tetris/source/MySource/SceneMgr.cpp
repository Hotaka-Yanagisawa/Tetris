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

	White = GetColor(255, 255, 255);    //色の取得

	while (1)
	{
		ClearDrawScreen();                   // 裏画面のデータを全て削除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SceneMgr::GetInstance()->Draw();
		if (c >= 0 && c < 256)
		{//cが0~256の時
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
		{//cが256~400の時
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 255);
		}
		if (c >= 400 && c < 400 + 256)
		{//cが400~400+256の時
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, 255 - (c - 400));
		}

		DrawBox(0, 0, 640, 480, White, TRUE);      //画面全体に白い四角形を描画

		c += 4;                                    //cをカウントアップ

		if (c > 400 + 255) break;                  //暗くなり終わったら終了
		ScreenFlip();                              // 裏画面データを表画面へ反映
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
