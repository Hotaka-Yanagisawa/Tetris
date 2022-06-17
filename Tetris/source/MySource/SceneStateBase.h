#pragma once

//class SceneMgr;
enum GameType
{
	Normal,
	PVE,
	PVP,
	TYPE_MAX,
};

class SceneStateBase
{
public:

	virtual ~SceneStateBase() {}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

