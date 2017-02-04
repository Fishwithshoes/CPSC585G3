#include "Physics.h"

using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

//mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
Physics::Physics()
{
}


Physics::~Physics()
{
}

void Physics::initializePhysX() {


}
