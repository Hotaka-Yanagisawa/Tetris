#pragma once

#include "SceneStateBase.h"

class SceneMgr
{
public:
	static SceneMgr* GetInstance()
	{
		static SceneMgr instance;
		return &instance;
	}

	enum Scene
	{
		Non,
		Title,
		Game,
		Result,
	};

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ChageScene(Scene scene);
	Scene GetScene()const { return m_Scene; }
	void SetScene(Scene scene) { m_Scene = scene; }
	void ReStart();
	GameType GetType()const { return m_type; }
	void SetType(GameType type) { m_type = type; }

private:
	Scene m_Scene;
	SceneStateBase* m_pSceneState;
	GameType m_type;
	
	SceneMgr() { m_Scene = Title;
	m_type = GameType::Normal;
	}
	~SceneMgr(){}

};
