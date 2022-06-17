#pragma once

namespace Enums
{
	enum SquwareState
	{
		Non,
		Move,
		Stop,
		Fall,//ライン処理時の使う
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