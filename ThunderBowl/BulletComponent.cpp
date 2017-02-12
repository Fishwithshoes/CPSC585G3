#include "BulletComponent.h"
#include "Game.h"

void BulletComponent::Start()
{
	Initialize();
	standardMat.diffuseColor = glm::vec3(1.0, 0.0, 0.0);

	Finalize();
}

void BulletComponent::Update()
{
	Initialize();

	transform.Translate(transform.GetForward() * speed * Time::getDeltaTime());

	lifeRemaining -= Time::getDeltaTime();

	Finalize();

	/*if (lifeRemaining <= 0.0f)
	{
		Game::DestroyWorldObjectAt(selfGameObjectID);
	}*/
}