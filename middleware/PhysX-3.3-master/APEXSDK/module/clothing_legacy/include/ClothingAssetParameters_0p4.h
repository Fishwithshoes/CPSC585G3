/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


// This file was generated by NxParameterized/scripts/GenParameterized.pl
// Created: 2015.10.22 17:26:30

#ifndef HEADER_ClothingAssetParameters_0p4_h
#define HEADER_ClothingAssetParameters_0p4_h

#include "NxParametersTypes.h"

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
#include "NxParameterized.h"
#include "NxParameters.h"
#include "NxParameterizedTraits.h"
#include "NxTraitsInternal.h"
#endif

namespace physx
{
namespace apex
{

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

namespace ClothingAssetParameters_0p4NS
{

struct BoneEntry_Type;
struct ActorEntry_Type;
struct CookedEntry_Type;
struct SimulationParams_Type;

struct REF_DynamicArray1D_Type
{
	NxParameterized::Interface** buf;
	bool isAllocated;
	physx::PxI32 elementSize;
	physx::PxI32 arraySizes[1];
};

struct BoneEntry_DynamicArray1D_Type
{
	BoneEntry_Type* buf;
	bool isAllocated;
	physx::PxI32 elementSize;
	physx::PxI32 arraySizes[1];
};

struct ActorEntry_DynamicArray1D_Type
{
	ActorEntry_Type* buf;
	bool isAllocated;
	physx::PxI32 elementSize;
	physx::PxI32 arraySizes[1];
};

struct VEC3_DynamicArray1D_Type
{
	physx::PxVec3* buf;
	bool isAllocated;
	physx::PxI32 elementSize;
	physx::PxI32 arraySizes[1];
};

struct CookedEntry_DynamicArray1D_Type
{
	CookedEntry_Type* buf;
	bool isAllocated;
	physx::PxI32 elementSize;
	physx::PxI32 arraySizes[1];
};

struct SimulationParams_Type
{
	physx::PxU32 hierarchicalLevels;
	physx::PxF32 thickness;
	physx::PxF32 selfcollisionThickness;
	physx::PxVec3 gravityDirection;
	physx::PxF32 sleepLinearVelocity;
	bool disableCCD;
	bool untangling;
	bool selfcollision;
	bool twowayInteraction;
};
struct CookedEntry_Type
{
	physx::PxF32 scale;
	NxParameterized::Interface* cookedData;
};
struct BoneEntry_Type
{
	physx::PxI32 internalIndex;
	physx::PxI32 externalIndex;
	physx::PxU32 numMeshReferenced;
	physx::PxU32 numRigidBodiesReferenced;
	physx::PxI32 parentIndex;
	physx::PxMat34Legacy bindPose;
	NxParameterized::DummyStringStruct name;
};
struct ActorEntry_Type
{
	physx::PxI32 boneIndex;
	physx::PxU32 convexVerticesStart;
	physx::PxU32 convexVerticesCount;
	physx::PxF32 capsuleRadius;
	physx::PxF32 capsuleHeight;
	physx::PxMat34Legacy localPose;
};

struct ParametersStruct
{

	REF_DynamicArray1D_Type physicalMeshes;
	REF_DynamicArray1D_Type graphicalLods;
	SimulationParams_Type simulation;
	BoneEntry_DynamicArray1D_Type bones;
	physx::PxU32 bonesReferenced;
	physx::PxU32 bonesReferencedByMesh;
	ActorEntry_DynamicArray1D_Type boneActors;
	VEC3_DynamicArray1D_Type boneVertices;
	CookedEntry_DynamicArray1D_Type cookedData;
	physx::PxBounds3 boundingBox;
	NxParameterized::Interface* materialLibrary;
	physx::PxU32 materialIndex;

};

static const physx::PxU32 checksum[] = { 0xfb509bc0, 0x1f80b5fa, 0x4ebbea14, 0x8c9c33b5, };

} // namespace ClothingAssetParameters_0p4NS

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
class ClothingAssetParameters_0p4 : public NxParameterized::NxParameters, public ClothingAssetParameters_0p4NS::ParametersStruct
{
public:
	ClothingAssetParameters_0p4(NxParameterized::Traits* traits, void* buf = 0, PxI32* refCount = 0);

	virtual ~ClothingAssetParameters_0p4();

	virtual void destroy();

	static const char* staticClassName(void)
	{
		return("ClothingAssetParameters");
	}

	const char* className(void) const
	{
		return(staticClassName());
	}

	static const physx::PxU32 ClassVersion = ((physx::PxU32)0 << 16) + (physx::PxU32)4;

	static physx::PxU32 staticVersion(void)
	{
		return ClassVersion;
	}

	physx::PxU32 version(void) const
	{
		return(staticVersion());
	}

	static const physx::PxU32 ClassAlignment = 8;

	static const physx::PxU32* staticChecksum(physx::PxU32& bits)
	{
		bits = 8 * sizeof(ClothingAssetParameters_0p4NS::checksum);
		return ClothingAssetParameters_0p4NS::checksum;
	}

	static void freeParameterDefinitionTable(NxParameterized::Traits* traits);

	const physx::PxU32* checksum(physx::PxU32& bits) const
	{
		return staticChecksum(bits);
	}

	const ClothingAssetParameters_0p4NS::ParametersStruct& parameters(void) const
	{
		ClothingAssetParameters_0p4* tmpThis = const_cast<ClothingAssetParameters_0p4*>(this);
		return *(static_cast<ClothingAssetParameters_0p4NS::ParametersStruct*>(tmpThis));
	}

	ClothingAssetParameters_0p4NS::ParametersStruct& parameters(void)
	{
		return *(static_cast<ClothingAssetParameters_0p4NS::ParametersStruct*>(this));
	}

	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle) const;
	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle);

	void initDefaults(void);

protected:

	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void);
	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void) const;


	virtual void getVarPtr(const NxParameterized::Handle& handle, void*& ptr, size_t& offset) const;

private:

	void buildTree(void);
	void initDynamicArrays(void);
	void initStrings(void);
	void initReferences(void);
	void freeDynamicArrays(void);
	void freeStrings(void);
	void freeReferences(void);

	static bool mBuiltFlag;
	static NxParameterized::MutexType mBuiltFlagMutex;
};

class ClothingAssetParameters_0p4Factory : public NxParameterized::Factory
{
	static const char* const vptr;

public:
	virtual NxParameterized::Interface* create(NxParameterized::Traits* paramTraits)
	{
		// placement new on this class using mParameterizedTraits

		void* newPtr = paramTraits->alloc(sizeof(ClothingAssetParameters_0p4), ClothingAssetParameters_0p4::ClassAlignment);
		if (!NxParameterized::IsAligned(newPtr, ClothingAssetParameters_0p4::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingAssetParameters_0p4");
			paramTraits->free(newPtr);
			return 0;
		}

		memset(newPtr, 0, sizeof(ClothingAssetParameters_0p4)); // always initialize memory allocated to zero for default values
		return NX_PARAM_PLACEMENT_NEW(newPtr, ClothingAssetParameters_0p4)(paramTraits);
	}

	virtual NxParameterized::Interface* finish(NxParameterized::Traits* paramTraits, void* bufObj, void* bufStart, physx::PxI32* refCount)
	{
		if (!NxParameterized::IsAligned(bufObj, ClothingAssetParameters_0p4::ClassAlignment)
		        || !NxParameterized::IsAligned(bufStart, ClothingAssetParameters_0p4::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingAssetParameters_0p4");
			return 0;
		}

		// Init NxParameters-part
		// We used to call empty constructor of ClothingAssetParameters_0p4 here
		// but it may call default constructors of members and spoil the data
		NX_PARAM_PLACEMENT_NEW(bufObj, NxParameterized::NxParameters)(paramTraits, bufStart, refCount);

		// Init vtable (everything else is already initialized)
		*(const char**)bufObj = vptr;

		return (ClothingAssetParameters_0p4*)bufObj;
	}

	virtual const char* getClassName()
	{
		return (ClothingAssetParameters_0p4::staticClassName());
	}

	virtual physx::PxU32 getVersion()
	{
		return (ClothingAssetParameters_0p4::staticVersion());
	}

	virtual physx::PxU32 getAlignment()
	{
		return (ClothingAssetParameters_0p4::ClassAlignment);
	}

	virtual const physx::PxU32* getChecksum(physx::PxU32& bits)
	{
		return (ClothingAssetParameters_0p4::staticChecksum(bits));
	}
};
#endif // NX_PARAMETERIZED_ONLY_LAYOUTS

} // namespace apex
} // namespace physx

#pragma warning(pop)

#endif
