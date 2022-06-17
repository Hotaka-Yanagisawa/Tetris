#include "ComboParticle.h"
#include "DxLib.h"

ComboParticle::ComboParticle()
{
	m_cr = GetColor(255, 255, 255);

	for (int i = 0; i < MAX_PARTICLE; ++i)
	{
		m_particle[i].exist = false;
		m_particle[i].moveSpd[0] = 0;
		m_particle[i].moveSpd[1] = 0;
		m_particle[i].point[0] = -999;
		m_particle[i].point[1] = -999;
		m_particle[i].r = 0;
		m_particle[i].timer = 0;
	}
}

ComboParticle::~ComboParticle()
{
}


void ComboParticle::Update()
{
	for (int i = 0; i < MAX_PARTICLE; ++i)
	{
		if (!m_particle[i].exist) continue;
		m_particle[i].exist = true;
		m_particle[i].point[0] += m_particle[i].moveSpd[0];
		m_particle[i].point[1] += m_particle[i].moveSpd[1];
		m_particle[i].timer -= 1.0f / 3.0f;
		if (m_particle[i].timer <= 0)
		{
			m_particle[i].exist = false;
		}
	}
}

void ComboParticle::Draw()
{
	for (int i = 0; i < MAX_PARTICLE; ++i)
	{
		if (!m_particle[i].exist) continue;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, rand() % 256);

		DrawCircle((int)m_particle[i].point[0],
			(int)m_particle[i].point[1],
			m_particle[i].r,
			m_cr,
			true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ComboParticle::Create(int x, int y, int num)
{
	int createNum = 0;

	if (num == 2)
	{
		createNum = 20;
	}
	if (num == 3)
	{
		createNum = 50;
	}
	if (num > 3)
	{
		createNum = 100;
	}

	for (int i = 0; i < createNum && i < MAX_PARTICLE; ++i)
	{
		if (m_particle[i].exist) continue;
		m_particle[i].exist = true;
		//-150~150�̗��������߂�v�Z
		m_particle[i].point[0] = x + (rand() % 3001 - 1500) * 0.1f;
		//0~0.8�̗��������߂�
		m_particle[i].point[1] = y + (rand() % 8) * 0.1f;
		//-5~5�̗��������߂�
		m_particle[i].moveSpd[0] = ((rand() % 101) - 50) * 0.1f;
		//0.1~10�̗��������߂�
		m_particle[i].moveSpd[1] = -(rand() % 100) * 0.1f;
		//�p�[�e�B�N���̏����鎞�Ԃ����߂�
		m_particle[i].timer = (float)((rand() % 50 + i * 0.5f)) / 8.0f + 0.1f;
		if (num == 2)
		{
			m_particle[i].r = 1;
		}
		if (num == 3)
		{
			m_particle[i].r = rand() % 2 + 1;
		}
		if (num > 3)
		{
			m_particle[i].r = rand() % 3 + 1;
		}
	}
}
