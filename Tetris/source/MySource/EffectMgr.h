#pragma once

class ComboParticle;
class DropAnime;
class Wave;

class EffectMgr
{
public:

	EffectMgr();
	~EffectMgr();

	enum eEffect
	{
		COMBO_PARTICLE,
		DROP_ANIME,
		WAVE,
		MAX
	};

	void Init();
	void Update();
	void Draw();
	void CreateEffect(int x, int y, eEffect effect, int num = 0);

private:
	ComboParticle* m_comboParticle;
	DropAnime* m_dropAnime;
	Wave* m_wave;
};