#pragma once
#include "Component.h"
#include "AINodeComponent.h"
#include "GameObject.h"

class AIControlComponent1 : public Component
{
public:
	void Start();
	void Update();
	void updateHeading();
	AINodeComponent* findNearest();
	void pathToDestination();
	void repathOnTimout();
	void resetCurrent();

	vec3 currentHeading;
	AINodeComponent* currentNode;
	AINodeComponent* previousNode;
	AINodeComponent* destinationNode;
	vector<AINodeComponent*> currentAINodes;
	vector<GameObject*> gameNodes;
	AINodeComponent* aiNode;
	GameObject* thisEnemy;

	float timer;

	bool arrived = false;
	bool newPath = true;


private:
	enum State {
		AI_SEEK_POWERUP,
		AI_SEEK_PLAYERS,
	};

	AIControlComponent1::State currentState;
};