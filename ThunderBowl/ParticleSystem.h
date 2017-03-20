#pragma once
#include "GameObject.h"
class ParticleSystem : public GameObject
{
public:
	ParticleSystem();
	ParticleSystem(ParticleSystemDesc desc);
	void InitParticleSystem(ParticleSystemDesc desc);
	ParticleSystemDesc descriptionIdentity();

	void Start();
	void Update();

	void AddParticleBurst(int count, float time);

	int GetParticleCount();
	vector<Particle> GetParticles();
	float GetSystemCurrentTime();

	int maxParticles;
	float spawnRate;
	float coneAngle;//In degrees
	RandRangeFloat initialSpeed;
	RandRangeFloat initialRadius;
	RandRangeColor initialColor;
	bool monochromatic;
	GLuint mainTexture;
	vector<GLuint> textures;
	RandRangeFloat initialFogLevel;
	RandRangeFloat lifeSpan;
	vec3 spawnPointVariance;
	float gravityScale;
	float accelerationScale;

	bool useSystemLifespan;
	float systemLifespan;
	bool destroySystemWhenEmpty;

protected:
	
private:
	void SpawnParticle();

	float systemCurrentTime;
	vector<Particle> particles;
	float nextSpawn;
	int numToSpawn;
	vector<ParticleBurst> burstList;
	vector<ParticleTimeStop> timeStopList;
};

