#include "AIMgr.h"
#include "AIStateBase.h"

void AIMgr::ChageState(AIStateBase * state, MinoMgr* minoMgr)
{
	if (m_AIState == nullptr) return;
	delete m_AIState;
	m_AIState = nullptr;
	m_AIState = state;
}
