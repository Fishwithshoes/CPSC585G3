#include "AIControlComponent1.h"
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
	startPath();

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();

	float currentDistance = glm::length((currentNode->nodeCurrentPosition - enemyControlled->enCurrentPosition));
	if (currentDistance <= 25.0) {
		findNewPath(currentNode);
	}
	else {
		updateHeading();
	}

	Finalize();
}

void AIControlComponent1::findNewPath(AINodeComponent* oldNode) {
	EnemyComponent* tempEnemy = &EnemyComponent();
	enemyControlled = (EnemyComponent*)Game::Find(selfName)->GetComponent(tempEnemy);
	physx::PxVec3 currentPhysLocation = enemyControlled->enPhysVehicle->getGlobalPose().p;

	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < currentAINodes.size(); i++) {
		if (currentAINodes.at(i) != oldNode) {
			vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - enemyControlled->enCurrentPosition);
			if (glm::length(currentPath) < shortestPath) {
				shortestPath = glm::length(currentPath);
				currentHeading = currentPath;
				currentNode = currentAINodes.at(i);
			}
		}
	}
}

void AIControlComponent1::startPath() {
	EnemyComponent* tempEnemy = &EnemyComponent();
	enemyControlled = (EnemyComponent*)Game::Find(selfName)->GetComponent(tempEnemy);
	physx::PxVec3 currentPhysLocation = enemyControlled->enPhysVehicle->getGlobalPose().p;

	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < currentAINodes.size(); i++) {
		vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - enemyControlled->enCurrentPosition);
		if (glm::length(currentPath) < shortestPath) {
			shortestPath = glm::length(currentPath);
			currentHeading = currentPath;
			currentNode = currentAINodes.at(i);
		}
	}
}

void AIControlComponent1::updateHeading() {
	vec3 currentPath = (currentNode->nodeCurrentPosition - enemyControlled->enCurrentPosition);
	currentHeading = currentPath;
}