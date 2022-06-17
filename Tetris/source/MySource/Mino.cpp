#include "Mino.h"
#include "input.h"
#include "DxLib.h"
#include "Enum.h"
#include "MinoMgr.h"

using namespace Enums;


Mino::Mino()
{
}

Mino::Mino(MinoKind kind)
	: m_minoKind(kind)
	,m_Cr(GetColor(255, 255, 255))
	, m_tetMino{0,0,0,0,
				0,0,0,0,
				0,0,0,0,
				0,0,0,0}
	, m_angle(Angle::ANGLE_0)
{	
	switch (m_minoKind)
	{
	case I_MINO:
	{
		int I_Mino[4][4] =
		{ 0,1,0,0,
		  0,1,0,0,
		  0,1,0,0,
		  0,1,0,0 };

		memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(0, 255, 255);
	}
	break;
	case O_MINO:
	{
		int O_Mino[4][4] =
		{ 0,0,0,0,
		  0,1,1,0,
		  0,1,1,0,
		  0,0,0,0 };

		memcpy(m_tetMino, O_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(255, 255, 0);
	}
	break;
	case S_MINO:
	{
		int S_Mino[4][4] =
		{ 0,0,0,0,
		  0,1,1,0,
		  1,1,0,0,
		  0,0,0,0 };

		memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(153, 255, 51);
	}
	break;
	case Z_MINO:
	{
		int Z_Mino[4][4] =
		{ 0,0,0,0,
		  1,1,0,0,
		  0,1,1,0,
		  0,0,0,0 };

		memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(255, 51, 51);
	}
	break;
	case J_MINO:
	{
		int J_Mino[4][4] =
		{ 0,0,1,0,
		  0,0,1,0,
		  0,1,1,0,
		  0,0,0,0 };

		memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(0, 0, 204);
	}
	break;
	case L_MINO:
	{
		int L_Mino[4][4] =
		{ 0,1,0,0,
		  0,1,0,0,
		  0,1,1,0,
		  0,0,0,0 };

		memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(255, 178, 102);
	}
	break;
	case T_MINO:
	{
		int T_Mino[4][4] =
		{ 0,0,0,0,
		  1,1,1,0,
		  0,1,0,0,
		  0,0,0,0 };

		memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));

		m_Cr = GetColor(255, 5, 255);
	}
	break;
	}
	m_point[0] = 0;
	m_point[1] = 13;
}

Mino::~Mino()
{

}

void Mino::Init(MinoMgr* minoMgr)
{
	m_point[0] = 3;
	m_point[1] = 4;
	int addY = 0;
	bool isHit = true;
	while (isHit)
	{
		isHit = false;
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (m_tetMino[y][x] == 1 && (addY + y) > -1)
				{
					if (minoMgr->GetSquware(m_point[0] + y + addY, m_point[1] + x) == SquwareState::Stop)
					{
						isHit = true;
						break;
					}
				}
			}
		}
		if (isHit)
		{
			addY -= 1;
		}
	}
	m_point[0] += addY;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (m_tetMino[y][x] == 1 && (m_point[0] + y) > -1)
			{
				minoMgr->SetSquware(m_point[0] + y, m_point[1] + x, SquwareState::Move);
			}
		}
	}
}


bool Mino::Spin(Angle angle, MinoMgr* minoMgr)
{
	switch (m_minoKind)
	{
		//----I
	case I_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int I_Mino[4][4] =
			{ 0,1,0,0,
			  0,1,0,0,
			  0,1,0,0,
			  0,1,0,0 };
			memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int I_Mino[4][4] =
			{ 0,0,0,0,
			  0,0,0,0,
			  1,1,1,1,
			  0,0,0,0 };
			memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
		}
			break;

		case ANGLE_180:
		{
			int I_Mino[4][4] =
			{ 0,0,1,0,
			  0,0,1,0,
			  0,0,1,0,
			  0,0,1,0 };
			memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
		}
			break;

		case ANGLE_270:
		{
			int I_Mino[4][4] =
			{ 0,0,0,0,
			  1,1,1,1,
			  0,0,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
		}
			break;

		}
		break;
		//----O
	case O_MINO:
		break;
		//----S
	case S_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int S_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,1,0,
			  1,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int S_Mino[4][4] =
			{ 0,1,0,0,
			  0,1,1,0,
			  0,0,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_180:
		{
			int S_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,1,0,
			  1,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_270:
		{
			int S_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,0,0,
			  0,1,1,0,
			  0,0,1,0 };
			memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
		}
		break;
		}
		break;

		//----Z
	case Z_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int Z_Mino[4][4] =
			{ 0,0,0,0,
			  1,1,0,0,
			  0,1,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int Z_Mino[4][4] =
			{ 0,0,0,0,
			  0,0,1,0,
			  0,1,1,0,
			  0,1,0,0 };
			memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_180:
		{
			int Z_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,1,0,
			  0,0,1,1,
			  0,0,0,0 };
			memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_270:
		{
			int Z_Mino[4][4] =
			{ 0,0,1,0,
			  0,1,1,0,
			  0,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
		}
		break;
		}
		break;

		//----J
	case J_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int J_Mino[4][4] =
			{ 0,0,1,0,
			  0,0,1,0,
			  0,1,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int J_Mino[4][4] =
			{ 0,0,0,0,
			  1,1,1,0,
			  0,0,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_180:
		{
			int J_Mino[4][4] =
			{ 0,1,1,0,
			  0,1,0,0,
			  0,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_270:
		{
			int J_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,0,0,
			  0,1,1,1,
			  0,0,0,0 };
			memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
		}
		break;
		}
		break;

		//----L
	case L_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int L_Mino[4][4] =
			{ 0,1,0,0,
			  0,1,0,0,
			  0,1,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int L_Mino[4][4] =
			{ 0,0,0,0,
			  0,0,1,0,
			  1,1,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_180:
		{
			int L_Mino[4][4] =
			{ 0,1,1,0,
			  0,0,1,0,
			  0,0,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_270:
		{
			int L_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,1,1,
			  0,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
		}
		break;
		}
		break;
		//----T
	case T_MINO:
		switch (angle)
		{
		case ANGLE_0:
		{
			int T_Mino[4][4] =
			{ 0,0,0,0,
			  1,1,1,0,
			  0,1,0,0,
			  0,0,0,0 };
			memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_90:
		{
			int T_Mino[4][4] =
			{ 0,0,0,0,
			  0,1,0,0,
			  0,1,1,0,
			  0,1,0,0 };
			memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_180:
		{
			int T_Mino[4][4] =
			{ 0,0,0,0,
			  0,0,1,0,
			  0,1,1,1,
			  0,0,0,0 };
			memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
		}
		break;

		case ANGLE_270:
		{
			int T_Mino[4][4] =
			{ 0,0,1,0,
			  0,1,1,0,
			  0,0,1,0,
			  0,0,0,0 };
			memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
		}
		break;
		}
		break;
	}


	bool isHit = false;	//êgìÆÇ´Ç™éÊÇÍÇ»Ç¢èÍçáÉtÉâÉOÇ™óßÇ¬
	//MinoMgr* ptr = MinoMgr::GetInstance();
	//âÒì]ÇµÇΩèÍèäÇ…é~Ç‹Ç¡ÇƒÇ¢ÇÈÉ~ÉmÇ™Ç†ÇÈÇ©Ç«Ç§Ç©
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			if (m_tetMino[y][x] == 1)
			{
				if (minoMgr->GetSquware(m_point[0] + y, m_point[1] + x) == SquwareState::Stop)
				{
					isHit = true;
				}
			}
		}
	}

	

	if (!isHit)
	{
		m_angle = angle;
		//åªç›ê∂Ç´ÇƒÇ¢ÇÈÉ~ÉmÇçÌèú
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if(minoMgr->GetSquware(m_point[0] + y, m_point[1] + x) == SquwareState::Move)
					minoMgr->SetSquware(m_point[0] + y, m_point[1] + x, SquwareState::Non);
			}
		}
		
		//êVÇΩÇ…âÒì]ÇµÇΩÉ~ÉmÇí«â¡
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				if (m_tetMino[y][x] == 1)
				{
					minoMgr->SetSquware(m_point[0] + y, m_point[1] + x, SquwareState::Move);
				}
			}
		}
		return true;
	}
	else
	{
		switch (m_minoKind)
		{
			//----I
		case I_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int I_Mino[4][4] =
				{ 0,1,0,0,
				  0,1,0,0,
				  0,1,0,0,
				  0,1,0,0 };
				memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int I_Mino[4][4] =
				{ 0,0,0,0,
				  0,0,0,0,
				  1,1,1,1,
				  0,0,0,0 };
				memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int I_Mino[4][4] =
				{ 0,0,1,0,
				  0,0,1,0,
				  0,0,1,0,
				  0,0,1,0 };
				memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int I_Mino[4][4] =
				{ 0,0,0,0,
				  1,1,1,1,
				  0,0,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, I_Mino, sizeof(m_tetMino));
			}
			break;

			}
			break;
			//----O
		case O_MINO:
			break;
			//----S
		case S_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int S_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,1,0,
				  1,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int S_Mino[4][4] =
				{ 0,1,0,0,
				  0,1,1,0,
				  0,0,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int S_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,1,0,
				  1,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int S_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,0,0,
				  0,1,1,0,
				  0,0,1,0 };
				memcpy(m_tetMino, S_Mino, sizeof(m_tetMino));
			}
			break;
			}
			break;

			//----Z
		case Z_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int Z_Mino[4][4] =
				{ 0,0,0,0,
				  1,1,0,0,
				  0,1,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int Z_Mino[4][4] =
				{ 0,0,0,0,
				  0,0,1,0,
				  0,1,1,0,
				  0,1,0,0 };
				memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int Z_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,1,0,
				  0,0,1,1,
				  0,0,0,0 };
				memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int Z_Mino[4][4] =
				{ 0,0,1,0,
				  0,1,1,0,
				  0,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, Z_Mino, sizeof(m_tetMino));
			}
			break;
			}
			break;

			//----J
		case J_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int J_Mino[4][4] =
				{ 0,0,1,0,
				  0,0,1,0,
				  0,1,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int J_Mino[4][4] =
				{ 0,0,0,0,
				  1,1,1,0,
				  0,0,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int J_Mino[4][4] =
				{ 0,1,1,0,
				  0,1,0,0,
				  0,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int J_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,0,0,
				  0,1,1,1,
				  0,0,0,0 };
				memcpy(m_tetMino, J_Mino, sizeof(m_tetMino));
			}
			break;
			}
			break;

			//----L
		case L_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int L_Mino[4][4] =
				{ 0,1,0,0,
				  0,1,0,0,
				  0,1,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int L_Mino[4][4] =
				{ 0,0,0,0,
				  0,0,1,0,
				  1,1,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int L_Mino[4][4] =
				{ 0,1,1,0,
				  0,0,1,0,
				  0,0,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int L_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,1,1,
				  0,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, L_Mino, sizeof(m_tetMino));
			}
			break;
			}
			break;
			//----T
		case T_MINO:
			switch (m_angle)
			{
			case ANGLE_0:
			{
				int T_Mino[4][4] =
				{ 0,0,0,0,
				  1,1,1,0,
				  0,1,0,0,
				  0,0,0,0 };
				memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_90:
			{
				int T_Mino[4][4] =
				{ 0,0,0,0,
				  0,1,0,0,
				  0,1,1,0,
				  0,1,0,0 };
				memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_180:
			{
				int T_Mino[4][4] =
				{ 0,0,0,0,
				  0,0,1,0,
				  0,1,1,1,
				  0,0,0,0 };
				memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
			}
			break;

			case ANGLE_270:
			{
				int T_Mino[4][4] =
				{ 0,0,1,0,
				  0,1,1,0,
				  0,0,1,0,
				  0,0,0,0 };
				memcpy(m_tetMino, T_Mino, sizeof(m_tetMino));
			}
			break;
			}
			break;
		}
	}
	return false;
}
