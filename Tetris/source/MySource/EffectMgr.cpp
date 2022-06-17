#include "EffectMgr.h"
#include "ComboParticle.h"
#include "DropAnime.h"
#include "Wave.h"
#include "GameMgr.h"

EffectMgr::EffectMgr() :m_comboParticle(nullptr), m_dropAnime(nullptr), m_wave(nullptr)
{
	if (m_comboParticle == nullptr)
	{
		m_comboParticle = new ComboParticle();
	}	
	if (m_dropAnime == nullptr)
	{
		m_dropAnime = new DropAnime();
	}
	if (m_wave == nullptr)
	{
		m_wave= new Wave();
	}
}

EffectMgr::~EffectMgr()
{
	if (m_comboParticle != nullptr)
	{
		delete m_comboParticle;
		m_comboParticle = nullptr;
	}
	if (m_dropAnime != nullptr)
	{
		delete m_dropAnime;
		m_dropAnime = nullptr;
	}
	if (m_wave != nullptr)
	{
		delete m_wave;
		m_wave = nullptr;
	}
}

void EffectMgr::Init()
{
	if (m_dropAnime != nullptr)
	{
		m_dropAnime->Init();
	}
	if (m_wave != nullptr)
	{
		m_wave->Init();
	}
}

void EffectMgr::Update()
{
	if (m_comboParticle != nullptr)
	{
		m_comboParticle->Update();
	}
	if (m_dropAnime != nullptr)
	{
		m_dropAnime->Update();
	}
	if (m_wave != nullptr)
	{
		m_wave->Update();
	}
}

void EffectMgr::Draw()
{
	if (m_comboParticle != nullptr)
	{
		m_comboParticle->Draw();
	}
	if (m_dropAnime != nullptr)
	{
		m_dropAnime->Draw();
	}
	if (m_wave != nullptr)
	{
		m_wave->Draw();
	}
}

void EffectMgr::CreateEffect(int x, int y, eEffect effect, int num)
{
	switch (effect)
	{
	case EffectMgr::COMBO_PARTICLE:
		if (m_comboParticle != nullptr)
		{
			m_comboParticle->Create(x, y, num);
		}
		break;

	case EffectMgr::DROP_ANIME:
		if (m_dropAnime != nullptr)
		{
			m_dropAnime->Create(x, y);
		}
		break;

	case EffectMgr::WAVE:
		if (m_wave != nullptr)
		{
			m_wave->Create(x, y);
		}
		break;
	}
}