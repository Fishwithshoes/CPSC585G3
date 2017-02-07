#include "CommonIncludes.h"
#include <PxPhysicsAPI.h>
#include <PxTransform.h>


class Physics
{
public:
	//Physics();
	//~Physics();
	void initializePhysX();
	void createTestBox(physx::PxTransform& t, physx::PxReal sideLength);
private:

};
