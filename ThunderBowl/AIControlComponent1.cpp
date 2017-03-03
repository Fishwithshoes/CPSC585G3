#include "AIControlComponent1.h"
#include "AINodeComponent.h"
#include "Game.h"
#include "Physics.h"

vector<AINodeComponent*> currentAINodes;
AINodeComponent* aiNode;
EnemyComponent* enemyControlled;

void AIControlComponent1::Start() {
	Initialize();

	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < Game::aiObjectList.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)Game::Find(Game::aiObjectList.at(i).name)->GetComponent(tempNode));
	}
	currentHeading = vec3(0.0, 0.0, 0.0);

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();

	EnemyComponent* tempEnemy = &EnemyComponent();
	enemyControlled = (EnemyComponent*)Game::Find(selfName)->GetComponent(tempEnemy);
	physx::PxVec3 currentPhysLocation = enemyControlled->enPhysVehicle->getGlobalPose().p;

	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < currentAINodes.size(); i++) {

		vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - enemyControlled->enCurrentPosition);
		currentPath.x = -currentPath.x;						//QUICK FIX, UNSURE WHY THIS NEEDS TO HAPPEN
		currentPath.y = -currentPath.y;
		if (glm::length(currentPath) < shortestPath) {
			currentHeading = currentPath;
		}
	}

	Finalize();
}