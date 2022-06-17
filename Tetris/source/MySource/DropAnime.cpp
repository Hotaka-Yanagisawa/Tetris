#include "DropAnime.h"
#include "DxLib.h"

DropAnime::DropAnime()
{
	LoadDivGraph("Assets/effect_1.png", 8, 8, 1, 120, 120, m_animTex);

	m_animeParam.animIdx = 0;
	m_animeParam.exist = false;
	m_animeParam.point[0] = -999;
	m_animeParam.point[1] = -999;
	m_animeParam.timer = 0;
}

DropAnime::~DropAnime()
{
	for (int i = 0; i < ANIM_MAX; i++)
	{
		DeleteGraph(m_animTex[i]);
	}
}

void DropAnime::Init()
{
	LoadDivGraph("Assets/effect_1.png", 8, 8, 1, 120, 120, m_animTex);
}

void DropAnime::Update()
{
	//アニメーションの更新
	if (m_animeParam.exist)
	{
		m_animeParam.timer += 1.0f / 3.0f;
		if (m_animeParam.timer > 1)
		{
			m_animeParam.timer = 0;
			++m_animeParam.animIdx;
			if (m_animeParam.animIdx >= ANIM_MAX)
			{
				m_animeParam.exist = false;
			}
		}
	}
}

void DropAnime::Draw()
{
	if (m_animeParam.exist)
	{
		DrawGraph(m_animeParam.point[0] - 20, m_animeParam.point[1] - 50, m_animTex[m_animeParam.animIdx], true);
	}
}

void DropAnime::Create(int x, int y)
{
	m_animeParam.exist = true;
	m_animeParam.point[0] = x;
	m_animeParam.point[1] = y;
	m_animeParam.animIdx = 0;
}
