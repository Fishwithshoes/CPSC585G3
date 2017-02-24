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

	transform.Rotate(speed * 0.1f * Time::getDeltaTime() * Transform::Right());

	Finalize();
}