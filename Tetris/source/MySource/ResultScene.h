#pragma once

#include "SceneStateBase.h"

class ResultScene : public SceneStateBase
{
public:
	ResultScene();
	virtual~ResultScene();

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;


	enum TEXTURE
	{
		TITLE,
		RETRY,
		END,
		BG,
		CHOICE_1,
		CHOICE_2,
		MAX,
	};

private:
	const int MAXRAND =8;
	float m_timer;
	float m_choiceT;
	int m_choiceRand[16];
	int m_choice;
	int m_choicePoint[8];
	int m_sHandle;
	int m_tex[MAX];
};