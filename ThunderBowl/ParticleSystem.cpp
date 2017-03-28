#include "ParticleSystem.h"
#include "Game.h"

ParticleSystem::ParticleSystem()
{
	ParticleSystemDesc desc = ParticleSystem::descriptionIdentity();
	InitParticleSystem(desc);
}

ParticleSystem::ParticleSystem(ParticleSystemDesc desc)
{
	InitParticleSystem(desc);
}

void ParticleSystem::InitParticleSystem(ParticleSystemDesc desc)
{
	maxParticles = desc.maxParticles;
	spawnRate = desc.spawnRate;
	coneAngle = desc.coneAngle;
	initialSpeed.min = desc.initialSpeed.min; initialSpeed.max = desc.initialSpeed.max;
	initialRadius.min = desc.initialRadius.min; initialRadius.max = desc.initialRadius.max;
	initialColor.alpha = desc.initialColor.alpha; initialColor.bravo = desc.initialColor.bravo;
	monochromatic = desc.monochromatic;
	mainTexture = desc.mainTexture;
	textures = desc.textures;
	initialFogLevel.min = desc.initialFogLevel.min; initialFogLevel.max = desc.initialFogLevel.max;
	lifeSpan.min = desc.lifeSpan.min; lifeSpan.max = desc.lifeSpan.max;
	spawnPointVariance = desc.spawnPointVariance;
	
	gravityScale = desc.gravityScale;
	accelerationScale = desc.accelerationScale;
	scaleScale = desc.scaleScale;

	burstList = desc.burstList;
	//Ensure all bursts are ready to fire
	for (int i = 0; i < burstList.size(); i++)
	{
		burstList[i].fired = false;
	}
	timeStopList = desc.timeStopList;

	useSystemLifespan = desc.useSystemLifespan;
	systemLifespan = desc.systemLifespan;
	destroySystemWhenEmpty = desc.destroySystemWhenEmpty;
}

ParticleSystemDesc ParticleSystem::descriptionIdentity()
{
	ParticleSystemDesc result;

	result.maxParticles = 100;
	result.spawnRate = 10;
	result.coneAngle = 360;
	result.initialSpeed.min = 2.0f; result.initialSpeed.max = 5.0f;
	result.initialRadius.min = 0.1f; result.initialRadius.max = 0.2f;
	result.initialColor.alpha = vec4(1.0, 1.0, 1.0, 1.0);
	result.initialColor.bravo = vec4(1.0, 1.0, 1.0, 1.0);
	result.monochromatic = false;
	result.mainTexture = MAP_DEFAULT_PART;
	result.initialFogLevel.min = 1.0f; result.initialFogLevel.max = 1.0f;
	result.lifeSpan.min = 1.0f; result.lifeSpan.max = 2.0f;
	result.spawnPointVariance = vec3(0.0, 0.0, 0.0);
	result.gravityScale = 1.0;
	result.accelerationScale = 1.0;
	result.scaleScale = 1.0;
	result.burstList = vector<ParticleBurst>();
	result.timeStopList = vector<ParticleTimeStop>();
	result.useSystemLifespan = true;
	result.systemLifespan = 10.0f;
	result.destroySystemWhenEmpty = true;

	return result;
}

//Use this for initialization
void ParticleSystem::Start()
{
	cout << "I am a ParticleSystem!" << endl;
	nextSpawn = 0.0f;
	systemCurrentTime = 0.0f;
}

//Use this for behaviour
void ParticleSystem::Update()
{
	//Particle Stream Handling
	if (spawnRate > 0)
	{
		nextSpawn -= Time::getDeltaTime();
		if (nextSpawn <= 0.0f)
		{
			numToSpawn = (int)ceil(spawnRate * Time::timeScale * Time::getDeltaTime());

			for (int i = 0; i < numToSpawn; i++)
			{
				if (particles.size() < maxParticles)
				{
					SpawnParticle();
				}
				else
					break;
			}
			nextSpawn = 1.0 / spawnRate;
		}
	}

	//Particle Burst Handling
	for (int i = 0; i < burstList.size(); i++)
	{
		ParticleBurst* burst = &burstList[i];
		if (!burst->fired && systemCurrentTime >= burst->time)
		{
			for (int j = 0; j < burst->count; j++)
			{
				if (particles.size() < maxParticles)
				{
					SpawnParticle();
				}
				else
					break;
			}
			burst->fired = true;
		}
	}

	//Particle movement, animation and death
	for (int i = 0; i < particles.size(); i++)
	{
		Particle* p = &particles[i];
	
		p->velocity -= Transform::Up() * gravityScale * Time::timeScale * Time::getDeltaTime();
		p->velocity *= accelerationScale;
		p->scale *= scaleScale;
		p->position += p->velocity * Time::timeScale * Time::getDeltaTime();
	
		//if (p->position.y < p->scale.x*0.5)
			//p->velocity = reflect(p->velocity, Transform::Up());
	
		float u = Mathf::Clamp(1.0-p->lifeRemaining/p->startingLife, 0, 1);
	
		p->material.color.w = 1 - u;
	
		//Life reduction
		p->lifeRemaining -= Time::timeScale * Time::getDeltaTime();
		if (p->lifeRemaining <= 0.0f)
			particles.erase(particles.begin() + i);
	}

	//System death
	systemCurrentTime += Time::getDeltaTime();
	if (useSystemLifespan && systemCurrentTime > systemLifespan)
		Game::DestroyParticleObjectAt(objectID);

	else if (destroySystemWhenEmpty && particles.size() <= 0 && systemCurrentTime > 1.0f)
		Game::DestroyParticleObjectAt(objectID);
}

void ParticleSystem::AddParticleBurst(int count, float time)
{
	ParticleBurst burst;
	burst.count = count;
	burst.time = time;
	burst.fired = false;
	burstList.push_back(burst);
}

int ParticleSystem::GetParticleCount()
{
	return particles.size();
}
vector<Particle> ParticleSystem::GetParticles()
{
	return particles;
}
float ParticleSystem::GetSystemCurrentTime()
{
	return systemCurrentTime;
}

void ParticleSystem::SpawnParticle()
{
	Particle p;
	vec3 spawnOffset;
	spawnOffset.x = Random::rangef(-spawnPointVariance.x, spawnPointVariance.x);
	spawnOffset.y = Random::rangef(-spawnPointVariance.y, spawnPointVariance.y);
	spawnOffset.z = Random::rangef(-spawnPointVariance.z, spawnPointVariance.z);
	p.position = transform.position + spawnOffset;
	p.scale = vec3(Random::rangef(initialRadius.min*2, initialRadius.max*2));

	float theta = Random::rangef(-coneAngle * Mathf::PI / 360, coneAngle * Mathf::PI / 360);
	float phi = Random::rangef(-Mathf::PI*0.5, Mathf::PI*0.5);

	mat3 pitch(
		1, 0, 0,
		0, cos(theta), -sin(theta),
		0, sin(theta), cos(theta));

	mat3 roll(
		cos(phi), -sin(phi), 0,
		sin(phi), cos(phi), 0,
		0, 0, 1);

	//Transform t = Transform();
	//t.Rotate(t.GetRight(), theta, false);
	//t.Rotate(t.GetForward(), phi, false);
	//vec3 dir = t.rotation * vec4(transform.GetForward(), 1.0);

	vec3 dir = pitch * transform.GetForward();
	if (coneAngle > 10)
		dir = roll * dir;

	p.velocity = dir * Random::rangef(initialSpeed.min, initialSpeed.max);
	ParticleOverlayMaterial mat;
	if (monochromatic)
	{
		float a = (initialColor.alpha.x + initialColor.alpha.y + initialColor.alpha.z) / 3;
		float b = (initialColor.bravo.x + initialColor.bravo.y + initialColor.bravo.z) / 3;
		float randVal = Random::rangef(a, b);
		mat.color.x = randVal;
		mat.color.y = randVal;
		mat.color.z = randVal;
		mat.color.w = Random::rangef(initialColor.alpha.w, initialColor.bravo.w);
	}
	else
	{
		mat.color.x = Random::rangef(initialColor.alpha.x, initialColor.bravo.x);
		mat.color.y = Random::rangef(initialColor.alpha.y, initialColor.bravo.y);
		mat.color.z = Random::rangef(initialColor.alpha.z, initialColor.bravo.z);
		mat.color.w = Random::rangef(initialColor.alpha.w, initialColor.bravo.w);
	}
	if(textures.size() <= 0)
		mat.mainTexture = mainTexture;
	else
	{
		mat.mainTexture = textures[Random::rangei(0, textures.size(), false)];
	}
	mat.fogLevel = Random::rangef(initialFogLevel.min, initialFogLevel.max);
	p.material = mat;
	p.startingLife = Random::rangef(lifeSpan.min, lifeSpan.max);
	p.lifeRemaining = Random::rangef(lifeSpan.min, lifeSpan.max);
	particles.push_back(p);
}