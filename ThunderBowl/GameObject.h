#pragma once
#include "Structs.h"
#include "Transform.h"

class GameObject
{
public:
	Mesh mesh;
	Transform transform;
	StandardMaterial standardMat;
	ParticleOverlayMaterial particleOverlayMat;

	GameObject();
	~GameObject();

	void Start();
	void Update();

	mat4 GetModelToWorld();
	mat4 GetNormalToWorld();

	//DO NOT modify, unless Creating or Destroying objects.
	//This Should ONLY be done in Game.CreateXXX() and Game.DestroyXXX()
	int objectID = -1;

protected:

private:
};
