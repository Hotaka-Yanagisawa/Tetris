#pragma once

class MinoMgr;

class AIStateBase
{
public:

	virtual ~AIStateBase() {}
	virtual void Update(MinoMgr*) = 0;
};

