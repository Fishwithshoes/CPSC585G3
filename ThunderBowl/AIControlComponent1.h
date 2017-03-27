#pragma once
#include "Component.h"
#include "AINodeComponent.h"
#include "GameObject.h"

class AIControlComponent1 : public Component
{
public:
	void Start();
	void Update();
	void findNewPath(AINodeComponent* oldNode);
	void startPath();
	void updateHeading();

	void findNearestOfType(NodeTypes inType);
	AINodeComponent* findNearest();
	void pathToPU();
	void trackPlayers();

	void pathToDestination(AINodeComponent* destination);

	vec3 currentHeading;
	AINodeComponent* currentNode;
	AINodeComponent* destinationNode;
	vector<AINodeComponent*> currentAINodes;
	vector<GameObject*> gameNodes;
	AINodeComponent* aiNode;
	GameObject* thisEnemy;

	bool newPath = true;


private:
	enum State {
		AI_SEEK_POWERUP,
		AI_SEEK_PLAYERS,
	};

	AIControlComponent1::State currentState;
};