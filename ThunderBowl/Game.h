#pragma once
#include "CommonIncludes.h"
#include "GameObject.h"

class Game
{
public:
	static vector<GameObject> worldObjectList;//For objects that appear physically in the world
	static vector<GameObject> particleObjectList;//For particle systems which spawn particles
	static vector<GameObject> overlayObjectList;//For menu and HUD elements to draw atop all else

	static void BuildWorld();//Reserves space in vectors and adds initial objects
	static void DestroyWorld();//Empties vectors, destroying all objects inside

	static void CreateWorldObject(GameObject object);
	static void CreateParticleObject(GameObject object);
	static void CreateOverlayObject(GameObject object);

	static void DestroyWorldObject(GameObject object);
	static void DestroyParticleObject(GameObject object);
	static void DestroyOverlayObject(GameObject object);

protected:
	
private:
};
