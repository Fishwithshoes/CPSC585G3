#include "Game.h"
#include "GeoGenerator.h"

vector<GameObject> Game::worldObjectList = {};
vector<GameObject> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};

void Game::BuildWorld()
{
	GameObject temp = GameObject();

	temp.mesh = GeoGenerator::MakeSphere(1, 8, 16);

	worldObjectList.push_back(temp);
}

void Game::DestroyWorld()
{

}