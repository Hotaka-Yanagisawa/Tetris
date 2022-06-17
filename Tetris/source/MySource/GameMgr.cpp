#include "GameMgr.h"
#include "SceneMgr.h"
#include "ResultScene.h"
#include "DxLib.h"

void GameMgr::Init()
{
	m_score[0].Init();
	m_score[1].Init();
	m_isGame = true;
	m_time = 0;
	m_tex = LoadGraph("Assets/GameOver.png");
	m_point[0] = 150;
	m_point[1] = 200;
}

void GameMgr::Uninit()
{
	m_score[0].Uninit();
	m_score[1].Uninit();
}

void GameMgr::Update()
{
	if (!m_isGame)
	{
		if (m_time < 1)
		{
			m_time += 1.0f / 60.0f;
		}
		else
		{
			m_time = 0;
		
			SceneMgr::GetInstance()->ChageScene(SceneMgr::Scene::Result);
		}
	}
}

void GameMgr::Draw()
{
	if (!m_isGame)
	{
		DrawGraph(m_point[0], m_point[1], m_tex, true);
	}
	else
	{
		if (m_score[0].GetExist())
		{
			m_score[0].Draw();
		}
		if (m_score[1].GetExist())
		{
			m_score[1].Draw();
		}

		//ボード上のお邪魔表示
		for (int i = 0; i < m_breakLine[0]; ++i)
		{
			DrawBox(440 + i * 25, 30, 460 + i * 25, 50, GetColor(122, 122, 122), true);
		}
		for (int i = 0; i < m_breakLine[1]; ++i)
		{
			DrawBox(140 + i * 25, 30, 160 + i * 25, 50, GetColor(122, 122, 122), true);
		}
	}
}
