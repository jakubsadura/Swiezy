/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKTransformImpl.h"

using namespace Core;

ITKTransformImpl::ITKTransformImpl()
{
	ResetData();
}

ITKTransformImpl::~ITKTransformImpl()
{
}

boost::any Core::ITKTransformImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::ITKTransformImpl::ResetData()
{
	m_Data = NULL;
}

void Core::ITKTransformImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<TransformPointer> ( val );
}

bool Core::ITKTransformImpl::SetData(
	blTagMap::Pointer tagMap, 
	ImportMemoryManagementType mem /*= gmCopyMemory */ )
{
	blTag::Pointer tag;

	tag = tagMap->FindTagByName( "DataPtr" );
	if ( tag.IsNotNull() )
	{
		TransformPointer srcData;
		if ( tag->GetValue<TransformPointer>( srcData ) )
		{
			m_Data = srcData;
			return true;
		}
	}

	return false;
}

bool Core::ITKTransformImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataPtr", m_Data );

	return true;
}
