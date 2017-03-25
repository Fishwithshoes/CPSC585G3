#include "AIControlComponent1.h"
#include "Game.h"
#include "Physics.h"
#include "GameObject.h"
#include "PlayerComponent.h"


void AIControlComponent1::Start() {
	Initialize();

	thisEnemy = Game::Find(selfName);


	//currentState = State::AI_SEEK_POWERUP;
	currentState = State::AI_SEEK_PLAYERS;


	gameNodes = Game::FindGameObjectsWithTag(TAGS_AI_NODE);

	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < gameNodes.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)gameNodes[i]->GetComponent(tempNode));
	}
	cout << "total nodes: " << currentAINodes.size() << endl;
	startPath();

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();

	if (currentState == AI_SEEK_POWERUP) {
		pathToPU();

		float currentDistance = glm::length((currentNode->nodeCurrentPosition - transform.position));

		if (currentDistance <= 10.0) {
			findNewPath(currentNode);
		}
		else {
			updateHeading();
		}
	}

	else if (currentState == AI_SEEK_PLAYERS) {
		trackPlayers();
	}

	Finalize();
}

void AIControlComponent1::findNearestOfType(NodeTypes inType)
{
	double shortestPath = 1000000.0;
	for (int i = 0; i < currentAINodes.size(); i++) {
		if (currentAINodes.at(i)->getNodeType() == inType) {
			vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);
			if (glm::length(currentPath) < shortestPath) {
				shortestPath = glm::length(currentPath);
				currentHeading = currentPath;
				currentNode = currentAINodes.at(i);
			}
		}
	}
}

void AIControlComponent1::pathToPU() {
	PlayerComponent* player = &PlayerComponent();
	player = (PlayerComponent*)Game::Find(selfName)->GetComponent(player);
	
	if (player->machineGunAmmo <= 0) {// (player->PlayerComponent::MAX_MACHINE_GUN_AMMO)*(0.75)) {
		findNearestOfType(NodeTypes::NT_MG_POWERUP);
	}
	else if (player->flamethrowerAmmo <= 0) {// (player->PlayerComponent::MAX_FLAMETHROWER_AMMO)*(0.75)) {
		findNearestOfType(NodeTypes::NT_FT_POWERUP);
	}
	else {
		findNearestOfType(NodeTypes::NT_RK_POWERUP);
	}

}

void AIControlComponent1::trackPlayers() {
	vector<GameObject*> gameHumans = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);
	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < gameHumans.size(); i++) {
		vec3 currentHumanPosition = gameHumans.at(i)->transform.position;

		currentHumanPosition.z = -currentHumanPosition.z;
		vec3 currentPath = (currentHumanPosition - transform.position);
		if (glm::length(currentPath) < shortestPath) {
			shortestPath = glm::length(currentPath);
			currentHeading = currentPath;
			//currentNode = NULL;
		}
	}
}

void AIControlComponent1::findNewPath(AINodeComponent* oldNode) {
	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < currentAINodes.size(); i++) {
		/*if (currentAINodes.at(currentAINodes.size() - 1) == oldNode) {
			currentNode = currentAINodes.at(0);
			currentHeading = (currentNode->nodeCurrentPosition - transform.position);
			break;
		}
		else if (currentAINodes.at(i) == oldNode) {
			currentNode = currentAINodes.at(i + 1);
			currentHeading = (currentNode->nodeCurrentPosition - transform.position);
			break;
		}*/

		if (currentAINodes.at(i) != oldNode) {
			vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);
			if (glm::length(currentPath) < shortestPath) {
				shortestPath = glm::length(currentPath);
				currentHeading = currentPath;
				currentNode = currentAINodes.at(i);
			}
		}
	}
	cout << "Car: " << selfName << "swapped from: " << oldNode->getName() << "to: " << currentNode->getName() << endl;
}

void AIControlComponent1::startPath() {

	double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
	for (int i = 0; i < currentAINodes.size(); i++) {
		vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);

		if (glm::length(currentPath) < shortestPath) {
			shortestPath = glm::length(currentPath);
			currentHeading = currentPath;
			currentNode = currentAINodes.at(i);
		}
	}
	cout << "start Node: " << currentNode->getName() << endl;
}

void AIControlComponent1::updateHeading() 
{
	vec3 newHeading = normalize(currentNode->nodeCurrentPosition - transform.position);
	newHeading.z = -newHeading.z;
	currentHeading = vec3(newHeading.x, newHeading.y, newHeading.z);
}