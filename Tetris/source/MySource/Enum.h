#pragma once

namespace Enums
{
	enum SquwareState
	{
		Non,
		Move,
		Stop,
		Fall,//���C���������̎g��
		Forecast,
		TENTATIVE,
		MAX,
	};

	enum Actor
	{
		PLAYER,
		ENEMY,
		MAX_ACTOR,
	};
};