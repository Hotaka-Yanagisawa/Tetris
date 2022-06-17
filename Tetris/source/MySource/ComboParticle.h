#pragma once



class ComboParticle
{
public:
	ComboParticle();
	~ComboParticle();

	struct Particle
	{
		int r;
		float point[2];
		float moveSpd[2];
		bool exist;
		float timer;
		Particle() :r(0), point{ 0, 0 }, moveSpd{ 0, 0 }, exist(false), timer(0)
		{
		}
	};

	void Update();
	void Draw();
	void Create(int x, int y, int num);

private:
	static const int MAX_PARTICLE = 200;
	Particle m_particle[MAX_PARTICLE];
	int m_cr;
};