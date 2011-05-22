/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKImageDataImpl.h"

Core::VtkImageDataImpl::VtkImageDataImpl( ) 
{
	m_Data = vtkSmartPointer<vtkImageData>::New();
}

Core::VtkImageDataImpl::~VtkImageDataImpl()
{

}

boost::any Core::VtkImageDataImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::VtkImageDataImpl::SetDataPtr( boost::any val )
{
	bool result = false;

	if ( val.type() == typeid( vtkSmartPointer<vtkImageData> ) )
	{
		SetAnyData( val );
		result = true;
	}
	else if ( val.type() == typeid( vtkImageData* ) )
	{
		vtkImageData* processingDataCasted;
		processingDataCasted = boost::any_cast<vtkImageData*> ( val );
		SetAnyData( vtkImageDataPtr( processingDataCasted ) );
		result = true;
	}

	return result;
}

bool Core::VtkImageDataImpl::IsValidType( const std::type_info &type )
{
	return ( type == typeid( vtkSmartPointer<vtkImageData> ) ||
			 type == typeid( vtkImageData* ) );
}

void Core::VtkImageDataImpl::ResetData()
{
	m_Data = vtkSmartPointer<vtkImageData>::New();
}

bool Core::VtkImageDataImpl::SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem )
{
	blTag::Pointer tag;
	
	tag = tagMap->FindTagByName( "DataPtr" );
	if ( tag.IsNotNull() && mem == gmCopyMemory )
	{
		DataType srcData;
		if ( tag->GetValue<DataType>( srcData ) )
		{
			m_Data->DeepCopy( srcData );
			return true;
		}
	}

	std::vector<double> temp1;
	std::vector<int> temp2;

	tag = tagMap->FindTagByName( "Origin" );
	if ( tag.IsNull() ) return false;
	temp1 = tag->GetValueCasted< std::vector<double> >();
	m_Data->SetOrigin( &temp1[ 0 ] );

	tag = tagMap->FindTagByName( "Spacing" );
	if ( tag.IsNull() ) return false;
	temp1 = tag->GetValueCasted< std::vector<double> >();
	m_Data->SetSpacing( &temp1[ 0 ] );

	tag = tagMap->FindTagByName( "Dimensions" );
	if ( tag.IsNull() ) return false;
	temp2 = tag->GetValueCasted< std::vector<int> >();
	m_Data->SetDimensions( &temp2[ 0 ] );

	tag = tagMap->FindTagByName( "ScalarPointer" );
	if ( tag.IsNull() ) return false;
	void *scalarPointer = tag->GetValueCasted<void*>();

	tag = tagMap->FindTagByName( "ScalarSizeInBytes" );
	if ( tag.IsNull() ) return false;
	size_t sizeInBytes = tag->GetValueCasted<size_t>();

	tag = tagMap->FindTagByName( "NumberOfComponents" );
	if ( tag.IsNull() ) return false;
	int numComponents = tag->GetValueCasted<int>();

	tag = tagMap->FindTagByName( "ScalarType" );
	if ( tag.IsNull() ) return false;
	std::string scalarType = tag->GetValueCasted<std::string>();
	if (scalarType == "float" )
	{
		m_Data->SetScalarTypeToFloat();
	}
	else if (scalarType == "double" )
	{
		m_Data->SetScalarTypeToDouble();
	}
	else if (scalarType == "int" )
	{
		m_Data->SetScalarTypeToInt();
	}
	else if (scalarType == "unsigned int" )
	{
		m_Data->SetScalarTypeToUnsignedInt();
	}
	else if (scalarType == "long" )
	{
		m_Data->SetScalarTypeToLong();
	}
	else if (scalarType == "unsigned long" )
	{
		m_Data->SetScalarTypeToUnsignedLong();
	}
	else if (scalarType == "short" )
	{
		m_Data->SetScalarTypeToShort();
	}
	else if (scalarType == "unsigned short" )
	{
		m_Data->SetScalarTypeToUnsignedShort();
	}
	else if (scalarType == "unsigned char" )
	{
		m_Data->SetScalarTypeToUnsignedChar();
	}
	else if (scalarType == "signed char" )
	{
		m_Data->SetScalarTypeToSignedChar();
	}
	else if (scalarType == "char" )
	{
		m_Data->SetScalarTypeToChar();
	}

	switch( mem )
	{
	case gmCopyMemory:
		m_Data->AllocateScalars();
		memcpy(m_Data->GetScalarPointer(), scalarPointer, sizeInBytes );
		break;
	case gmManageMemory:
		break;
	case gmReferenceMemory:
		vtkDataArray *scalars;
		scalars = vtkDataArray::CreateDataArray( m_Data->GetScalarType( ) );
		scalars->SetName( "scalars" );
		scalars->SetNumberOfComponents( numComponents );
		size_t size = sizeInBytes/scalars->GetDataTypeSize();
		scalars->SetVoidArray( scalarPointer, size, 1 );
		m_Data->GetPointData( )->SetScalars( scalars );
		break;
	}

	// Xavi: I don't know why do we need this
	m_Data->SetNumberOfScalarComponents( numComponents );

	return true;
}

bool Core::VtkImageDataImpl::GetData( blTagMap::Pointer tagMap )
{
	std::vector<double> temp1;
	std::vector<int> temp2;

	temp1.assign( &m_Data->GetOrigin()[ 0 ], &m_Data->GetOrigin()[ 0 ] + 3 );
	tagMap->AddTag( "Origin", temp1 );
	temp1.assign( &m_Data->GetSpacing()[ 0 ], &m_Data->GetSpacing()[ 0 ] + 3 );
	tagMap->AddTag( "Spacing", temp1 );
	temp2.assign( &m_Data->GetDimensions()[ 0 ], &m_Data->GetDimensions()[ 0 ] + 3 );
	tagMap->AddTag( "Dimensions", temp2 );
	tagMap->AddTag( "ScalarType", std::string( m_Data->GetScalarTypeAsString() ) );
	tagMap->AddTag( "ScalarPointer", m_Data->GetScalarPointer() );

	vtkDataArray* scalars = m_Data->GetPointData()->GetScalars();
	size_t sizeInBytes = scalars->GetNumberOfComponents() *
		scalars->GetNumberOfTuples() * scalars->GetDataTypeSize();
	tagMap->AddTag( "ScalarSizeInBytes", sizeInBytes );
	tagMap->AddTag( "NumberOfComponents", scalars->GetNumberOfComponents( ) );

	tagMap->AddTag( "DataPtr", m_Data );

	return true;
}

void* Core::VtkImageDataImpl::GetVoidPtr() const
{
	void* ptr; ptr = m_Data; return ptr;
}

void Core::VtkImageDataImpl::SetVoidPtr( void* ptr )
{
	m_Data = reinterpret_cast<vtkImageData*> ( ptr );
}

void Core::VtkImageDataImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}
