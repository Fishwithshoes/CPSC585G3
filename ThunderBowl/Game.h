#pragma once
#include "CommonIncludes.h"
#include "GameObject.h"

class Game
{
public:
	static GameObject skybox;//The environment skybox
	static vector<GameObject> worldObjectList;//For objects that appear physically in the world
	static vector<GameObject> particleObjectList;//For particle systems which spawn particles
	static vector<GameObject> overlayObjectList;//For menu and HUD elements to draw atop all else

	static void BuildWorld();//Reserves space in vectors and adds initial objects
	static void DestroyWorld();//Empties vectors, destroying all objects inside

	static GameObject* CreateWorldObject(GameObject object);
	static GameObject* CreateParticleObject(GameObject object);
	static GameObject* CreateOverlayObject(GameObject object);

	static void DestroyWorldObject(GameObject object);
	static void DestroyWorldObjectAt(int objectID);
	static void DestroyParticleObject(GameObject object);
	static void DestroyOverlayObject(GameObject object);

	static GameObject* Find(string name);//Returns first one matching name
	static vector<GameObject*> FindGameObjectsWithTag(Tags tag);

protected:
	
private:
};
