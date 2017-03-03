#include "AINodeComponent.h"
#include "Game.h"
#include "Physics.h"

void AINodeComponent::Start() {
	Initialize();
	nodeCurrentPosition = transform.position;
	Finalize();
}

void AINodeComponent::Update() {
	Initialize();

	Finalize();
}