#pragma once


class DropAnime
{
public:
	DropAnime();
	~DropAnime();

	struct AnimeParam
	{
		int animIdx;
		int point[2];
		float timer;
		bool exist;
	};

	void Init();
	void Update();
	void Draw();
	void Create(int x, int y);

private:
	static const int ANIM_MAX = 8;

	AnimeParam m_animeParam;

	int m_animTex[ANIM_MAX];
};