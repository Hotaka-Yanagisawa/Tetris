#pragma once

class Score
{
public:
	Score();
	~Score();

	void Init();
	void Uninit();
	void Draw();
	int GetScore()const { return m_nScore; }
	void SetScore(int score) { m_nScore = score; }
	void AddScore(int score) { m_nScore += score; }

	void SetPoint(int point[2])
	{
		m_point[0] = point[0];
		m_point[1] = point[1];
	}
	void SetPoint(int a, int b)
	{
		m_point[0] = a;
		m_point[1] = b;
	}
	void SetExist(bool exist)
	{
		m_exist = exist;
	}
	bool GetExist()
	{
		return m_exist;
	}

	void SetCr(int Cr) { m_Cr = Cr; }
private:
	int m_point[2];
	int m_nScore;
	int m_Cr;
	bool m_exist;
};
