#include "CommonIncludes.h"
#include <PxPhysicsAPI.h>
#include <PxTransform.h>
#include <PxRigidBodyExt.h>


class Physics
{

public:
	//Physics();
	//~Physics();
	static void initializePhysX();
	static physx::PxRigidDynamic* createTestBox(physx::PxReal sideLength);
	static void computeRotation(physx::PxQuat angle);
	static void stepPhysics();
	static void cleanupPhysics();
private:

};
