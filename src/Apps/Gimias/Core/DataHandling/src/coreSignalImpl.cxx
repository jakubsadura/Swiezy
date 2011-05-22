// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreSignalImpl.h"



Core::SignalImpl::SignalImpl( )
{
	ResetData();
}

Core::SignalImpl::~SignalImpl()
{

}

boost::any Core::SignalImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::SignalImpl::ResetData()
{
	m_Data = blSignalCollective::New( );
}

void Core::SignalImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

bool Core::SignalImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		DataType srcData;
		if ( tagDataPtr->GetValue<DataType>( srcData ) )
		{
			m_Data->Copy( srcData );
			return true;
		}
	}

	return false;
}

bool Core::SignalImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataPtr", m_Data ); return true;
}
