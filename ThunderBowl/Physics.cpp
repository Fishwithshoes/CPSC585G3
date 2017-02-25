#include "Physics.h"
#include "Component.h"

using namespace physx;

PxDefaultErrorCallback gDefaultErrorCallback;
PxDefaultAllocator gDefaultAllocator;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxMaterial* gMaterial = NULL;
PxCooking*				gCooking = NULL;

VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
PxBatchQuery*			gBatchQuery = NULL;

PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

PxRigidStatic*			gGroundPlane = NULL;
PxVehicleNoDrive*		gVehicleNoDrive = NULL;
PxVehicleNoDrive*		enVehicleNoDrive = NULL;



//PxTransform* gBaseTrans = NULL;
//PxVisualDebuggerConnection* gVDebugConnection = NULL;

/*Physics::Physics()
{
}


Physics::~Physics()
{
}*/

class ContactReportCallback : public PxSimulationEventCallback
{
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		PxU32 shapeBuffer1Size = pairHeader.actors[0]->getNbShapes() * sizeof(PxShape*);	//get first actor's shape
		PxU32 shapeBuffer2Size = pairHeader.actors[1]->getNbShapes() * sizeof(PxShape*);	//get second actor's shape
		PxShape** shapeBuffer1 = new PxShape*[shapeBuffer1Size];
		PxShape** shapeBuffer2 = new PxShape*[shapeBuffer2Size];
		pairHeader.actors[0]->getShapes(shapeBuffer1, shapeBuffer1Size);
		pairHeader.actors[1]->getShapes(shapeBuffer2, shapeBuffer2Size);
		Component* firstCollider = reinterpret_cast<Component*>(pairHeader.actors[0]->userData);	//get Component rep for each collider
		Component* secondCollider = reinterpret_cast<Component*>(pairHeader.actors[1]->userData);

		if ((shapeBuffer1[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_CHASSIS || 
			shapeBuffer1[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_WHEEL) &&
			shapeBuffer2[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_OBSTACLE) {
			firstCollider->OnCollision(Component::CollisionPair::CP_VEHICLE_POWERUP);
			secondCollider->OnCollision(Component::CollisionPair::CP_VEHICLE_POWERUP);
		}
		else if (shapeBuffer1[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_PROJECTILE &&
			shapeBuffer2[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_OBSTACLE) {
			firstCollider->OnCollision(Component::CollisionPair::CP_VEHICLE_POWERUP);
			secondCollider->OnCollision(Component::CollisionPair::CP_VEHICLE_POWERUP);
		}
		/*
		if (shapeBuffer[0]->getSimulationFilterData().word0 == Physics::CollisionTypes::COLLISION_FLAG_OBSTACLE) {
			Component* owner = reinterpret_cast<Component*>(pairHeader.actors[0]->userData);
			owner->OnCollision();
		}*/
		/*shape->getSimulationFilterData();
		
		PX_UNUSED((pairHeader));
		std::vector<PxContactPairPoint> contactPoints;

		for (PxU32 i = 0; i<nbPairs; i++)
		{
			PxU32 contactCount = pairs[i].contactCount;
			if (contactCount)
			{
				contactPoints.resize(contactCount);
				pairs[i].extractContacts(&contactPoints[0], contactCount);

				for (PxU32 j = 0; j<contactCount; j++)
				{
					gContactPositions.push_back(contactPoints[j].position);
					gContactImpulses.push_back(contactPoints[j].impulse);
				}
			}
		}*/
		//delete[] shapeBuffer;
	}
};

ContactReportCallback gContactReportCallback;

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
	sceneDesc.filterShader = VehicleFilterShader;//PxFilterFlag::eSUPPRESS;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.3f);

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	//PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene->addActor(*groundPlane);

	PxInitVehicleSDK(*gPhysics);
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, gDefaultAllocator);
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = Physics::createFrictionPairs(gMaterial);

	//Create a plane to drive on.
	gGroundPlane = createDrivablePlane(gMaterial, gPhysics);
	gScene->addActor(*gGroundPlane);
}

void Physics::computeRotation(PxQuat angle) {}

void Physics::stepPhysics()
{
	const PxF32 timestep = 1.0f / 60.0f;

	//Raycasts.
	PxVehicleWheels* vehicles[1] = { gVehicleNoDrive };
	PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
	const PxU32 raycastResultsSize = gVehicleSceneQueryData->getRaycastQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(gBatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

	//Vehicle update.
	const PxVec3 grav = gScene->getGravity();
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	PxVehicleWheelQueryResult vehicleQueryResults[1] = { { wheelQueryResults, gVehicleNoDrive->mWheelsSimData.getNbWheels() } };
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	//Scene update.
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void Physics::cleanupPhysics()
{

	gVehicleNoDrive->getRigidDynamicActor()->release();
	gVehicleNoDrive->free();
	gGroundPlane->release();
	gBatchQuery->release();
	gVehicleSceneQueryData->free(gDefaultAllocator);
	gFrictionPairs->release();
	PxCloseVehicleSDK();

	gMaterial->release();
	gCooking->release();
	gScene->release();
	gDispatcher->release();
	PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
	gPhysics->release();
	profileZoneManager->release();
	gFoundation->release();

	printf("ThunderBowl done.\n");
}

//getters
physx::PxPhysics* Physics::getGPhysics()
{
	return gPhysics;
}

physx::PxCooking* Physics::getGCooking()
{
	return gCooking;
}

physx::PxScene* Physics::getGScene()
{
	return gScene;
}

//setters
void Physics::setGVehicleNoDrive(physx::PxVehicleNoDrive* in)
{
	gVehicleNoDrive = in;
}

void Physics::setEnVehicleNoDrive(physx::PxVehicleNoDrive* in)
{
	enVehicleNoDrive = in;
}


//PxVehicleFunctions
void Physics::computeWheelCenterActorOffsets
(const PxF32 wheelFrontZ, const PxF32 wheelRearZ, const PxVec3& chassisDims, const PxF32 wheelWidth, const PxF32 wheelRadius, const PxU32 numWheels, PxVec3* wheelCentreOffsets)
{
	//chassisDims.z is the distance from the rear of the chassis to the front of the chassis.
	//The front has z = 0.5*chassisDims.z and the rear has z = -0.5*chassisDims.z.
	//Compute a position for the front wheel and the rear wheel along the z-axis.
	//Compute the separation between each wheel along the z-axis.
	const PxF32 numLeftWheels = numWheels / 2.0f;
	const PxF32 deltaZ = (wheelFrontZ - wheelRearZ) / (numLeftWheels - 1.0f);
	//Set the outside of the left and right wheels to be flush with the chassis.
	//Set the top of the wheel to be just touching the underside of the chassis.
	for (PxU32 i = 0; i < numWheels; i += 2)
	{
		//Left wheel offset from origin.
		wheelCentreOffsets[i + 0] = PxVec3((-chassisDims.x + wheelWidth)*0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i*deltaZ*0.5f);
		//Right wheel offsets from origin.
		wheelCentreOffsets[i + 1] = PxVec3((+chassisDims.x - wheelWidth)*0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i*deltaZ*0.5f);
	}
}

void Physics::setupWheelsSimulationData
(const PxF32 wheelMass, const PxF32 wheelMOI, const PxF32 wheelRadius, const PxF32 wheelWidth,
	const PxU32 numWheels, const PxVec3* wheelCenterActorOffsets,
	const PxVec3& chassisCMOffset, const PxF32 chassisMass,
	PxVehicleWheelsSimData* wheelsSimData)
{
	//Set up the wheels.
	PxVehicleWheelData wheels[PX_MAX_NB_WHEELS];
	{
		//Set up the wheel data structures with mass, moi, radius, width.
		for (PxU32 i = 0; i < numWheels; i++)
		{
			wheels[i].mMass = wheelMass;
			wheels[i].mMOI = wheelMOI;
			wheels[i].mRadius = wheelRadius;
			wheels[i].mWidth = wheelWidth;
		}
	}

	//Set up the tires.
	PxVehicleTireData tires[PX_MAX_NB_WHEELS];
	{
		//Set up the tires.
		for (PxU32 i = 0; i < numWheels; i++)
		{
			tires[i].mType = Physics::TireTypes::TIRE_TYPE_NORMAL;
		}
	}

	//Set up the suspensions
	PxVehicleSuspensionData suspensions[PX_MAX_NB_WHEELS];
	{
		//Compute the mass supported by each suspension spring.
		PxF32 suspSprungMasses[PX_MAX_NB_WHEELS];
		PxVehicleComputeSprungMasses(numWheels, wheelCenterActorOffsets, chassisCMOffset, chassisMass, 1, suspSprungMasses);

		//Set the suspension data.
		for (PxU32 i = 0; i < numWheels; i++)
		{
			suspensions[i].mMaxCompression = 0.3f;
			suspensions[i].mMaxDroop = 0.1f;
			suspensions[i].mSpringStrength = 35000.0f;
			suspensions[i].mSpringDamperRate = 4500.0f;
			suspensions[i].mSprungMass = suspSprungMasses[i];
		}

		//Set the camber angles.
		const PxF32 camberAngleAtRest = 0.0;
		const PxF32 camberAngleAtMaxDroop = 0.01f;
		const PxF32 camberAngleAtMaxCompression = -0.01f;
		for (PxU32 i = 0; i < numWheels; i += 2)
		{
			suspensions[i + 0].mCamberAtRest = camberAngleAtRest;
			suspensions[i + 1].mCamberAtRest = -camberAngleAtRest;
			suspensions[i + 0].mCamberAtMaxDroop = camberAngleAtMaxDroop;
			suspensions[i + 1].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
			suspensions[i + 0].mCamberAtMaxCompression = camberAngleAtMaxCompression;
			suspensions[i + 1].mCamberAtMaxCompression = -camberAngleAtMaxCompression;
		}
	}

	//Set up the wheel geometry.
	PxVec3 suspTravelDirections[PX_MAX_NB_WHEELS];
	PxVec3 wheelCentreCMOffsets[PX_MAX_NB_WHEELS];
	PxVec3 suspForceAppCMOffsets[PX_MAX_NB_WHEELS];
	PxVec3 tireForceAppCMOffsets[PX_MAX_NB_WHEELS];
	{
		//Set the geometry data.
		for (PxU32 i = 0; i < numWheels; i++)
		{
			//Vertical suspension travel.
			suspTravelDirections[i] = PxVec3(0, -1, 0);

			//Wheel center offset is offset from rigid body center of mass.
			wheelCentreCMOffsets[i] = wheelCenterActorOffsets[i] - chassisCMOffset;

			//Suspension force application point 0.3 metres below rigid body center of mass.
			suspForceAppCMOffsets[i] = PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);

			//Tire force application point 0.3 metres below rigid body center of mass.
			tireForceAppCMOffsets[i] = PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
		}
	}

	//Set up the filter data of the raycast that will be issued by each suspension.
	PxFilterData qryFilterData;
	setupNonDrivableSurface(qryFilterData);

	//Set the wheel, tire and suspension data.
	//Set the geometry data.
	//Set the query filter data
	for (PxU32 i = 0; i < numWheels; i++)
	{
		wheelsSimData->setWheelData(i, wheels[i]);
		wheelsSimData->setTireData(i, tires[i]);
		wheelsSimData->setSuspensionData(i, suspensions[i]);
		wheelsSimData->setSuspTravelDirection(i, suspTravelDirections[i]);
		wheelsSimData->setWheelCentreOffset(i, wheelCentreCMOffsets[i]);
		wheelsSimData->setSuspForceAppPointOffset(i, suspForceAppCMOffsets[i]);
		wheelsSimData->setTireForceAppPointOffset(i, tireForceAppCMOffsets[i]);
		wheelsSimData->setSceneQueryFilterData(i, qryFilterData);
		wheelsSimData->setWheelShapeMapping(i, i);
	}
}

PxVehicleNoDrive* Physics::createVehicleNoDrive(const Physics::VehicleDesc& vehicleDesc, PxPhysics* physics, PxCooking* cooking)
{
	const PxVec3 chassisDims = vehicleDesc.chassisDims;
	const PxF32 wheelWidth = vehicleDesc.wheelWidth;
	const PxF32 wheelRadius = vehicleDesc.wheelRadius;
	const PxU32 numWheels = vehicleDesc.numWheels;

	//Construct a physx actor with shapes for the chassis and wheels.
	//Set the rigid body mass, moment of inertia, and center of mass offset.
	PxRigidDynamic* vehActor = NULL;
	{
		//Construct a convex mesh for a cylindrical wheel.
		PxConvexMesh* wheelMesh = Physics::createWheelMesh(wheelWidth, wheelRadius, *physics, *cooking);
		//Assume all wheels are identical for simplicity.
		PxConvexMesh* wheelConvexMeshes[PX_MAX_NB_WHEELS];
		PxMaterial* wheelMaterials[PX_MAX_NB_WHEELS];

		//Set the meshes and materials for the driven wheels.
		for (PxU32 i = 0; i < numWheels; i++)
		{
			wheelConvexMeshes[i] = wheelMesh;
			wheelMaterials[i] = vehicleDesc.wheelMaterial;
		}

		//Chassis just has a single convex shape for simplicity.
		PxConvexMesh* chassisConvexMesh = Physics::createChassisMesh(chassisDims, *physics, *cooking);
		PxConvexMesh* chassisConvexMeshes[1] = { chassisConvexMesh };
		PxMaterial* chassisMaterials[1] = { vehicleDesc.chassisMaterial };

		//Rigid body data.
		PxVehicleChassisData rigidBodyData;
		rigidBodyData.mMOI = vehicleDesc.chassisMOI;
		rigidBodyData.mMass = vehicleDesc.chassisMass;
		rigidBodyData.mCMOffset = vehicleDesc.chassisCMOffset;

		vehActor = Physics::createVehicleActor
		(rigidBodyData,
			wheelMaterials, wheelConvexMeshes, numWheels,
			chassisMaterials, chassisConvexMeshes, 1,
			*physics
		);
	}

	//Set up the sim data for the wheels.
	PxVehicleWheelsSimData* wheelsSimData = PxVehicleWheelsSimData::allocate(numWheels);
	{
		//Compute the wheel center offsets from the origin.
		PxVec3 wheelCentreActorOffsets[PX_MAX_NB_WHEELS];
		const PxF32 frontZ = chassisDims.z*0.3f;
		const PxF32 rearZ = -chassisDims.z*0.3f;
		computeWheelCenterActorOffsets(frontZ, rearZ, chassisDims, wheelWidth, wheelRadius, numWheels, wheelCentreActorOffsets);

		setupWheelsSimulationData
		(vehicleDesc.wheelMass, vehicleDesc.wheelMOI, wheelRadius, wheelWidth,
			numWheels, wheelCentreActorOffsets,
			vehicleDesc.chassisCMOffset, vehicleDesc.chassisMass,
			wheelsSimData
		);
	}

	//Create a vehicle from the wheels and drive sim data.
	PxVehicleNoDrive* vehDriveNoDrive = PxVehicleNoDrive::allocate(numWheels);
	vehDriveNoDrive->setup(physics, vehActor, *wheelsSimData);

	//Free the sim data because we don't need that any more.
	wheelsSimData->free();

	return vehDriveNoDrive;
}

PxFilterFlags VehicleFilterShader
(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);

	if ((0 == (filterData0.word0 & filterData1.word1)) &&
		(0 == (filterData1.word0 & filterData0.word1))
		)
		return PxFilterFlag::eSUPPRESS;

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlags();
}

/*void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)SAMPLE_ALLOC(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	SAMPLE_FREE(shapes);
}

void setFilters()
{
	setupFiltering(gVehicleNoDrive->getRigidDynamicActor, Physics::CollisionTypes::COLLISION_FLAG_CHASSIS, Physics::CollisionTypes::CH);
	setupFiltering(link, FilterGroup::eMINE_LINK, FilterGroup::eSUBMARINE);
	setupFiltering(mineHead, FilterGroup::eMINE_HEAD, FilterGroup::eSUBMARINE);
}*/


PxRigidDynamic* Physics::createVehicleActor
(const PxVehicleChassisData& chassisData,
	PxMaterial** wheelMaterials, PxConvexMesh** wheelConvexMeshes, const PxU32 numWheels,
	PxMaterial** chassisMaterials, PxConvexMesh** chassisConvexMeshes, const PxU32 numChassisMeshes,
	PxPhysics& physics)
{
	//We need a rigid body actor for the vehicle.
	//Don't forget to add the actor to the scene after setting up the associated vehicle.
	PxRigidDynamic* vehActor = physics.createRigidDynamic(PxTransform(PxIdentity));

	//Wheel and chassis simulation filter data.
	PxFilterData wheelSimFilterData;
	wheelSimFilterData.word0 = Physics::CollisionTypes::COLLISION_FLAG_WHEEL;
	wheelSimFilterData.word1 = Physics::CollisionTypes::COLLISION_FLAG_WHEEL_AGAINST;
	PxFilterData chassisSimFilterData;
	chassisSimFilterData.word0 = Physics::CollisionTypes::COLLISION_FLAG_CHASSIS;
	chassisSimFilterData.word1 = Physics::CollisionTypes::COLLISION_FLAG_CHASSIS_AGAINST;

	//Wheel and chassis query filter data.
	//Optional: cars don't drive on other cars.
	PxFilterData wheelQryFilterData;
	setupNonDrivableSurface(wheelQryFilterData);
	PxFilterData chassisQryFilterData;
	setupNonDrivableSurface(chassisQryFilterData);

	//Add all the wheel shapes to the actor.
	for (PxU32 i = 0; i < numWheels; i++)
	{
		PxConvexMeshGeometry geom(wheelConvexMeshes[i]);
		PxShape* wheelShape = vehActor->createShape(geom, *wheelMaterials[i]);
		wheelShape->setQueryFilterData(wheelQryFilterData);
		wheelShape->setSimulationFilterData(wheelSimFilterData);
		wheelShape->setLocalPose(PxTransform(PxIdentity));
	}

	//Add the chassis shapes to the actor.
	for (PxU32 i = 0; i < numChassisMeshes; i++)
	{
		PxShape* chassisShape = vehActor->createShape
		(PxConvexMeshGeometry(chassisConvexMeshes[i]), *chassisMaterials[i]);
		chassisShape->setQueryFilterData(chassisQryFilterData);
		chassisShape->setSimulationFilterData(chassisSimFilterData);
		chassisShape->setLocalPose(PxTransform(PxIdentity));
	}

	vehActor->setMass(chassisData.mMass);
	vehActor->setMassSpaceInertiaTensor(chassisData.mMOI);
	vehActor->setCMassLocalPose(PxTransform(chassisData.mCMOffset, PxQuat(PxIdentity)));

	return vehActor;
}

PxRigidStatic* Physics::createDrivablePlane(PxMaterial* material, PxPhysics* physics)
{
	//Add a plane to the scene.
	PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *material);

	//Get the plane shape so we can set query and simulation filter data.
	PxShape* shapes[1];
	groundPlane->getShapes(shapes, 1);

	//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.
	physx::PxFilterData qryFilterData;
	setupDrivableSurface(qryFilterData);
	shapes[0]->setQueryFilterData(qryFilterData);

	//Set the simulation filter data of the ground plane so that it collides with the chassis of a vehicle but not the wheels.
	PxFilterData simFilterData;
	simFilterData.word0 = Physics::CollisionTypes::COLLISION_FLAG_GROUND;
	simFilterData.word1 = Physics::CollisionTypes::COLLISION_FLAG_GROUND_AGAINST;
	shapes[0]->setSimulationFilterData(simFilterData);

	return groundPlane;
}

PxRigidDynamic* Physics::createTestBox(PxReal sideLength)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(sideLength, sideLength, sideLength), *gMaterial);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxIdentity));

	PxFilterData boxSimFilterData;
	boxSimFilterData.word0 = Physics::CollisionTypes::COLLISION_FLAG_OBSTACLE;
	boxSimFilterData.word1 = Physics::CollisionTypes::COLLISION_FLAG_OBSTACLE_AGAINST;
	shape->setSimulationFilterData(boxSimFilterData);
	body->attachShape(*shape);
	//PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	gScene->addActor(*body);
	return body;
}

PxRigidDynamic* Physics::createTestProjectile()
{
	PxShape* shape = gPhysics->createShape(PxSphereGeometry(0.1), *gMaterial);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxIdentity));

	PxFilterData projSimFilterData;
	projSimFilterData.word0 = Physics::CollisionTypes::COLLISION_FLAG_PROJECTILE;
	projSimFilterData.word1 = Physics::CollisionTypes::COLLISION_FLAG_PROJECTILE_AGAINST;
	shape->setSimulationFilterData(projSimFilterData);
	body->attachShape(*shape);
	//PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	gScene->addActor(*body);
	return body;
}

static PxConvexMesh* createConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics& physics, PxCooking& cooking)
{
	// Create descriptor for convex mesh
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = numVerts;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = verts;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eINFLATE_CONVEX;

	PxConvexMesh* convexMesh = NULL;
	PxDefaultMemoryOutputStream buf;
	if (cooking.cookConvexMesh(convexDesc, buf))
	{
		PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
		convexMesh = physics.createConvexMesh(id);
	}

	return convexMesh;
}

PxConvexMesh* Physics::createChassisMesh(const PxVec3 dims, PxPhysics& physics, PxCooking& cooking)
{
	const PxF32 x = dims.x*0.5f;
	const PxF32 y = dims.y*0.5f;
	const PxF32 z = dims.z*0.5f;
	PxVec3 verts[8] =
	{
		PxVec3(x,y,-z),
		PxVec3(x,y,z),
		PxVec3(x,-y,z),
		PxVec3(x,-y,-z),
		PxVec3(-x,y,-z),
		PxVec3(-x,y,z),
		PxVec3(-x,-y,z),
		PxVec3(-x,-y,-z)
	};

	return createConvexMesh(verts, 8, physics, cooking);
}

PxConvexMesh* Physics::createWheelMesh(const PxF32 width, const PxF32 radius, PxPhysics& physics, PxCooking& cooking)
{
	PxVec3 points[2 * 16];
	for (PxU32 i = 0; i < 16; i++)
	{
		const PxF32 cosTheta = PxCos(i*PxPi*2.0f / 16.0f);
		const PxF32 sinTheta = PxSin(i*PxPi*2.0f / 16.0f);
		const PxF32 y = radius*cosTheta;
		const PxF32 z = radius*sinTheta;
		points[2 * i + 0] = PxVec3(-width / 2.0f, y, z);
		points[2 * i + 1] = PxVec3(+width / 2.0f, y, z);
	}

	return createConvexMesh(points, 32, physics, cooking);
}

void Physics::customizeVehicleToLengthScale(const PxReal lengthScale, PxRigidDynamic* rigidDynamic, PxVehicleWheelsSimData* wheelsSimData, PxVehicleDriveSimData* driveSimData)
{
	//Rigid body center of mass and moment of inertia.
	{
		PxTransform t = rigidDynamic->getCMassLocalPose();
		t.p *= lengthScale;
		rigidDynamic->setCMassLocalPose(t);

		PxVec3 moi = rigidDynamic->getMassSpaceInertiaTensor();
		moi *= (lengthScale*lengthScale);
		rigidDynamic->setMassSpaceInertiaTensor(moi);
	}

	//Wheels, suspensions, wheel centers, tire/susp force application points.
	{
		for (PxU32 i = 0; i < wheelsSimData->getNbWheels(); i++)
		{
			PxVehicleWheelData wheelData = wheelsSimData->getWheelData(i);
			wheelData.mRadius *= lengthScale;
			wheelData.mWidth *= lengthScale;
			wheelData.mDampingRate *= lengthScale*lengthScale;//WHEEL_DAMPING_RATE
			wheelData.mMaxBrakeTorque *= lengthScale*lengthScale;
			wheelData.mMaxHandBrakeTorque *= lengthScale*lengthScale;
			wheelData.mMOI *= lengthScale*lengthScale;
			wheelsSimData->setWheelData(i, wheelData);

			PxVehicleSuspensionData suspData = wheelsSimData->getSuspensionData(i);
			suspData.mMaxCompression *= lengthScale;
			suspData.mMaxDroop *= lengthScale;
			wheelsSimData->setSuspensionData(i, suspData);

			PxVec3 v = wheelsSimData->getWheelCentreOffset(i);
			v *= lengthScale;
			wheelsSimData->setWheelCentreOffset(i, v);

			v = wheelsSimData->getSuspForceAppPointOffset(i);
			v *= lengthScale;
			wheelsSimData->setSuspForceAppPointOffset(i, v);

			v = wheelsSimData->getTireForceAppPointOffset(i);
			v *= lengthScale;
			wheelsSimData->setTireForceAppPointOffset(i, v);
		}
	}

	//Slow forward speed correction.
	{
		wheelsSimData->setSubStepCount(5.0f*lengthScale, 3, 1);
		wheelsSimData->setMinLongSlipDenominator(4.0f*lengthScale);
	}

	//Engine
	if (driveSimData)
	{
		PxVehicleEngineData engineData = driveSimData->getEngineData();
		engineData.mMOI *= lengthScale*lengthScale;
		engineData.mPeakTorque *= lengthScale*lengthScale;
		engineData.mDampingRateFullThrottle *= lengthScale*lengthScale;
		engineData.mDampingRateZeroThrottleClutchEngaged *= lengthScale*lengthScale;
		engineData.mDampingRateZeroThrottleClutchDisengaged *= lengthScale*lengthScale;
		driveSimData->setEngineData(engineData);
	}

	//Clutch.
	if (driveSimData)
	{
		PxVehicleClutchData clutchData = driveSimData->getClutchData();
		clutchData.mStrength *= lengthScale*lengthScale;
		driveSimData->setClutchData(clutchData);
	}

	//Scale the collision meshes too.
	{
		PxShape* shapes[16];
		const PxU32 nbShapes = rigidDynamic->getShapes(shapes, 16);
		for (PxU32 i = 0; i < nbShapes; i++)
		{
			switch (shapes[i]->getGeometryType())
			{
			case PxGeometryType::eSPHERE:
			{
				PxSphereGeometry sphere;
				shapes[i]->getSphereGeometry(sphere);
				sphere.radius *= lengthScale;
				shapes[i]->setGeometry(sphere);
			}
			break;
			case PxGeometryType::ePLANE:
				PX_ASSERT(false);
				break;
			case PxGeometryType::eCAPSULE:
			{
				PxCapsuleGeometry capsule;
				shapes[i]->getCapsuleGeometry(capsule);
				capsule.radius *= lengthScale;
				capsule.halfHeight *= lengthScale;
				shapes[i]->setGeometry(capsule);
			}
			break;
			case PxGeometryType::eBOX:
			{
				PxBoxGeometry box;
				shapes[i]->getBoxGeometry(box);
				box.halfExtents *= lengthScale;
				shapes[i]->setGeometry(box);
			}
			break;
			case PxGeometryType::eCONVEXMESH:
			{
				PxConvexMeshGeometry convexMesh;
				shapes[i]->getConvexMeshGeometry(convexMesh);
				convexMesh.scale.scale *= lengthScale;
				shapes[i]->setGeometry(convexMesh);
			}
			break;
			case PxGeometryType::eTRIANGLEMESH:
			{
				PxTriangleMeshGeometry triMesh;
				shapes[i]->getTriangleMeshGeometry(triMesh);
				triMesh.scale.scale *= lengthScale;
				shapes[i]->setGeometry(triMesh);
			}
			break;
			case PxGeometryType::eHEIGHTFIELD:
			{
				PxHeightFieldGeometry hf;
				shapes[i]->getHeightFieldGeometry(hf);
				hf.columnScale *= lengthScale;
				hf.heightScale *= lengthScale;
				hf.rowScale *= lengthScale;
				shapes[i]->setGeometry(hf);
			}
			break;
			case PxGeometryType::eINVALID:
			case PxGeometryType::eGEOMETRY_COUNT:
				break;
			}
		}
	}
}

void Physics::customizeVehicleToLengthScale(const PxReal lengthScale, PxRigidDynamic* rigidDynamic, PxVehicleWheelsSimData* wheelsSimData, PxVehicleDriveSimData4W* driveSimData)
{
	Physics::customizeVehicleToLengthScale(lengthScale, rigidDynamic, wheelsSimData, (PxVehicleDriveSimData*)driveSimData);

	//Ackermann geometry.
	if (driveSimData)
	{
		PxVehicleAckermannGeometryData ackermannData = driveSimData->getAckermannGeometryData();
		ackermannData.mAxleSeparation *= lengthScale;
		ackermannData.mFrontWidth *= lengthScale;
		ackermannData.mRearWidth *= lengthScale;
		driveSimData->setAckermannGeometryData(ackermannData);
	}
}
//Vehicle tire code
//Tire model friction for each combination of drivable surface type and tire type.
static PxF32 gTireFrictionMultipliers[Physics::SurfaceTypes::MAX_NUM_SURFACE_TYPES][Physics::TireTypes::MAX_NUM_TIRE_TYPES] =
{
	//NORMAL,	WORN
	{ 15.00f,		0.1f }//TARMAC FRICTION VALUE FOR TIRE
};

PxVehicleDrivableSurfaceToTireFrictionPairs* Physics::createFrictionPairs(const PxMaterial* defaultMaterial)
{
	PxVehicleDrivableSurfaceType surfaceTypes[1];
	surfaceTypes[0].mType = Physics::SurfaceTypes::SURFACE_TYPE_TARMAC;

	const PxMaterial* surfaceMaterials[1];
	surfaceMaterials[0] = defaultMaterial;

	PxVehicleDrivableSurfaceToTireFrictionPairs* surfaceTirePairs =
		PxVehicleDrivableSurfaceToTireFrictionPairs::allocate(Physics::TireTypes::MAX_NUM_TIRE_TYPES, Physics::SurfaceTypes::MAX_NUM_SURFACE_TYPES);

	surfaceTirePairs->setup(Physics::TireTypes::MAX_NUM_TIRE_TYPES, Physics::SurfaceTypes::MAX_NUM_SURFACE_TYPES, surfaceMaterials, surfaceTypes);

	for (PxU32 i = 0; i < Physics::SurfaceTypes::MAX_NUM_SURFACE_TYPES; i++)
	{
		for (PxU32 j = 0; j < Physics::TireTypes::MAX_NUM_TIRE_TYPES; j++)
		{
			surfaceTirePairs->setTypePairFriction(i, j, gTireFrictionMultipliers[i][j]);
		}
	}
	return surfaceTirePairs;
}



//Vehicle initialization code
Physics::VehicleDesc Physics::initVehicleDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 500.0f;
	const PxVec3 chassisDims(2.0f, 1.0f, 2.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.0f, 0.25f);						//Center of mass

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 10.0f;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.4f;
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

	Physics::VehicleDesc vehicleDesc;
	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = gMaterial;
	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = gMaterial;
	return vehicleDesc;
}
