/*
* Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef VEHICLE_RAYCAST_H
#define VEHICLE_RAYCAST_H

#include "PxPhysicsAPI.h"

enum
{
	DRIVABLE_SURFACE = 0xffff0000,
	UNDRIVABLE_SURFACE = 0x0000ffff
};

void setupDrivableSurface(physx::PxFilterData& filterData);

void setupNonDrivableSurface(physx::PxFilterData& filterData);

physx::PxQueryHitType::Enum WheelRaycastPreFilter
(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
	const void* constantBlock, physx::PxU32 constantBlockSize,
	physx::PxHitFlags& queryFlags);

//Data structure for quick setup of scene queries for suspension raycasts.
class VehicleSceneQueryData
{
public:
	VehicleSceneQueryData();
	~VehicleSceneQueryData();

	//Allocate scene query data for up to maxNumVehicles and up to maxNumWheelsPerVehicle with numVehiclesInBatch per batch query.
	static VehicleSceneQueryData* allocate(const physx::PxU32 maxNumVehicles, const physx::PxU32 maxNumWheelsPerVehicle, const physx::PxU32 numVehiclesInBatch, physx::PxAllocatorCallback& allocator);

	//Free allocated buffers.
	void free(physx::PxAllocatorCallback& allocator);

	//Create a PxBatchQuery instance that will be used for a single specified batch.
	static physx::PxBatchQuery* setUpBatchedSceneQuery(const physx::PxU32 batchId, const VehicleSceneQueryData& vehicleSceneQueryData, physx::PxScene* scene);

	//Return an array of scene query results for a single specified batch.
	physx::PxRaycastQueryResult* getRaycastQueryResultBuffer(const physx::PxU32 batchId);

	//Get the number of scene query results that have been allocated for a single batch.
	physx::PxU32 getRaycastQueryResultBufferSize() const;

private:

	//Number of raycasts per batch
	physx::PxU32 mNumRaycastsPerBatch;

	//One result for each wheel.
	physx::PxRaycastQueryResult* mSqResults;

	//One hit for each wheel.
	physx::PxRaycastHit* mSqHitBuffer;

	//Filter shader used to filter drivable and non-drivable surfaces
	physx::PxBatchQueryPreFilterShader mPreFilterShader;
};


#endif //VEHICLE_RAYCAST_H