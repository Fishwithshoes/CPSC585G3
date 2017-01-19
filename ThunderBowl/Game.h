#pragma once
#include "CommonIncludes.h"
#include "GameObject.h"

class Game
{
public:
	static vector<GameObject> worldObjectList;//For objects that appear physically in the world
	static vector<GameObject> particleObjectList;//For particle systems which spawn particles
	static vector<GameObject> overlayObjectList;//For menu and HUD elements to draw atop all else

	static void BuildWorld();

	static void DestroyWorld();

protected:
	
private:
};
