#pragma once
#include "Structs.h"
#include "Transform.h"

class GameObject
{
public:
	Mesh mesh;
	Transform transform;

	GameObject();
	~GameObject();

	void Start();
	void Update();

	mat4 GetModelToWorld();
	mat4 GetNormalToWorld();

protected:

private:
};
