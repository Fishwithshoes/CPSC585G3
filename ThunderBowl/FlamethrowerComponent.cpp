#include "FlamethrowerComponent.h"
#include "Game.h"
#include "ParticleSystem.h"
#include "Audio.h"

void FlamethrowerComponent::Start()
{
	Initialize();

	cout << "I am a Flamethrower Component!" << endl;

	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "FireStream";
	ps.coneAngle = 0;
	ps.gravityScale = -9.0;
	ps.spawnPointVariance = vec3(2.1, 0.5, 2.1);
	ps.initialColor.alpha = vec4(0.9, 0.2, 0.1, 1)*1.4f;
	ps.initialColor.bravo = vec4(0.9, 0.5, 0.1, 1)*1.4f;
	ps.monochromatic = false;
	ps.lifeSpan.min = 0.6;
	ps.lifeSpan.max = 0.8;
	ps.mainTexture = MAP_DEFAULT_PART;
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

	//t.rotation = t.GetInverseRotation();

	fireStream->transform = t;
	fireStream->transform.Translate(t.GetForward() * 3.0f, false);
	fireStream->transform.Translate(t.GetUp() * 0.5f, false);

	fireStream->transform.Rotate(Transform::Up(), 0.1, true);

	float streamPower = 0.0;
	float inheritedVelocity = 0.0;
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		float vertical = Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_RIGHT_VERTICAL);
		float horizontal = Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_RIGHT_HORIZONTAL);
		streamPower = vertical + abs(horizontal);
		streamPower = Mathf::Clamp(streamPower, 0.0, 1.0);

		float theta = -Mathf::PI * 0.3 * horizontal;
		fireStream->transform.Rotate(t.GetUp(), theta, false);

		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		inheritedVelocity = vehicle->physVehicle->getLinearVelocity().magnitude();
	}
	else
	{

	}

	fireStream->initialSpeed.min = streamPower * 16 + inheritedVelocity;
	fireStream->initialSpeed.max = streamPower * 18 + inheritedVelocity;
	fireStream->spawnRate = streamPower * 10;
	fireStream->initialRadius.min = streamPower * 1.4;
	fireStream->initialRadius.max = streamPower * 1.8;
}