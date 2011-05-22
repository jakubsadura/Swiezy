/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKUnstructuredGridImpl.h"

Core::VtkUnstructuredGridImpl::VtkUnstructuredGridImpl( ) 
{
	ResetData();
}

Core::VtkUnstructuredGridImpl::~VtkUnstructuredGridImpl()
{

}

boost::any Core::VtkUnstructuredGridImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::VtkUnstructuredGridImpl::SetDataPtr( boost::any val )
{
	bool result = false;

	if ( val.type() == typeid( DataType ) )
	{
		SetAnyData( val );
		result = true;
	}
	else if ( val.type() == typeid( vtkUnstructuredGrid* ) )
	{
		vtkUnstructuredGrid* processingDataCasted;
		processingDataCasted = boost::any_cast<vtkUnstructuredGrid*> ( val );
		SetAnyData( DataType( processingDataCasted ) );
		result = true;
	}

	return result;
}

bool Core::VtkUnstructuredGridImpl::IsValidType( const std::type_info &type )
{
	return ( type == typeid( DataType ) ||
			 type == typeid( vtkUnstructuredGrid* ) );
}

void Core::VtkUnstructuredGridImpl::ResetData()
{
	SetAnyData( DataType::New() );
}

void* Core::VtkUnstructuredGridImpl::GetVoidPtr() const
{
	void* ptr; ptr = m_Data; return ptr;
}

void Core::VtkUnstructuredGridImpl::SetVoidPtr( void* ptr )
{
	SetAnyData( reinterpret_cast<vtkUnstructuredGrid*> ( ptr ) );
}

void Core::VtkUnstructuredGridImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

bool Core::VtkUnstructuredGridImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		DataType srcData;
		if ( tagDataPtr->GetValue<DataType>( srcData ) )
		{
			m_Data->DeepCopy( srcData );
			return true;
		}
	}

	return false;
}

bool Core::VtkUnstructuredGridImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataPtr", m_Data ); return true;
}
