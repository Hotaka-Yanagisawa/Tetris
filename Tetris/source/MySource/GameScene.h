#pragma once

#include "SceneStateBase.h"
#include "Enum.h"
#include <memory>

class MinoMgr;
class EffectMgr;
class Board;

class GameScene : public SceneStateBase
{
public:
	GameScene(GameType type);
	virtual~GameScene();

	virtual void Init() override; 
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;


	enum TEXTURE
	{
		PLAY,
		RETRY,
		TITLE,
		END,
		CHOICE_1,
		MAX,
	};
	
private:
	void Pause();
	static const int MAXRAND = 8;
	static const int ADDPOINT = 60;
	MinoMgr* m_minoMgrP;
	MinoMgr* m_minoMgrE;
	std::shared_ptr<EffectMgr> m_effectMgr;
	std::shared_ptr<Board> m_Board[Enums::Actor::MAX_ACTOR];

	int m_tex[MAX];

	int m_point[4];

	float m_choiceT;
	int m_choiceRand[MAXRAND];
	int m_choice;
	int m_choicePoint[8];
};