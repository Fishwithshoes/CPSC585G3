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
// Created: 2015.10.22 17:26:19

#include "VertexBufferParameters.h"
#include <string.h>
#include <stdlib.h>

using namespace NxParameterized;

namespace physx
{
namespace apex
{

using namespace VertexBufferParametersNS;

const char* const VertexBufferParametersFactory::vptr =
    NxParameterized::getVptr<VertexBufferParameters, VertexBufferParameters::ClassAlignment>();

const physx::PxU32 NumParamDefs = 5;
static NxParameterized::DefinitionImpl* ParamDefTable; // now allocated in buildTree [NumParamDefs];


static const size_t ParamLookupChildrenTable[] =
{
	1, 2, 3, 4,
};

#define TENUM(type) physx::##type
#define CHILDREN(index) &ParamLookupChildrenTable[index]
static const NxParameterized::ParamLookupNode ParamLookupTable[NumParamDefs] =
{
	{ TYPE_STRUCT, false, 0, CHILDREN(0), 3 },
	{ TYPE_U32, false, (size_t)(&((ParametersStruct*)0)->vertexCount), NULL, 0 }, // vertexCount
	{ TYPE_REF, false, (size_t)(&((ParametersStruct*)0)->vertexFormat), NULL, 0 }, // vertexFormat
	{ TYPE_ARRAY, true, (size_t)(&((ParametersStruct*)0)->buffers), CHILDREN(3), 1 }, // buffers
	{ TYPE_REF, false, 1 * sizeof(NxParameterized::Interface*), NULL, 0 }, // buffers[]
};


bool VertexBufferParameters::mBuiltFlag = false;
NxParameterized::MutexType VertexBufferParameters::mBuiltFlagMutex;

VertexBufferParameters::VertexBufferParameters(NxParameterized::Traits* traits, void* buf, PxI32* refCount) :
	NxParameters(traits, buf, refCount)
{
	//mParameterizedTraits->registerFactory(className(), &VertexBufferParametersFactoryInst);

	if (!buf) //Do not init data if it is inplace-deserialized
	{
		initDynamicArrays();
		initStrings();
		initReferences();
		initDefaults();
	}
}

VertexBufferParameters::~VertexBufferParameters()
{
	freeStrings();
	freeReferences();
	freeDynamicArrays();
}

void VertexBufferParameters::destroy()
{
	// We cache these fields here to avoid overwrite in destructor
	bool doDeallocateSelf = mDoDeallocateSelf;
	NxParameterized::Traits* traits = mParameterizedTraits;
	physx::PxI32* refCount = mRefCount;
	void* buf = mBuffer;

	this->~VertexBufferParameters();

	NxParameters::destroy(this, traits, doDeallocateSelf, refCount, buf);
}

const NxParameterized::DefinitionImpl* VertexBufferParameters::getParameterDefinitionTree(void)
{
	if (!mBuiltFlag) // Double-checked lock
	{
		NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);
		if (!mBuiltFlag)
		{
			buildTree();
		}
	}

	return(&ParamDefTable[0]);
}

const NxParameterized::DefinitionImpl* VertexBufferParameters::getParameterDefinitionTree(void) const
{
	VertexBufferParameters* tmpParam = const_cast<VertexBufferParameters*>(this);

	if (!mBuiltFlag) // Double-checked lock
	{
		NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);
		if (!mBuiltFlag)
		{
			tmpParam->buildTree();
		}
	}

	return(&ParamDefTable[0]);
}

NxParameterized::ErrorType VertexBufferParameters::getParameterHandle(const char* long_name, Handle& handle) const
{
	ErrorType Ret = NxParameters::getParameterHandle(long_name, handle);
	if (Ret != ERROR_NONE)
	{
		return(Ret);
	}

	size_t offset;
	void* ptr;

	getVarPtr(handle, ptr, offset);

	if (ptr == NULL)
	{
		return(ERROR_INDEX_OUT_OF_RANGE);
	}

	return(ERROR_NONE);
}

NxParameterized::ErrorType VertexBufferParameters::getParameterHandle(const char* long_name, Handle& handle)
{
	ErrorType Ret = NxParameters::getParameterHandle(long_name, handle);
	if (Ret != ERROR_NONE)
	{
		return(Ret);
	}

	size_t offset;
	void* ptr;

	getVarPtr(handle, ptr, offset);

	if (ptr == NULL)
	{
		return(ERROR_INDEX_OUT_OF_RANGE);
	}

	return(ERROR_NONE);
}

void VertexBufferParameters::getVarPtr(const Handle& handle, void*& ptr, size_t& offset) const
{
	ptr = getVarPtrHelper(&ParamLookupTable[0], const_cast<VertexBufferParameters::ParametersStruct*>(&parameters()), handle, offset);
}


/* Dynamic Handle Indices */
/* [0] - buffers (not an array of structs) */

void VertexBufferParameters::freeParameterDefinitionTable(NxParameterized::Traits* traits)
{
	if (!traits)
	{
		return;
	}

	if (!mBuiltFlag) // Double-checked lock
	{
		return;
	}

	NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);

	if (!mBuiltFlag)
	{
		return;
	}

	for (physx::PxU32 i = 0; i < NumParamDefs; ++i)
	{
		ParamDefTable[i].~DefinitionImpl();
	}

	traits->free(ParamDefTable);

	mBuiltFlag = false;
}

#define PDEF_PTR(index) (&ParamDefTable[index])

void VertexBufferParameters::buildTree(void)
{

	physx::PxU32 allocSize = sizeof(NxParameterized::DefinitionImpl) * NumParamDefs;
	ParamDefTable = (NxParameterized::DefinitionImpl*)(mParameterizedTraits->alloc(allocSize));
	memset(static_cast<void*>(ParamDefTable), 0, allocSize);

	for (physx::PxU32 i = 0; i < NumParamDefs; ++i)
	{
		NX_PARAM_PLACEMENT_NEW(ParamDefTable + i, NxParameterized::DefinitionImpl)(*mParameterizedTraits);
	}

	// Initialize DefinitionImpl node: nodeIndex=0, longName=""
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[0];
		ParamDef->init("", TYPE_STRUCT, "STRUCT", true);






	}

	// Initialize DefinitionImpl node: nodeIndex=1, longName="vertexCount"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[1];
		ParamDef->init("vertexCount", TYPE_U32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("longDescription", "The number of vertices in the verrtex buffer.  All vertex data channels (positions, normals, etc.) will contain this many items.", true);
		HintTable[1].init("shortDescription", "Number of vertices", true);
		ParamDefTable[1].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=2, longName="vertexFormat"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[2];
		ParamDef->init("vertexFormat", TYPE_REF, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		ParamDefTable[2].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#else

		static HintImpl HintTable[3];
		static Hint* HintPtrTable[3] = { &HintTable[0], &HintTable[1], &HintTable[2], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("longDescription", "This describes the data channels which exist per vertex, the vertex winding order, custom buffers, etc.  See VertexFormatParameters.", true);
		HintTable[2].init("shortDescription", "Attributes of the vertex buffer", true);
		ParamDefTable[2].setHints((const NxParameterized::Hint**)HintPtrTable, 3);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */


		static const char* const RefVariantVals[] = { "VertexFormatParameters" };
		ParamDefTable[2].setRefVariantVals((const char**)RefVariantVals, 1);



	}

	// Initialize DefinitionImpl node: nodeIndex=3, longName="buffers"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[3];
		ParamDef->init("buffers", TYPE_ARRAY, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("NOPVD", physx::PxU64(1), true);
		ParamDefTable[3].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#else

		static HintImpl HintTable[4];
		static Hint* HintPtrTable[4] = { &HintTable[0], &HintTable[1], &HintTable[2], &HintTable[3], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("NOPVD", physx::PxU64(1), true);
		HintTable[2].init("longDescription", "This is the buffer data according to the description in the VertexFormat", true);
		HintTable[3].init("shortDescription", "The data buffers for standard and custom semantics", true);
		ParamDefTable[3].setHints((const NxParameterized::Hint**)HintPtrTable, 4);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */


		static const char* const RefVariantVals[] = { "BufferU8x1", "BufferU8x2", "BufferU8x3", "BufferU8x4", "BufferU16x1", "BufferU16x2", "BufferU16x3", "BufferU16x4", "BufferU32x1", "BufferU32x2", "BufferU32x3", "BufferU32x4", "BufferF32x1", "BufferF32x2", "BufferF32x3", "BufferF32x4" };
		ParamDefTable[3].setRefVariantVals((const char**)RefVariantVals, 16);


		ParamDef->setArraySize(-1);
		static const physx::PxU8 dynHandleIndices[1] = { 0, };
		ParamDef->setDynamicHandleIndicesMap(dynHandleIndices, 1);

	}

	// Initialize DefinitionImpl node: nodeIndex=4, longName="buffers[]"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[4];
		ParamDef->init("buffers", TYPE_REF, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("NOPVD", physx::PxU64(1), true);
		ParamDefTable[4].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#else

		static HintImpl HintTable[4];
		static Hint* HintPtrTable[4] = { &HintTable[0], &HintTable[1], &HintTable[2], &HintTable[3], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("NOPVD", physx::PxU64(1), true);
		HintTable[2].init("longDescription", "This is the buffer data according to the description in the VertexFormat", true);
		HintTable[3].init("shortDescription", "The data buffers for standard and custom semantics", true);
		ParamDefTable[4].setHints((const NxParameterized::Hint**)HintPtrTable, 4);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */


		static const char* const RefVariantVals[] = { "BufferU8x1", "BufferU8x2", "BufferU8x3", "BufferU8x4", "BufferU16x1", "BufferU16x2", "BufferU16x3", "BufferU16x4", "BufferU32x1", "BufferU32x2", "BufferU32x3", "BufferU32x4", "BufferF32x1", "BufferF32x2", "BufferF32x3", "BufferF32x4" };
		ParamDefTable[4].setRefVariantVals((const char**)RefVariantVals, 16);



	}

	// SetChildren for: nodeIndex=0, longName=""
	{
		static Definition* Children[3];
		Children[0] = PDEF_PTR(1);
		Children[1] = PDEF_PTR(2);
		Children[2] = PDEF_PTR(3);

		ParamDefTable[0].setChildren(Children, 3);
	}

	// SetChildren for: nodeIndex=3, longName="buffers"
	{
		static Definition* Children[1];
		Children[0] = PDEF_PTR(4);

		ParamDefTable[3].setChildren(Children, 1);
	}

	mBuiltFlag = true;

}
void VertexBufferParameters::initStrings(void)
{
}

void VertexBufferParameters::initDynamicArrays(void)
{
	buffers.buf = NULL;
	buffers.isAllocated = true;
	buffers.elementSize = sizeof(NxParameterized::Interface*);
	buffers.arraySizes[0] = 0;
}

void VertexBufferParameters::initDefaults(void)
{

	freeStrings();
	freeReferences();
	freeDynamicArrays();
	vertexCount = physx::PxU32(0);

	initDynamicArrays();
	initStrings();
	initReferences();
}

void VertexBufferParameters::initReferences(void)
{
	vertexFormat = NULL;

}

void VertexBufferParameters::freeDynamicArrays(void)
{
	if (buffers.isAllocated && buffers.buf)
	{
		mParameterizedTraits->free(buffers.buf);
	}
}

void VertexBufferParameters::freeStrings(void)
{
}

void VertexBufferParameters::freeReferences(void)
{
	if (vertexFormat)
	{
		vertexFormat->destroy();
	}


	for (int i = 0; i < buffers.arraySizes[0]; ++i)
	{
		if (buffers.buf[i])
		{
			buffers.buf[i]->destroy();
		}
	}
}

} // namespace apex
} // namespace physx
