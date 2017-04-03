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

#ifndef PX_SNIPPET_EXTENSION_METADATAOBJECTS_H
#define PX_SNIPPET_EXTENSION_METADATAOBJECTS_H
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "PxMetaDataObjects.h"
#include "PxExtensionMetaDataObjects.h"

/** \addtogroup physics
@{
*/

namespace physx
{

struct PxSnippetExtensionPropertyInfoName
{
	enum Enum
	{
		Unnamed = PxExtensionsPropertyInfoName::LastPxPropertyInfoName,
#include "PxSnippetExtensionAutoGeneratedMetaDataObjectNames.h"
		LastPxPropertyInfoName,
	};
};

#define DEFINE_PROPERTY_TO_VALUE_STRUCT_MAP( type, prop, valueStruct )																	\
	template<> struct PxPropertyToValueStructMemberMap< PxSnippetExtensionPropertyInfoName::type##_##prop >									\
	{																																	\
		PxU32 Offset;																													\
		PxPropertyToValueStructMemberMap< PxSnippetExtensionPropertyInfoName::type##_##prop >() : Offset( offsetof( valueStruct, prop ) ) {}	\
		template<typename TOperator> void visitProp( TOperator inOperator, valueStruct& inStruct ) { inOperator( inStruct.prop );	}	\
	};

#ifdef PX_VC
#pragma warning(push)
#pragma warning(disable: 4100)
#endif

#include "PxSnippetExtensionAutoGeneratedMetaDataObjects.h"

#ifdef PX_VC
#pragma warning(pop)
#endif

#undef DEFINE_PROPERTY_TO_VALUE_STRUCT_MAP

}

/** @} */
#endif
