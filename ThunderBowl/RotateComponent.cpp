#include "RotateComponent.h"
#include "Game.h"

void RotateComponent::Start()
{
	Initialize();
	cout << "I am a RotateComponent!" << endl;
	Finalize();
}

void RotateComponent::Update()
{
	Initialize();

	transform.Rotate(vec3(speed) * Time::getDeltaTime() * transform.Up());

	Finalize();
}