#include "Physics.h"

using namespace physx;

PxDefaultErrorCallback gDefaultErrorCallback;
PxDefaultAllocator gDefaultAllocator;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxMaterial* gMaterial = NULL;
//PxTransform* gBaseTrans = NULL;
//PxVisualDebuggerConnection* gVDebugConnection = NULL;

/*Physics::Physics()
{
}


Physics::~Physics()
{
}*/

void Physics::initializePhysX() {
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
	PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, profileZoneManager);

	/*if (gPhysics->getPvdConnectionManager())
	{
	gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
	gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
	gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
	gVDebugConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), PVD_HOST, 5425, 10);
	}*/

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);
}

PxRigidDynamic* Physics::createTestBox(PxReal sideLength)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(sideLength, sideLength, sideLength), *gMaterial);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0, 0, 0)));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	gScene->addActor(*body);
	return body;
}

void Physics::stepPhysics()
{
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void Physics::cleanupPhysics()
{
	gScene->release();
	gDispatcher->release();
	PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
	gPhysics->release();
	profileZoneManager->release();
	gFoundation->release();
}
