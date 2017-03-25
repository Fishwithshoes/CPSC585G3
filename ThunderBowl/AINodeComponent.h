#pragma once
#include "Component.h"
class AINodeComponent :
	public Component
{
public:
	AINodeComponent();
	AINodeComponent(NodeTypes typeIn);

	void Start();
	void Update();
	NodeTypes getNodeType();

	vec3 nodeCurrentPosition;

private:
	NodeTypes thisNodeType;
};



