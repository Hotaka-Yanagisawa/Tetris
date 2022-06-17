#include "MoveState.h"
#include "MinoMgr.h"
#include "AIMgr.h"
#include "SearchState.h"

MoveState::MoveState():m_timer(0)
{
}

MoveState::~MoveState()
{
}

void MoveState::Update(MinoMgr *pObj)
{
	//Œo˜H’Tõ“I‚È‚Ì‚ðì‚é
	const float SPD = 2.0f;

	//y,x
	int curPoint[2];
	int disX = 0;
	curPoint[0] = pObj->GetCurMino()->GetPoint(0);
	curPoint[1] = pObj->GetCurMino()->GetPoint(1);
	pObj->GetCurMino()->SetAngle(m_angle);
	pObj->GetCurMino()->Spin(m_angle, pObj);

	//ˆÚ“®‚Å‚«‚È‚¯‚ê‚Î’Tõ‚µ’¼‚µ

	//Œ»Ý‚ÌˆÊ’u‚Æ–Ú“I‚ÌˆÊ’u‚Ì‹——£
	disX = m_point[1] - curPoint[1];
	
	if (disX > 0)
	{
		m_timer += 1.0f / SPD;
		if (m_timer > 1)
		{
			m_timer = 0;
			pObj->RightMove();
		}
	}
	else if (disX < 0)
	{
		m_timer += 1.0f / SPD;
		if (m_timer > 1)
		{
			m_timer = 0;
			pObj->LeftMove();
		}
	}
	else if(disX == 0)
	{
		m_timer += 1.0f / SPD;
		if (m_timer > 1)
		{
			m_timer = 0;
			//‰º‚É“®‚©‚·ˆ—
			pObj->Fall();
			pObj->HardDrop();
			AIMgr::GetInstance()->ChageState(new SearchState(), pObj);
		}
	}
}
