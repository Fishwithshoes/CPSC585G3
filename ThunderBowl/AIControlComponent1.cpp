#include "AIControlComponent1.h"
#include "Game.h"
#include "Physics.h"
#include "GameObject.h"
#include "PlayerComponent.h"


void AIControlComponent1::Start() {
	Initialize();

	timer = 0.0;

	thisEnemy = Game::Find(selfName);

	gameNodes = Game::FindGameObjectsWithTag(TAGS_AI_NODE);

	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < gameNodes.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)gameNodes[i]->GetComponent(tempNode));
	}
	for (int i = 0; i < currentAINodes.size(); i++) {
		cout << "Node names: " << currentAINodes.at(i)->getName() << endl;
	}
	cout << "Nodes size: " << currentAINodes.size() << endl;

	AINodeComponent* outerNode7 = &AINodeComponent();
	outerNode7 = (AINodeComponent*)Game::Find("OuterNode6")->GetComponent(outerNode7);
	currentNode = findNearest();
	destinationNode = outerNode7;
	pathToDestination();

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();
	timer += Time::getDeltaTime();
	repathOnTimout();

	if (glm::length(currentNode->nodeCurrentPosition - transform.position) < 15.00) {
		pathToDestination();
		timer = 0.0;
	}
	else {
		updateHeading();
	}

	Finalize();
}

void AIControlComponent1::pathToDestination()
{
	AINodeComponent* nearestNode = findNearest();
	vec3 projectedHeading = glm::normalize((destinationNode->nodeCurrentPosition = transform.position));

	double tempDot = -2.0;
	AINodeComponent* tempNode = NULL;
	for (int i = 0; i < nearestNode->adjacentNodes.size(); i++) {
		//if (nearestNode->adjacentNodes.at(i) == destinationNode) {
			//tempNode = nearestNode->adjacentNodes.at(i);
		//}
		if (nearestNode->adjacentNodes.at(i) != previousNode) {
			vec3 tempPath = glm::normalize((nearestNode->adjacentNodes.at(i)->nodeCurrentPosition - transform.position));
			if (glm::dot(tempPath, projectedHeading) > tempDot) {
				tempDot = glm::dot(tempPath, projectedHeading);
				tempNode = nearestNode->adjacentNodes.at(i);
			}
		}
	}
	previousNode = currentNode;
	currentNode = tempNode;
	currentHeading = (tempNode->nodeCurrentPosition - transform.position);
}

void AIControlComponent1::repathOnTimout() {
	float timeToRepath = 3.0;
	if (timer >= timeToRepath) {
		//AINodeComponent* nearestNode = findNearest();
		for (int i = 0; i < currentNode->adjacentNodes.size(); i++) {
			if (currentNode != previousNode) {
				if (currentNode->adjacentNodes.at(i)->getName().find("Middle") != string::npos) {
					previousNode = currentNode;
					currentNode = currentNode->adjacentNodes.at(i);
					currentHeading = (currentNode->nodeCurrentPosition - transform.position);
					timer = 0.0;
					break;
				}
			}
		}
		cout << "Repath to Node: " << currentNode->getName() << endl;
	}
}

void AIControlComponent1::resetCurrent()
{
	currentNode = findNearest();
}

AINodeComponent* AIControlComponent1::findNearest()
{
	AINodeComponent* returnComponent;
	double shortestPath = 1000000.0;
	for (int i = 0; i < currentAINodes.size(); i++) {
		vec3 currentPath = (currentAINodes.at(i)->nodeCurrentPosition - transform.position);
		if (glm::length(currentPath) < shortestPath) {
			shortestPath = glm::length(currentPath);
			returnComponent = currentAINodes[i];
		}
	}
	cout << "nearest is: " << returnComponent->getName() << endl;
	return returnComponent;
}

void AIControlComponent1::updateHeading()
{
	vec3 newHeading = normalize(currentNode->nodeCurrentPosition - transform.position);
	newHeading.z = -newHeading.z;
	currentHeading = vec3(newHeading.x, newHeading.y, newHeading.z);
}

/*void AIControlComponent1::startPath() {

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
}*/

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

/*void AIControlComponent1::pathToDestination(AINodeComponent * destination)
{
	AINodeComponent* pathNode = destination;
	AINodeComponent* nearestNode = findNearest();
	cout << "nearest node: " << nearestNode->getName() << endl;
	for (int i = 0; i < nearestNode->adjacentNodes.size(); i++) {
		cout << "nodes adj: " << nearestNode->adjacentNodes.at(i)->getName() << endl;
	}
	while (find(nearestNode->adjacentNodes.begin(), nearestNode->adjacentNodes.end(), pathNode) == nearestNode->adjacentNodes.end()) {
		double shortestPath = 1000000.0;					//CHANGE TO MAP_MAX_WIDTH
		AINodeComponent* tempNode = NULL;
		for (int i = 0; i < pathNode->adjacentNodes.size(); i++) {
			vec3 tempPath = (pathNode->adjacentNodes.at(i)->nodeCurrentPosition - transform.position);
			if (glm::length(tempPath) < shortestPath) {
				shortestPath = glm::length(tempPath);
				tempNode = pathNode->adjacentNodes.at(i);
			}
		}
		pathNode = tempNode;
		//cout << pathNode->getName() << endl;
	}
	currentNode = pathNode;
	currentHeading = (currentNode->nodeCurrentPosition - transform.position);
}*/



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