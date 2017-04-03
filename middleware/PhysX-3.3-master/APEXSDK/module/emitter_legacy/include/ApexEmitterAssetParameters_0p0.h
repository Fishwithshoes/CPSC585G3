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

#ifndef HEADER_ApexEmitterAssetParameters_0p0_h
#define HEADER_ApexEmitterAssetParameters_0p0_h

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

namespace ApexEmitterAssetParameters_0p0NS
{

struct rangeStructF32_Type;
struct rangeStructVec3_Type;
struct emitterLodParamDesc_Type;

struct emitterLodParamDesc_Type
{
	physx::PxU32 version;
	physx::PxF32 maxDistance;
	physx::PxF32 distanceWeight;
	physx::PxF32 speedWeight;
	physx::PxF32 lifeWeight;
	physx::PxF32 separationWeight;
	physx::PxF32 bias;
};
struct rangeStructVec3_Type
{
	physx::PxVec3 min;
	physx::PxVec3 max;
};
struct rangeStructF32_Type
{
	physx::PxF32 min;
	physx::PxF32 max;
};

struct ParametersStruct
{

	rangeStructF32_Type densityRange;
	rangeStructF32_Type rateRange;
	rangeStructF32_Type lifetimeRange;
	rangeStructVec3_Type velocityRange;
	physx::PxU32 maxSamples;
	emitterLodParamDesc_Type lodParamDesc;
	NxParameterized::Interface* iofxAssetName;
	NxParameterized::Interface* iosAssetName;
	NxParameterized::Interface* geometryType;

};

static const physx::PxU32 checksum[] = { 0xd97d47e5, 0xad62c135, 0x535e8f07, 0x9f2e07c0, };

} // namespace ApexEmitterAssetParameters_0p0NS

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
class ApexEmitterAssetParameters_0p0 : public NxParameterized::NxParameters, public ApexEmitterAssetParameters_0p0NS::ParametersStruct
{
public:
	ApexEmitterAssetParameters_0p0(NxParameterized::Traits* traits, void* buf = 0, PxI32* refCount = 0);

	virtual ~ApexEmitterAssetParameters_0p0();

	virtual void destroy();

	static const char* staticClassName(void)
	{
		return("ApexEmitterAssetParameters");
	}

	const char* className(void) const
	{
		return(staticClassName());
	}

	static const physx::PxU32 ClassVersion = ((physx::PxU32)0 << 16) + (physx::PxU32)0;

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
		bits = 8 * sizeof(ApexEmitterAssetParameters_0p0NS::checksum);
		return ApexEmitterAssetParameters_0p0NS::checksum;
	}

	static void freeParameterDefinitionTable(NxParameterized::Traits* traits);

	const physx::PxU32* checksum(physx::PxU32& bits) const
	{
		return staticChecksum(bits);
	}

	const ApexEmitterAssetParameters_0p0NS::ParametersStruct& parameters(void) const
	{
		ApexEmitterAssetParameters_0p0* tmpThis = const_cast<ApexEmitterAssetParameters_0p0*>(this);
		return *(static_cast<ApexEmitterAssetParameters_0p0NS::ParametersStruct*>(tmpThis));
	}

	ApexEmitterAssetParameters_0p0NS::ParametersStruct& parameters(void)
	{
		return *(static_cast<ApexEmitterAssetParameters_0p0NS::ParametersStruct*>(this));
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

class ApexEmitterAssetParameters_0p0Factory : public NxParameterized::Factory
{
	static const char* const vptr;

public:
	virtual NxParameterized::Interface* create(NxParameterized::Traits* paramTraits)
	{
		// placement new on this class using mParameterizedTraits

		void* newPtr = paramTraits->alloc(sizeof(ApexEmitterAssetParameters_0p0), ApexEmitterAssetParameters_0p0::ClassAlignment);
		if (!NxParameterized::IsAligned(newPtr, ApexEmitterAssetParameters_0p0::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ApexEmitterAssetParameters_0p0");
			paramTraits->free(newPtr);
			return 0;
		}

		memset(newPtr, 0, sizeof(ApexEmitterAssetParameters_0p0)); // always initialize memory allocated to zero for default values
		return NX_PARAM_PLACEMENT_NEW(newPtr, ApexEmitterAssetParameters_0p0)(paramTraits);
	}

	virtual NxParameterized::Interface* finish(NxParameterized::Traits* paramTraits, void* bufObj, void* bufStart, physx::PxI32* refCount)
	{
		if (!NxParameterized::IsAligned(bufObj, ApexEmitterAssetParameters_0p0::ClassAlignment)
		        || !NxParameterized::IsAligned(bufStart, ApexEmitterAssetParameters_0p0::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ApexEmitterAssetParameters_0p0");
			return 0;
		}

		// Init NxParameters-part
		// We used to call empty constructor of ApexEmitterAssetParameters_0p0 here
		// but it may call default constructors of members and spoil the data
		NX_PARAM_PLACEMENT_NEW(bufObj, NxParameterized::NxParameters)(paramTraits, bufStart, refCount);

		// Init vtable (everything else is already initialized)
		*(const char**)bufObj = vptr;

		return (ApexEmitterAssetParameters_0p0*)bufObj;
	}

	virtual const char* getClassName()
	{
		return (ApexEmitterAssetParameters_0p0::staticClassName());
	}

	virtual physx::PxU32 getVersion()
	{
		return (ApexEmitterAssetParameters_0p0::staticVersion());
	}

	virtual physx::PxU32 getAlignment()
	{
		return (ApexEmitterAssetParameters_0p0::ClassAlignment);
	}

	virtual const physx::PxU32* getChecksum(physx::PxU32& bits)
	{
		return (ApexEmitterAssetParameters_0p0::staticChecksum(bits));
	}
};
#endif // NX_PARAMETERIZED_ONLY_LAYOUTS

} // namespace apex
} // namespace physx

#pragma warning(pop)

#endif
