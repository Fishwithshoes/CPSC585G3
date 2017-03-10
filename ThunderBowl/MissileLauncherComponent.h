#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>
class MissileLauncherComponent :
	public Component
{
public:
	void Start();
	void Update();
	void FireMissile(float ownerVelocity);

	float launchDelay = 0.3;

protected:

private:
	float nextLaunch = launchDelay;
	int currentMissile = 0;
};

