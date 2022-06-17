#pragma once

class Combo
{
public:
	Combo();
	~Combo();

	void Init();
	void Update();
	void Draw();
	void AddCombo();
	int GetCombo() { return m_nCurCombo; }
	int GetPoint(int idx)const
	{
		return m_point[idx];
	}
	int GetCr() const { return m_Cr; }

	void SetPoint(int x, int y)
	{
		m_point[0] = x;
		m_point[1] = y;
	}
	void SetCr(int cr)
	{
		m_Cr = cr;
	}
private:
	int m_nCurCombo;
	int m_nOldCombo;
	int m_point[2];
	int m_Cr;
};
