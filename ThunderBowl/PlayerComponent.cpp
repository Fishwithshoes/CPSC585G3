#include "PlayerComponent.h"
#include "VehicleComponent.h"

#include "Game.h"


//VehicleComponent* playerVeh;
void PlayerComponent::Start() {
	Initialize();

	playerHealth = 100.0;
	oldHealth = playerHealth;
	playerScore = 0;
	oldScore = playerScore;

	Finalize();
}

void PlayerComponent::Update() {
	Initialize();

	if (oldHealth != playerHealth || oldScore != playerScore) {
		PlayerStateToConsole();
		oldHealth = playerHealth;
		oldScore = playerScore;
	}

	//SBurnCheck();

	//function to resolve damage to player

	//function to resolve score for:
		//damage to enemy
		//destroying enemy
		//driving without taking damage
		//driving at high speed

	Finalize();
}

void PlayerComponent::PlayerStateToConsole() {
	cout << selfName << endl;
	cout << "Player Health: " << playerHealth << endl;
	cout << "Player Score: " << playerScore << endl;
	cout << endl;
}

/*void PlayerComponent::BurnCheck() {
	VehicleComponent* vehRef = &VehicleComponent();
	playerVeh = (VehicleComponent*)Game::Find(selfName)->GetComponent(vehRef);
	if (!burning) {
		double currentVelocity = playerVeh->currentSpeed;
		cout << currentVelocity << endl;
		if (currentVelocity >= 100.00)
			burnDetectionTime -= Time::getDeltaTime();
		else
			burnDetectionTime = 2.5;
	}

	if (burnDetectionTime <= 0.0) {
		burning = true;
		cout << selfName << " is Burning" << endl;
	}

}*/