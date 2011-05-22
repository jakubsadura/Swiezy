/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMITKImageImpl.h"
#include "coreVTKImageDataImpl.h"

using namespace Core;

/**
*/
Core::MitkImageImpl::MitkImageImpl( ) 
{
	ResetData( );
}

Core::MitkImageImpl::~MitkImageImpl()
{

}

boost::any Core::MitkImageImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::MitkImageImpl::SetData( 
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
	if ( setOfDataEntitySetImpl.size() )
	{
		SetSize( setOfDataEntitySetImpl.size(), *setOfDataEntitySetImpl.begin() );
	}

	DataEntityImplSetType::iterator it;
	int time = 0;
	for ( it = setOfDataEntitySetImpl.begin() ; it != setOfDataEntitySetImpl.end() ; it++)
	{
		// Create a new VtkPolyDataImpl
		DataEntityImplFactory::Pointer factory;
		factory = DataEntityImplFactory::FindFactory( "vtkImageData" );
		DataEntityImpl::Pointer dataEntityImpl = factory->NewData();

		// Copy data
		if ( !dataEntityImpl->DeepCopy( *it, mem ) )
		{
			return false;
		}

		// Set data
		SetAt( time, dataEntityImpl.GetPointer(), mem );

		time++;
	}

	return true;
}

bool Core::MitkImageImpl::GetData( blTagMap::Pointer tagMap )
{

	DataEntityImplSetType dataEntityImplSet;
	int i = 0;
	while ( m_Data->IsVolumeSet( i ))
	{
		dataEntityImplSet.push_back( GetAt( i ) );
		i++;
	}

	tagMap->AddTag( "DataEntityImplSet", dataEntityImplSet );

	return true;
}

void Core::MitkImageImpl::ResetData()
{
	if ( m_Data.IsNull() || GetSize() > 0 )
	{
		m_Data = mitk::Image::New();
	}
	m_Origin[ 0 ] = 0;
	m_Origin[ 1 ] = 0;
	m_Origin[ 2 ] = 0;
}

bool Core::MitkImageImpl::SwitchImplementation( const std::type_info &type )
{
	return false;
}

size_t Core::MitkImageImpl::GetSize() const
{
	int i = 0;
	while ( m_Data->IsVolumeSet( i ))
	{
		i++;
	}
	return i;
}

DataEntityImpl::Pointer Core::MitkImageImpl::GetAt( size_t pos )
{
	vtkImageDataPtr data = m_Data->GetVtkImageData( pos );
	data->SetOrigin( m_Origin[0], m_Origin[1], m_Origin[2] );
	VtkImageDataImpl::Pointer dataImpl = VtkImageDataImpl::New( );
	dataImpl->SetDataPtr( data );
	return dataImpl.GetPointer();
}


bool Core::MitkImageImpl::SetAt( 
	size_t pos, 
	DataEntityImpl::Pointer data,
	ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	if ( data->GetDataPtr().type() == typeid( vtkImageDataPtr ) )
	{
		vtkImageDataPtr timeStepData;
		timeStepData = boost::any_cast<vtkImageDataPtr>( data->GetDataPtr() );

		switch( mem )
		{
		case gmCopyMemory:
			m_Data->SetImportVolume( 
				timeStepData->GetScalarPointer(), pos, 0, mitk::Image::CopyMemory );
			break;
		case gmManageMemory:
			m_Data->SetImportVolume( 
				timeStepData->GetScalarPointer(), pos, 0, mitk::Image::ManageMemory );
			break;
		case gmReferenceMemory:
			m_Data->SetImportVolume( 
				timeStepData->GetScalarPointer(), pos, 0, mitk::Image::ReferenceMemory );
			break;
		}

		m_Origin[0] = mitk::Point3D::ValueType( timeStepData->GetOrigin()[0] );
		m_Origin[1] = mitk::Point3D::ValueType( timeStepData->GetOrigin()[1] );
		m_Origin[2] = mitk::Point3D::ValueType( timeStepData->GetOrigin()[2] );
		m_Data->GetGeometry( pos )->SetOrigin( m_Origin );

		return true;
	}

	return false;
}

void* Core::MitkImageImpl::GetVoidPtr() const
{
	void* ptr; ptr = m_Data; return ptr;
}

void Core::MitkImageImpl::SetVoidPtr( void* ptr )
{
	m_Data = reinterpret_cast<mitk::Image*> ( ptr );
}

void Core::MitkImageImpl::SetSize( size_t size, DataEntityImpl::Pointer data ) const
{
	// Create a new VtkPolyDataImpl
	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( "vtkImageData" );
	DataEntityImpl::Pointer dataEntityImpl = factory->NewData();

	// Reference data
	dataEntityImpl->DeepCopy( data, gmReferenceMemory );

	vtkImageDataPtr timeStepData;
	timeStepData = boost::any_cast<vtkImageDataPtr>( dataEntityImpl->GetDataPtr() );
	m_Data->Initialize( timeStepData, 1, size );
}

void Core::MitkImageImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<mitk::Image::Pointer> ( val );
}
