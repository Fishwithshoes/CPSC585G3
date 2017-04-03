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

	transform.Rotate(speed * Time::getDeltaTime() * Transform::Up(), true);

	Finalize();
}