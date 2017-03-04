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

	string strScore = to_string(playerScore);

	cout << "LENGTH -> " << strScore.length() << endl;
	cout << "STRING -> " << strScore << endl;

	GameObject* score1 = Game::Find("Score1");
	GameObject* score2 = Game::Find("Score2");
	GameObject* score3 = Game::Find("Score3");
	GameObject* score4 = Game::Find("Score4");

	for (int i = strScore.length()-1; i >= 0; i--)
	{
		switch (i)
		{
		case 0:
			score4->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-1 + strScore.length()] - 48);
			break;
		case 1:
			score3->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-2 + strScore.length()] - 48);
			break;
		case 2:
			score2->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-3 + strScore.length()] - 48);
			break;
		case 3:
			score1->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-4 + strScore.length()] - 48);
			break;
		default:
			cout << "What are ya doin' matey? Score be too large!" << endl;
			break;
		}
	}

	GameObject* health = Game::Find("HealthMeter");
	health->transform.scale.x = playerHealth / 100;

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