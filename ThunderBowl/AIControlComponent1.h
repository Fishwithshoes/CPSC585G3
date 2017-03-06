#pragma once
#include "Component.h"
#include "AINodeComponent.h"

class AIControlComponent1 : public Component
{
public:
	void Start();
	void Update();
	void findNewPath(AINodeComponent* oldNode);
	void startPath();
	void updateHeading();

	vec3 currentHeading;
	AINodeComponent* currentNode;

private:
};