#pragma once
//#include "Board.h"

class MinoMgr;


enum MinoKind
{
	I_MINO,
	O_MINO,
	S_MINO,
	Z_MINO,
	J_MINO,
	L_MINO,
	T_MINO,
	MAX_MINO,
};

enum Angle
{
	ANGLE_0,
	ANGLE_90,
	ANGLE_180,
	ANGLE_270,
	ANGLE_MAX,
};

class Mino
{
public:
	Mino();
	Mino(MinoKind kind);
	~Mino();


	void Init(MinoMgr* minoMgr);
	bool Spin(Angle, MinoMgr* minoMgr);

	int GetCr() const { return m_Cr; }
	int GetTetMino(int idx1, int idx2)
	{
		return m_tetMino[idx1][idx2];
	}

	int GetPoint(int idx)
	{
		return m_point[idx];
	}

	void SetPoint(int point[2])
	{
		m_point[0] = point[0];
		m_point[1] = point[1];
	}
	void SetPoint(int a, int b)
	{
		m_point[0] = a;
		m_point[1] = b;
	}

	void SetAngle(Angle angle)
	{
		m_angle = angle;
	}

	Angle GetAngle()
	{
		return m_angle;
	}

	MinoKind GetMinoKind()const { return m_minoKind; }

private:
	int m_Cr;
	MinoKind m_minoKind;
	int m_point[2];
	int m_tetMino[4][4];
	Angle m_angle;
};
