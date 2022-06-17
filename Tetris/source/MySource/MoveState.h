#pragma once

#include "AIStateBase.h"
//#include "Board.h"
#include "Mino.h"

class Mino;

class MoveState : public AIStateBase
{
public:
	MoveState();
	virtual ~MoveState();

	virtual void Update(MinoMgr*) override;

	void SetPoint(int a, int b) {
		m_point[0] = a;
		m_point[1] = b;
	}
	void SetAngle(Angle angle) { m_angle = angle; };
private:
	int m_point[2];
	Angle m_angle;
	float m_timer;
};