#include "Score.h"
#include "DxLib.h"

Score::Score() :m_nScore(0), m_Cr(0), m_exist(false)
{
	m_point[0] = m_point[1] = 0;
}

Score::~Score()
{
}

void Score::Init()
{
	m_nScore = 0;
}

void Score::Uninit()
{
	m_exist = false;
}


void Score::Draw()
{
	//テキストの描画
	DrawFormatString(m_point[0], m_point[1], m_Cr, "スコア:%d", m_nScore);
}
