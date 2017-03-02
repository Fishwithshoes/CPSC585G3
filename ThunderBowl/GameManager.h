#pragma once
class GameManager
{
public:
	double gameTimer;

	enum gameState
	{
		GS_MENUS,
		GS_IN_GAME,
		GS_GAME,
	};
};

