#include "AIControlComponent1.h"
#include "Game.h"
#include "Physics.h"
#include "GameObject.h"
#include "PlayerComponent.h"


void AIControlComponent1::Start() {
	Initialize();

	thisEnemy = Game::Find(selfName);

	//cout << "CHECKER AIControlComponent Ref1" << endl;

	currentState = State::AI_SEEK_POWERUP;
	//currentState = State::AI_SEEK_PLAYERS;


	gameNodes = Game::FindGameObjectsWithTag(TAGS_AI_NODE);

	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < gameNodes.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)gameNodes[i]->GetComponent(tempNode));
		cout << "got the node here" << endl;
	}
	cout << "total nodes: " << currentAINodes.size() << endl;
	startPath();
	//AINodeComponent* outerNode7 = &AINodeComponent();
	//outerNode7 = (AINodeComponent*)Game::Find("OuterNode7")->GetComponent(outerNode7);
	//pathToDestination(outerNode7);

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();

	//AINodeComponent* outerNode7 = &AINodeComponent();
	//outerNode7 = (AINodeComponent*)Game::Find("OuterNode7")->GetComponent(outerNode7);
	//pathToDestination(outerNode7);

		float currentDistance = glm::length((currentNode->nodeCurrentPosition - transform.position));
		if (currentDistance <= 10.0) {
			//AINodeComponent* innerNode3 = &AINodeComponent();
			//innerNode3 = (AINodeComponent*)Game::Find("InnerNode3")->GetComponent(innerNode3);
			//pathToDestination(innerNode3);
			//current Node arrived at, repath
		}
		else {
			updateHeading();
		}

	Finalize();
}

/*void AIControlComponent1::findNearestOfType(NodeTypes inType)
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
}*/

AINodeComponent* AIControlComponent1::findNearest()
{
	AINodeComponent* returnComponent;
	double shortestPath = 1000000.0;
	for (int i = 0; i < currentAINodes.size(); i++) {
		vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);
		if (glm::length(currentPath) < shortestPath) {
			shortestPath = glm::length(currentPath);
			returnComponent = currentAINodes.at(i);
		}
	}
	return returnComponent;
}

/*void AIControlComponent1::pathToPU() {
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

}*/

/*void AIControlComponent1::trackPlayers() {
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
}*/

void AIControlComponent1::pathToDestination(AINodeComponent * destination)
{
	AINodeComponent* pathNode = destination;
	AINodeComponent* nearestNode = findNearest();
	for (int i = 0; i < nearestNode->adjacentNodes.size(); i++) {
		cout << "nodes adj: " << nearestNode->adjacentNodes.at(i)->getName() << endl;
	}

	std::vector<AINodeComponent*> visited;
//	cout << "nearest: " << nearestNode->getName() << endl;
	while (find(nearestNode->adjacentNodes.begin(), nearestNode->adjacentNodes.end(), pathNode) == nearestNode->adjacentNodes.end()) {
		double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
		AINodeComponent* tempNode = NULL; //= &AINodeComponent();
		for (int i = 0; i < pathNode->adjacentNodes.size(); i++) {
			//std::cout << "i = " << i << std::endl;
			vec3 tempPath = (pathNode->adjacentNodes.at(i)->nodeCurrentPosition - transform.position);
			if (glm::length(tempPath) < shortestPath) { // && pathNode->adjacentNodes.at(i) != tempNode) {
				shortestPath = glm::length(tempPath);
				tempNode = pathNode->adjacentNodes.at(i);
				//std::cout << "min len found: " << glm::length(tempPath) <<  std::endl;
			}
		}
		if (pathNode == tempNode) {

			std::cout << "What" << std::endl;
		}
		visited.push_back(tempNode);
		for (auto &a : visited) {
			// if (a == pathNode)
			//	std::cout << "been here before" << std::endl;

		}
		pathNode = tempNode;
		cout << pathNode->getName() << endl;
		for (AINodeComponent* a : nearestNode->adjacentNodes) {
			//if (a == pathNode)
				//std::cout << "ptr match found" << std::endl;
		}
	}
	currentNode = pathNode;
	currentHeading = (currentNode->nodeCurrentPosition - transform.position);
}

/*void AIControlComponent1::findNewPath(AINodeComponent* oldNode) {
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
		}

		if (find(oldNode->adjacentNodes.begin(), oldNode->adjacentNodes.end(), currentAINodes.at(i)) != oldNode->adjacentNodes.end()) {
			vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);
			if (glm::length(currentPath) < shortestPath) {
				shortestPath = glm::length(currentPath);
				currentHeading = currentPath;
				currentNode = currentAINodes.at(i);
			}
		}
	}
	cout << "Car: " << selfName << "swapped from: " << oldNode->getName() << "to: " << currentNode->getName() << endl;
}*/

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
	//cout << "start Node: " << currentNode->getName() << endl;
}

void AIControlComponent1::updateHeading() 
{
	vec3 newHeading = normalize(currentNode->nodeCurrentPosition - transform.position);
	newHeading.z = -newHeading.z;
	currentHeading = vec3(newHeading.x, newHeading.y, newHeading.z);
}