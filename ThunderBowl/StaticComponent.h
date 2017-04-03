#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class RigidStaticComponent :
	public Component
{
public:
	RigidStaticComponent(float widthX, float heightY, float depthZ);//For a box collider
	RigidStaticComponent(float radiusR);//For a sphere collider
	RigidStaticComponent(float radiusR, float heightY);//For a capsule collider

	physx::PxRigidStatic* rigidStatic;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);
private:
	float width;
	float height;
	float depth;
	float radius;

	enum ShapeType
	{
		ST_BOX,
		ST_SPHERE,
		ST_CAPSULE
	};
	ShapeType shape;
};

