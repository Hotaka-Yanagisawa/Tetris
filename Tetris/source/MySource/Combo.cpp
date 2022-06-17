#include "Combo.h"
#include "DxLib.h"

Combo::Combo():m_nCurCombo(0),m_nOldCombo(0)
{
}

Combo::~Combo()
{
}

void Combo::Init()
{
	m_nCurCombo = m_nOldCombo = 0;
	m_point[0] = 10;
	m_point[1] = 430;
	m_Cr = GetColor(255, 0, 0);
}

void Combo::Update()
{
	if (m_nCurCombo != m_nOldCombo)
	{
		m_nOldCombo = m_nCurCombo;
	}
	else
	{
		m_nCurCombo = m_nOldCombo = 0;
	}
}

void Combo::Draw()
{
	DrawFormatString(m_point[0], m_point[1], m_Cr, "ƒRƒ“ƒ{:%d", m_nCurCombo);
}

void Combo::AddCombo()
{
	++m_nCurCombo;
}
