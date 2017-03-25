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
	void pathToPU();
	void trackPlayers();

	vec3 currentHeading;
	AINodeComponent* currentNode;
	vector<AINodeComponent*> currentAINodes;
	vector<GameObject*> gameNodes;
	AINodeComponent* aiNode;
	GameObject* thisEnemy;


private:
	enum State {
		AI_SEEK_POWERUP,
		AI_SEEK_PLAYERS,
	};

	AIControlComponent1::State currentState;
};