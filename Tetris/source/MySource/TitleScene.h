#pragma once

#include "SceneStateBase.h"

class TitleScene : public SceneStateBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	enum TEXTURE
	{
		TITLE,
		ENTER,
		A_BUTTON,
		BG,
		Normal,
		PVE,
		PVP,
		CHOICE,
		MAX,
	};
	int m_sHandle;
	int m_tex[MAX];
	float m_drawT;
	int m_point[2][4];
	int m_titlePoint[2];
	const int MAXRAND = 8;
	float m_choiceT;
	int m_choiceRand[16];
	int m_choice;
	int m_choicePoint[8];
};