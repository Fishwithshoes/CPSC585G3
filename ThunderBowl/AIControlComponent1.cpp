#include "AIControlComponent1.h"
#include "AINodeComponent.h"
#include "Game.h"

vector<AINodeComponent*> currentAINodes;
AINodeComponent* aiNode;

void AIControlComponent1::Start() {
	AINodeComponent* tempNode = &AINodeComponent();
	for (int i = 0; i < Game::aiObjectList.size(); i++) {
		currentAINodes.push_back((AINodeComponent*)Game::Find(Game::aiObjectList.at(i).name)->GetComponent(tempNode));
	}
}

void AIControlComponent1::Update() {

}