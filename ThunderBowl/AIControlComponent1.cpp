#include "AIControlComponent1.h"
#include "Game.h"
#include "Physics.h"
#include "GameObject.h"
#include "PlayerComponent.h"


void AIControlComponent1::Start() {
	Initialize();

	timer = 0.0;
	thisEnemy = Game::Find(selfName);

	PlayerComponent* tempAiPlayer = &PlayerComponent();
	thisAIPlayer = (PlayerComponent*)Game::Find(selfName)->GetComponent(tempAiPlayer);

	gameNodes = Game::FindGameObjectsWithTag(TAGS_AI_NODE);
	gamePlayers = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);
	//ADD in AI PLAYERS

	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < gameNodes.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)gameNodes[i]->GetComponent(tempNode));
	}

	PlayerComponent* tempPlayer = &PlayerComponent();
	for (int i = 0; i < gamePlayers.size(); i++) {
		currentPlayers.push_back((PlayerComponent*)gamePlayers[i]->GetComponent(tempPlayer));
	}

	for (int i = 0; i < currentPlayers.size(); i++) {
		cout << "Player names: " << currentPlayers.at(i)->getName() << endl;
	}
	cout << "Nodes size: " << currentPlayers.size() << endl;

	AINodeComponent* outerNode7 = &AINodeComponent();
	outerNode7 = (AINodeComponent*)Game::Find("OuterNode6")->GetComponent(outerNode7);
	currentNode = findNearest();
	destinationNode = outerNode7;
	pathToDestination();

	Finalize();
}

void AIControlComponent1::Update() {
	Initialize();

	detectEnemy();
	timer += Time::getDeltaTime();

	if (tracking) {
		trackEnemy();
		chooseWeapon();
	}

	else if (reversing) {
		EnemyComponent* thisEnemyComp = &EnemyComponent();
		thisEnemyComp = (EnemyComponent*)thisEnemy->GetComponent(thisEnemyComp);
		thisEnemyComp->ReverseOut();
		if (timer >= 3.0) {
			timer = 0.0;
			reversing = false;
		}
	}
	else {
		repathOnTimout();
		if (glm::length(currentNode->nodeCurrentPosition - transform.position) < 15.00) {
			pathToDestination();
			timer = 0.0;
		}
		else {
			updateHeading();
		}
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
	EnemyComponent* thisEnemyComp = &EnemyComponent();
	thisEnemyComp = (EnemyComponent*)thisEnemy->GetComponent(thisEnemyComp);
	HealthComponent* thisHealthComp = &HealthComponent();
	thisHealthComp = (HealthComponent*)thisEnemy->GetComponent(thisHealthComp);

	float timeToRepath = 10.0;
	if (timer >= timeToRepath) {
		if ((thisHealthComp->currentHealth > 0.0) && (thisEnemyComp->enPhysVehicle->getLinearVelocity().magnitude() < 75.0)) {
			for (int i = 0; i < currentNode->adjacentNodes.size(); i++) {
				if (currentNode != previousNode) {
					if (currentNode->adjacentNodes.at(i)->getName().find("Middle") != string::npos) {
						previousNode = currentNode;
						currentNode = currentNode->adjacentNodes.at(i);
					}
				}
			}
			thisEnemyComp->enVehicleNoDrive->setSteerAngle(2, -thisEnemyComp->enVehicleNoDrive->getSteerAngle(2));
			thisEnemyComp->enVehicleNoDrive->setSteerAngle(3, -thisEnemyComp->enVehicleNoDrive->getSteerAngle(3));
			reversing = true;
			timer = 0.0;
		}
	}
}


/*for (int i = 0; i < currentNode->adjacentNodes.size(); i++) {
if (currentNode != previousNode) {
if (currentNode->adjacentNodes.at(i)->getName().find("Middle") != string::npos) {
previousNode = currentNode;
currentNode = currentNode->adjacentNodes.at(i);
currentHeading = (currentNode->nodeCurrentPosition - transform.position);
timer = 0.0;
cout << selfName << " Repath to Node: " << currentNode->getName() << endl;

break;
}
}
}*/
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

void AIControlComponent1::updateTracking()
{
	if (currentEnemy != NULL) {
		vec3 newHeading = normalize(currentEnemy->playerCurrentPosition - transform.position);
		newHeading.z = -newHeading.z;
		currentHeading = vec3(newHeading.x, newHeading.y, newHeading.z);
	}
}

void AIControlComponent1::detectEnemy() {
	bool enemyFound = false;
	for (int i = 0; i < currentPlayers.size(); i++) {
		if (glm::distance(transform.position, currentPlayers.at(i)->playerCurrentPosition) < 50.0) {
			currentEnemy = currentPlayers.at(i);
			enemyFound = true;
			tracking = true;
		}
	}
	if (!enemyFound)
		currentEnemy = NULL;
}

void AIControlComponent1::trackEnemy() {
	if (currentEnemy != NULL) {
		if (glm::distance(transform.position, currentEnemy->playerCurrentPosition) < 100.0) {
			updateTracking();
		}
	}
	else {
		currentEnemy = NULL;
		tracking = false;
	}
}

void AIControlComponent1::chooseWeapon()
{
	//MissileLauncherComponent* thisMissileLauncher = &MissileLauncherComponent();
	//thisMissileLauncher = (MissileLauncherComponent*)Game::Find(selfName)->GetComponent(thisMissileLauncher);
	//FlamethrowerComponent* thisFlameThrower = &FlamethrowerComponent();
	//thisFlameThrower = (FlamethrowerComponent*)Game::Find(selfName)->GetComponent(thisFlameThrower);

	if (currentEnemy != NULL) 
	{
		if((thisAIPlayer->machineGunAmmo != 0) && glm::dot(getFiringVector(), transform.GetForward()) > 0.5) 
		{
			MachineGunComponent* thisMG = &MachineGunComponent();
			thisMG = (MachineGunComponent*)Game::Find(selfName)->GetComponent(thisMG);
			thisMG->FireMG();
		}
	}

}

vec3 AIControlComponent1::getFiringVector()
{
	return glm::normalize(currentEnemy->playerCurrentPosition - transform.position);
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