/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityImpl.h"
#include "coreDataEntityImplFactory.h"

using namespace Core;

DataEntityImpl::DataEntityImplMapType DataEntityImpl::m_DataEntityImplMap;

/**
*/
Core::DataEntityImpl::DataEntityImpl( ) 
{
	m_RealTime = 0;
	m_GenerateTemporalData = true;
}

bool Core::DataEntityImpl::IsConstantInTime() const
{
	return m_RealTime == CONSTANT_IN_TIME;
}


Core::RealTime Core::DataEntityImpl::GetRealTime() const
{
	return m_RealTime;
}

void Core::DataEntityImpl::SetRealTime( Core::RealTime val )
{
	m_RealTime = val;
}

bool Core::DataEntityImpl::DeepCopy( DataEntityImpl::Pointer input, ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	bool sucess = true;

	blTagMap::Pointer data = blTagMap::New();

	// Try without generating temporal data
	input->SetGenerateTemporalData( false );
	sucess = input->GetData( data );
	if ( sucess )
	{
		sucess &= SetData( data, mem );
	}

	// Try using temporal data
	if ( !sucess )
	{
		input->SetGenerateTemporalData( true );
		sucess = input->GetData( data );
		if ( sucess )
		{
			sucess &= SetData( data, mem );
		}
		input->CleanTemporalData( );
	}

	return sucess;
}

bool Core::DataEntityImpl::DeepCopy( boost::any val, ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	bool result = true;
	
	// Create a DataEntityImpl for this type
	DataEntityImplFactory::Pointer factory = DataEntityImplFactory::FindFactory( val.type() );
	DataEntityImpl::Pointer temporalDataEntity = factory->NewData();
	temporalDataEntity->SetDataPtr( val );
	if ( temporalDataEntity.IsNull() )
	{
		return false;
	}

	return DeepCopy( temporalDataEntity, mem );
}

DataEntityImpl::Pointer Core::DataEntityImpl::GetAt( size_t num )
{
	return this;
}

bool Core::DataEntityImpl::SetAt( 
	size_t pos, 
	DataEntityImpl::Pointer data,
	ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	return false;
}

size_t Core::DataEntityImpl::GetSize() const
{
	return 1;
}

void* Core::DataEntityImpl::GetVoidPtr() const
{
	return 0;
}

void Core::DataEntityImpl::SetVoidPtr( void* )
{

}

void Core::DataEntityImpl::SetSize( size_t size, DataEntityImpl::Pointer data ) const
{
}

bool Core::DataEntityImpl::SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem )
{
	return false;
}

bool Core::DataEntityImpl::GetData( blTagMap::Pointer tagMap )
{
	return false;
}

bool Core::DataEntityImpl::IsValidType( const std::type_info &type )
{
	return ( type == GetDataPtr().type() );
}

bool Core::DataEntityImpl::SetDataPtr( boost::any val )
{
	bool success = false;

	if ( val.type() == GetDataPtr().type() )
	{
		SetAnyData( val );
		success = true;
	}

	return success;
}

bool Core::DataEntityImpl::GetGenerateTemporalData() const
{
	return m_GenerateTemporalData;
}

void Core::DataEntityImpl::SetGenerateTemporalData( bool val )
{
	m_GenerateTemporalData = val;
}

void Core::DataEntityImpl::CleanTemporalData()
{
	
}
