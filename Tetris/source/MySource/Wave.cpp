#include "Wave.h"
#include "DxLib.h"
#include "SceneMgr.h"

Wave::Wave()
{
	for (int i = 0; i < MAX_LINE; ++i)
	{
		for (int j = 0; j < AFTER_IMAGE; ++j)
		{
			for (int k = 0; k < VERTEX_NUM; ++k)
			{
				m_arrow[i][j].point[k] = -999;
			}
			m_arrow[i][j].movePoint = -99;
			m_arrow[i][j].exist = false;
		}
	}
	m_tex = LoadGraph("Assets/arrow.png");
	
}

Wave::~Wave()
{
	DeleteGraph(m_tex);
}


void Wave::Init()
{
	m_tex = LoadGraph("Assets/arrow.png");
}

void Wave::Update()
{
	for (int i = 0; i < MAX_LINE; ++i)
	{
		for (int j = 0; j < AFTER_IMAGE; ++j)
		{
			if (!m_arrow[i][j].exist) continue;

			for (int k = 0; k < VERTEX_NUM; k += 2)
			{
				m_arrow[i][j].point[k] += MOVE_SPD;
			}
			if (m_arrow[i][j].point[2] > m_arrow[i][j].movePoint)
			{
				m_arrow[i][j].exist = false;
			}
		}
	}
}

void Wave::Draw()
{
	const int DISTANCE = 15;
	for (int i = 0; i < MAX_LINE; ++i)
	{
		for (int j = 0; j < AFTER_IMAGE; ++j)
		{
			if (!m_arrow[i][j].exist) continue;

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (j + 1) * (255 / AFTER_IMAGE));

			DrawModiGraph(m_arrow[i][j].point[0] + j * DISTANCE,	//左上
						  m_arrow[i][j].point[1],
						  m_arrow[i][j].point[2] + j * DISTANCE,	//右上
						  m_arrow[i][j].point[3],
						  m_arrow[i][j].point[4] + j * DISTANCE,	//右下
						  m_arrow[i][j].point[5],
						  m_arrow[i][j].point[6] + j * DISTANCE,	//左下
						  m_arrow[i][j].point[7],
						  m_tex, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Wave::Create(int x, int y)
{
	const int PIECESIZE = 20;
	bool isEnd = false;			//1列だけ生成するためのフラグ
	for (int i = 0; i < MAX_LINE; ++i)
	{
		for (int j = 0; j < AFTER_IMAGE; ++j)
		{
			if (m_arrow[i][j].exist) continue;

			m_arrow[i][j].exist = true;
			//中心座標を入れる
			m_arrow[i][j].movePoint = x + 100;
			m_arrow[i][j].point[0] = x;
			m_arrow[i][j].point[1] = y;
			m_arrow[i][j].point[2] = x + PIECESIZE;
			m_arrow[i][j].point[3] = y;
			m_arrow[i][j].point[4] = x + PIECESIZE;
			m_arrow[i][j].point[5] = y + PIECESIZE;
			m_arrow[i][j].point[6] = x;
			m_arrow[i][j].point[7] = y + PIECESIZE;
			isEnd = true;
		}
		if (isEnd) return;
	}
}
