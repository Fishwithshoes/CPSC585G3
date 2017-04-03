#include "StaticComponent.h"
#include "Game.h"
#include "Physics.h"
#include "Audio.h"

//For a box collider
RigidStaticComponent::RigidStaticComponent(float widthX, float heightY, float depthZ)
{
	width = widthX;
	height = heightY;
	depth = depthZ;

	shape = ST_BOX;
}
//For a sphere collider
RigidStaticComponent::RigidStaticComponent(float radiusR)
{
	radius = radiusR;

	shape = ST_SPHERE;
}
//For a capsule collider
RigidStaticComponent::RigidStaticComponent(float radiusR, float heightY)
{
	radius = radiusR;
	height = heightY;

	shape = ST_CAPSULE;
}

void RigidStaticComponent::Start() 
{
	Initialize();

	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	physx::PxVec3 startPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);

	switch (shape)
	{
	case ST_BOX:
		rigidStatic = Physics::CreateStaticBox(width, height, depth);
		break;
	case ST_SPHERE:
		rigidStatic = Physics::CreateStaticSphere(radius);
		break;
	case ST_CAPSULE:
		rigidStatic = Physics::CreateStaticCapsule(radius, height);
		break;
	default:
		cout << "CRITICAL ERROR! Invalid collider shape specified on: " << selfName << endl;
		break;
	}
	rigidStatic->userData = this;

	rigidStatic->setGlobalPose(physx::PxTransform(startPosition, physx::PxQuat(physx::PxIdentity)));
	if (shape == ST_CAPSULE)
	{
		startPosition.y -= height*0.5;
		vec4 q = normalize(vec4(0, 0, 1, 1));
		physx::PxQuat rot = physx::PxQuat(q.x, q.y, q.z, q.w);
		rigidStatic->setGlobalPose(physx::PxTransform(startPosition, rot));
	}

	if (shape != ST_CAPSULE)
	{
		transform.position.x = rigidStatic->getGlobalPose().p.x;
		transform.position.y = rigidStatic->getGlobalPose().p.y;
		transform.position.z = rigidStatic->getGlobalPose().p.z;
	}

	Finalize();
}

void RigidStaticComponent::Update()
{
	Initialize();

	//transform.position.x = rigidStatic->getGlobalPose().p.x;
	//transform.position.y = rigidStatic->getGlobalPose().p.y;
	//transform.position.z = rigidStatic->getGlobalPose().p.z;
	//
	//physx::PxQuat rotQuat = rigidStatic->getGlobalPose().q;
	//
	//transform.rotation.x = rotQuat.x;
	//transform.rotation.y = rotQuat.y;
	//transform.rotation.z = rotQuat.z;
	//transform.rotation.w = rotQuat.w;

	Finalize();
}

void RigidStaticComponent::OnCollision(Component::CollisionPair collisionPair) 
{
	cout << "Stat Collision" << endl;

	switch (collisionPair)
	{
	case CollisionPair::CP_VEHICLE_STATIC:
		Audio::Play2DSound(SFX_Hit, Random::rangef(0.3, 0.4), 0);
		break;
	case CollisionPair::CP_STATIC_PROJECTILE:
		Audio::Play2DSound(SFX_Hit, Random::rangef(0.2, 0.3), 0);
		break;
	default:
		break;
	}
}
