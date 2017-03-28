#include "FlamethrowerComponent.h"
#include "Game.h"
//#include "GameManager.h"
#include "GeoGenerator.h"
#include "ParticleSystem.h"
#include "Audio.h"

void FlamethrowerComponent::Start()
{
	Initialize();

	cout << "I am a Flamethrower Component!" << endl;
	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "FireStream";
	ps.coneAngle = 0;
	ps.gravityScale = -29.0;
	ps.spawnPointVariance = vec3(2.1, 0.5, 2.1);
	ps.initialColor.alpha = vec4(1.0, 0.9, 0.9, 1)*1.4f;
	ps.initialColor.bravo = vec4(0.6, 0.6, 0.9, 1)*1.4f;
	ps.monochromatic = false;
	ps.spawnRate = 0.0;
	ps.lifeSpan.min = 0.10;
	ps.lifeSpan.max = 0.15;
	ps.mainTexture = MAP_DEFAULT_PART;
	for (int i = 0; i < 16; i++)
		ps.textures.push_back(MAP_FLAME01_PART + i);
	ps.destroySystemWhenEmpty = false;
	ps.useSystemLifespan = false;
	Game::CreateParticleObject(ps);
	fireStreamName = ps.name;

	t = transform;
	t.rotation = t.GetInverseRotation();

	UpdateParticles();

	Finalize();
}

void FlamethrowerComponent::Update()
{
	Initialize();
	t = transform;
	t.rotation = t.GetInverseRotation();

	UpdateParticles();

	Finalize();
}

void FlamethrowerComponent::UpdateParticles()
{
	ParticleSystem* fireStream = (ParticleSystem*)Game::Find(fireStreamName);
	GameObject* self = Game::Find(selfName);
	PlayerComponent* ownerPlayer = &PlayerComponent();
	ownerPlayer = (PlayerComponent*)self->GetComponent(ownerPlayer);

	fireStream->transform = t;
	fireStream->transform.Translate(t.GetForward() * 3.0f, false);
	fireStream->transform.Translate(t.GetUp() * 0.5f, false);
	fireStream->transform.Rotate(Transform::Up(), 0.1, false);

	float streamPower = 0.0;
	float inheritedVelocity = 0.0;
	float vertical = 0.0;
	float horizontal = 0.0;

	//t.rotation = t.GetInverseRotation();
	if (ownerPlayer->currentWeapon == GW_FLAMETHROWER && ownerPlayer->flamethrowerAmmo > 0)
	{
		if (self->tag == TAGS_HUMAN_PLAYER)
		{
			VehicleComponent* vehicle = &VehicleComponent();
			vehicle = (VehicleComponent*)self->GetComponent(vehicle);
			int controllerNum = vehicle->GetPlayerNum();
			vertical = Input::GetXBoxAxis(controllerNum, ButtonCode::XBOX_JOY_RIGHT_VERTICAL);
			horizontal = Input::GetXBoxAxis(controllerNum, ButtonCode::XBOX_JOY_RIGHT_HORIZONTAL);
		}
		else
		{
			vertical = 1.0;
			horizontal = 0.0;
		}
	}

	streamPower = vertical + abs(horizontal);
	streamPower = Mathf::Clamp(streamPower, 0.0, 1.0);

	float theta = -Mathf::PI * 0.3 * horizontal;
	fireStream->transform.Rotate(t.GetUp(), theta, false);

	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		inheritedVelocity = vehicle->physVehicle->getLinearVelocity().magnitude();
	}
	else
	{
		EnemyComponent* enemy = &EnemyComponent();
		enemy = (EnemyComponent*)self->GetComponent(enemy);
		inheritedVelocity = enemy->enPhysVehicle->getLinearVelocity().magnitude();
	}

	fireStream->initialSpeed.min = streamPower * 60 + inheritedVelocity;
	fireStream->initialSpeed.max = streamPower * 68 + inheritedVelocity;
	fireStream->spawnRate = streamPower * 60;
	fireStream->initialRadius.min = streamPower * 0.7;
	fireStream->initialRadius.max = streamPower * 1.2;
	fireStream->scaleScale = 1.0 + streamPower*0.3;

	vector<GameObject*> players = Game::FindGameObjectsWithTag(TAGS_AI_PLAYER);
	vector<GameObject*> playeri = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);

	players.insert(players.end(), playeri.begin(), playeri.end());

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->name != selfName)
		{
			HealthComponent* victimHealth = &HealthComponent();
			victimHealth = (HealthComponent*)players[i]->GetComponent(victimHealth);
			vec3 vPos = players[i]->transform.position;
			float damage = 0.0f;
			for (int j = 0; j < fireStream->GetParticleCount(); j++)
			{
				vec3 pPos = fireStream->GetParticles()[j].position;

				damage += Mathf::Clamp(10.0f - distance(vPos, pPos), 0, 10) *
					streamPower * Time::timeScale * Time::getDeltaTime();
			}
				
			victimHealth->currentHealth -= damage;

			if (victimHealth->currentHealth <= 0.0)//Points for making a kill
			{
				ownerPlayer->playerScore += 100;
			}
			else//Points for damage
			{
				ownerPlayer->playerScore += damage*0.5;
			}
		}
	}

	ownerPlayer->flamethrowerAmmo -= streamPower*Time::getDeltaTime();
	if (ownerPlayer->flamethrowerAmmo < 0)
		ownerPlayer->flamethrowerAmmo = 0;
}