#pragma once


class Wave
{
private:
	static const int MOVE_SPD = 16;
	static const int MAX_LINE = 8;
	static const int AFTER_IMAGE = 10;
	static const int VERTEX_NUM = 8;
public:
	Wave();
	~Wave();

	struct Arrow
	{
		int  point[VERTEX_NUM];			
		int  movePoint;			//エフェクトを消す目印
		bool exist;				//falseなら削除
	};
	void Init();
	void Update();
	void Draw();
	void Create(int x, int y);

private:
	Arrow  m_arrow[MAX_LINE][AFTER_IMAGE];

	int  m_tex;
};