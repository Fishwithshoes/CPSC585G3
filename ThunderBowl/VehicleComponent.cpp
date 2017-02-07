#include "VehicleComponent.h"
#include "Game.h"

void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component!" << endl;
	Finalize();
}

void VehicleComponent::Update()
{
	Initialize();
	
	if (Input::GetButton(ButtonCode::KEY_UP))
	{
		currentSpeed += acceleration;
		currentSpeed = Mathf::Clamp(currentSpeed, -topSpeed, topSpeed);
	}
	else if(Input::GetButton(ButtonCode::KEY_DOWN))
	{
		currentSpeed -= acceleration;
		currentSpeed = Mathf::Clamp(currentSpeed, -topSpeed, topSpeed);
	}
	else
	{
		if (currentSpeed > 0)
			currentSpeed -= acceleration;
		else
			currentSpeed += acceleration;

		if (currentSpeed > -2.2 && currentSpeed < 2.2)
			currentSpeed = 0;
	}

	if (Input::GetButton(ButtonCode::KEY_LEFT))
	{
		currentTurn -= turnAccel;
		currentTurn = Mathf::Clamp(currentTurn, -topTurn, topTurn);
	}
	else if(Input::GetButton(ButtonCode::KEY_RIGHT))
	{
		currentTurn += turnAccel;
		currentTurn = Mathf::Clamp(currentTurn, -topTurn, topTurn);
	}
	else
	{
		if (currentTurn > 0)
			currentTurn -= turnAccel;
		else
			currentTurn += turnAccel;

		if (currentTurn > -0.1 && currentTurn < 0.1)
			currentTurn = 0;
	}

	if (Input::GetButton(ButtonCode::MIDDLE_MOUSE))
		Time::timeScale += Input::GetMouseDelta().x * Time::getDeltaTime();	

	transform.Translate((float)currentSpeed * Time::getDeltaTime() * transform.GetForward());
	transform.Rotate((float)currentTurn * Time::getDeltaTime() * 0.1f*currentSpeed * transform.GetUp());
	
	Finalize();
}