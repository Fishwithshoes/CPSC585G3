#pragma once
#include "Component.h"
#include "AINodeComponent.h"
#include "PlayerComponent.h"
#include "GameObject.h"

class AIControlComponent1 : public Component
{
public:
	void Start();
	void Update();
	void updateHeading();
	void updateTracking();
	AINodeComponent* findNearest();
	void pathToDestination();
	void repathOnTimout();
	void resetCurrent();
	void detectEnemy();
	void trackEnemy();

	vec3 currentHeading;
	AINodeComponent* currentNode;
	AINodeComponent* previousNode;
	AINodeComponent* destinationNode;
	PlayerComponent* currentEnemy;
	vector<AINodeComponent*> currentAINodes;
	vector<PlayerComponent*> currentPlayers;
	vector<GameObject*> gameNodes;
	vector<GameObject*> gamePlayers;
	AINodeComponent* aiNode;
	GameObject* thisEnemy;

	float timer;

	bool arrived = false;
	bool newPath = true;
	bool reversing = false;
	bool tracking = false;


private:
	enum State {
		AI_SEEK_POWERUP,
		AI_SEEK_PLAYERS,
	};

	AIControlComponent1::State currentState;
};