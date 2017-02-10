#include "CommonIncludes.h"
#include <PxPhysicsAPI.h>
#include <PxTransform.h>
#include <PxRigidBodyExt.h>

//#include <PhysX-3.3-master\PhysXSDK\Snippets\SnippetVehicleCommon\SnippetVehicleRaycast.h>
//#include <PhysX-3.3-master\PhysXSDK\Snippets\SnippetVehicleCommon\SnippetVehicleRaycast.cpp>
#include "VehicleRaycast.h"


class Physics
{

public:
	enum SurfaceTypes
	{
		SURFACE_TYPE_TARMAC,
		MAX_NUM_SURFACE_TYPES
	};

	//Tire types.
	enum TireTypes
	{
		TIRE_TYPE_NORMAL = 0,
		TIRE_TYPE_WORN,
		MAX_NUM_TIRE_TYPES
	};

	enum CollisionTypes
	{
		COLLISION_FLAG_GROUND = 1 << 0,
		COLLISION_FLAG_WHEEL = 1 << 1,
		COLLISION_FLAG_CHASSIS = 1 << 2,
		COLLISION_FLAG_OBSTACLE = 1 << 3,
		COLLISION_FLAG_DRIVABLE_OBSTACLE = 1 << 4,

		COLLISION_FLAG_GROUND_AGAINST = COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
		COLLISION_FLAG_WHEEL_AGAINST = COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE,
		COLLISION_FLAG_CHASSIS_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
		COLLISION_FLAG_OBSTACLE_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
		COLLISION_FLAG_DRIVABLE_OBSTACLE_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
	};

	//Physics();
	//~Physics();
	static void initializePhysX();
	static physx::PxRigidDynamic* createTestBox(physx::PxReal sideLength);
	static void computeRotation(physx::PxQuat angle);
	static void stepPhysics();
	static void cleanupPhysics();

	//PxVehicleSetup
	static void computeWheelCenterActorOffsets(const physx::PxF32 wheelFrontZ, const physx::PxF32 wheelRearZ, const physx::PxVec3& chassisDims, const physx::PxF32 wheelWidth, const physx::PxF32 wheelRadius, const physx::PxU32 numWheels, physx::PxVec3* wheelCentreOffsets);
	static physx::PxRigidStatic* createDrivablePlane(physx::PxMaterial* material, physx::PxPhysics* physics);

	static void Physics::setupWheelsSimulationData
	(const physx::PxF32 wheelMass, const physx::PxF32 wheelMOI, const physx::PxF32 wheelRadius, const physx::PxF32 wheelWidth,
		const physx::PxU32 numWheels, const physx::PxVec3* wheelCenterActorOffsets,
		const physx::PxVec3& chassisCMOffset, const physx::PxF32 chassisMass,
		physx::PxVehicleWheelsSimData* wheelsSimData);

	////////////////////////////////////////////////

	static physx::PxConvexMesh* createChassisMesh(const physx::PxVec3 dims, physx::PxPhysics& physics, physx::PxCooking& cooking);

	static physx::PxConvexMesh* createWheelMesh(const physx::PxF32 width, const physx::PxF32 radius, physx::PxPhysics& physics, physx::PxCooking& cooking);

	////////////////////////////////////////////////

	static physx::PxRigidDynamic* createVehicleActor
	(const physx::PxVehicleChassisData& chassisData,
		physx::PxMaterial** wheelMaterials, physx::PxConvexMesh** wheelConvexMeshes, const physx::PxU32 numWheels,
		physx::PxMaterial** chassisMaterials, physx::PxConvexMesh** chassisConvexMeshes, const physx::PxU32 numChassisMeshes,
		physx::PxPhysics& physics);

	////////////////////////////////////////////////

	struct VehicleDesc
	{
		physx::PxF32 chassisMass;
		physx::PxVec3 chassisDims;
		physx::PxVec3 chassisMOI;
		physx::PxVec3 chassisCMOffset;
		physx::PxMaterial* chassisMaterial;
		physx::PxF32 wheelMass;
		physx::PxF32 wheelWidth;
		physx::PxF32 wheelRadius;
		physx::PxF32 wheelMOI;
		physx::PxMaterial* wheelMaterial;
		physx::PxU32 numWheels;
	};


	static physx::PxVehicleNoDrive* createVehicleNoDrive(const VehicleDesc& vehDesc, physx::PxPhysics* physics, physx::PxCooking* cooking);

	static void customizeVehicleToLengthScale(const physx::PxReal lengthScale, physx::PxRigidDynamic* rigidDynamic, physx::PxVehicleWheelsSimData* wheelsSimData, physx::PxVehicleDriveSimData* driveSimData);

	static void customizeVehicleToLengthScale(const physx::PxReal lengthScale, physx::PxRigidDynamic* rigidDynamic, physx::PxVehicleWheelsSimData* wheelsSimData, physx::PxVehicleDriveSimData4W* driveSimData);
private:

};
