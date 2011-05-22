// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreNumericImpl.h"



Core::NumericImpl::NumericImpl( )
{
	ResetData( );
}

Core::NumericImpl::~NumericImpl()
{

}

void Core::NumericImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

boost::any Core::NumericImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::NumericImpl::ResetData()
{
	m_Data = blTagMap::New( );
}

bool Core::NumericImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		DataType srcData;
		if ( tagDataPtr->GetValue<DataType>( srcData ) )
		{
			m_Data->RemoveAll( );
			m_Data->AddTags( srcData );
			return true;
		}
	}

	return false;
}

bool Core::NumericImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataPtr", m_Data ); 
	return true;
}
