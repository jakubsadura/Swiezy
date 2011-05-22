/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMITKSurfaceImpl.h"
#include "coreVTKPolyDataImpl.h"

using namespace Core;

/**
*/
Core::MitkSurfaceImpl::MitkSurfaceImpl( ) 
{
	ResetData( );
}

Core::MitkSurfaceImpl::~MitkSurfaceImpl()
{

}

boost::any Core::MitkSurfaceImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::MitkSurfaceImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataEntitySetImplSet = tagMap->FindTagByName( "DataEntityImplSet" );
	if ( tagDataEntitySetImplSet.IsNull() )
	{
		return false;
	}

	DataEntityImplSetType setOfDataEntitySetImpl;
	setOfDataEntitySetImpl = tagDataEntitySetImplSet->GetValueCasted<DataEntityImplSetType>();

	ResetData( );

	DataEntityImplSetType::iterator it;
	int time = 0;
	for ( it = setOfDataEntitySetImpl.begin() ; it != setOfDataEntitySetImpl.end() ; it++)
	{
		// Create a new VtkPolyDataImpl
		DataEntityImplFactory::Pointer factory;
		factory = DataEntityImplFactory::FindFactory( "vtkPolyData" );
		DataEntityImpl::Pointer dataEntityImpl = factory->NewData();

		// Copy data
		dataEntityImpl->DeepCopy( *it, mem );

		// Set data
		SetAt( time, dataEntityImpl.GetPointer(), mem );

		time++;
	}

	return true;
}

bool Core::MitkSurfaceImpl::GetData( blTagMap::Pointer tagMap )
{

	DataEntityImplSetType dataEntityImplSet;
	for ( int i = 0 ; i < m_Data->GetSizeOfPolyDataSeries() ; i++)
	{
		dataEntityImplSet.push_back( GetAt( i ) );
	}

	tagMap->AddTag( "DataEntityImplSet", dataEntityImplSet );

	return true;
}

void Core::MitkSurfaceImpl::ResetData()
{
	m_Data = mitk::Surface::New();
}

bool Core::MitkSurfaceImpl::SwitchImplementation( const std::type_info &type )
{
	return false;
}

size_t Core::MitkSurfaceImpl::GetSize() const
{
	return m_Data->GetSizeOfPolyDataSeries();
}

DataEntityImpl::Pointer Core::MitkSurfaceImpl::GetAt( size_t pos )
{
	vtkPolyDataPtr polyData = m_Data->GetVtkPolyData( pos );
	VtkPolyDataImpl::Pointer vtkPolyDataImpl = VtkPolyDataImpl::New( );
	vtkPolyDataImpl->SetDataPtr( polyData );
	return vtkPolyDataImpl.GetPointer();
}


bool Core::MitkSurfaceImpl::SetAt( 
	size_t pos, 
	DataEntityImpl::Pointer data,
	ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	if ( data->GetDataPtr().type() == typeid( vtkPolyDataPtr ) )
	{
		vtkPolyDataPtr polyData;
		polyData = boost::any_cast<vtkPolyDataPtr>( data->GetDataPtr() );
		m_Data->SetVtkPolyData( polyData, pos );
		return true;
	}
	return false;
}

void* Core::MitkSurfaceImpl::GetVoidPtr() const
{
	void* ptr; ptr = m_Data; return ptr;
}

void Core::MitkSurfaceImpl::SetVoidPtr( void* ptr )
{
	m_Data = reinterpret_cast<mitk::Surface*> ( ptr );
}

void Core::MitkSurfaceImpl::SetSize( size_t size, DataEntityImpl::Pointer data ) const
{
	m_Data->Expand( size );
}

void Core::MitkSurfaceImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<mitk::Surface::Pointer> ( val );
}
