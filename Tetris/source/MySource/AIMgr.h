#pragma once

class AIStateBase;
class MinoMgr;

class AIMgr
{
public:
	static AIMgr* GetInstance()
	{
		static AIMgr instance;
		return &instance;
	}
	//enumƒNƒ‰ƒX
	enum AIState:int
	{
		Search,
		Spin,
		Move,
		MAX
	};


	AIState GetCurState()const { return m_curState; }
	bool GetAI() const { return m_isAI; }
	void SetAI(bool flag) { m_isAI = flag; }
	AIStateBase* GetAIState()const { return m_AIState; }
	void SetAIState(AIStateBase* state) { m_AIState = state; }

	void ChageState(AIStateBase* state, MinoMgr* minoMgr);
private:
	AIStateBase* m_AIState;
	AIState m_curState;
	AIState m_oldState;
	bool m_isAI;
	//‰Šú‰»‚µ‚Ä‚¨‚­
	AIMgr():m_isAI(false),m_oldState(MAX),m_curState(MAX),m_AIState(nullptr) {}
	~AIMgr() {}

};
