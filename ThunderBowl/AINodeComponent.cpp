#include "AINodeComponent.h"
#include "Game.h"
#include "Physics.h"

AINodeComponent::AINodeComponent()
{
	thisNodeType = NT_TRANSIT;
}

AINodeComponent::AINodeComponent(NodeTypes typeIn)
{
	thisNodeType = typeIn;
}

void AINodeComponent::Start() {
	Initialize();
	nodeCurrentPosition = transform.position;
	Finalize();
}

void AINodeComponent::Update() {
	Initialize();

	Finalize();
}

NodeTypes AINodeComponent::getNodeType() {
	return thisNodeType;
}