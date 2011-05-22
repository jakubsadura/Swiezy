/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKImageImpl.h"
#include <typeinfo>

using namespace Core;

#define SetComponentType( type1, IOComponent ) \
	if ( typeid( typename ImageType::PixelType ) == typeid( type1 ) ) \
	{ \
		m_ComponentType = IOComponent; \
	}

// Create an itk image sharing the input buffer and cast to the internal type
#define CastData( name, PixType, IDimension ) \
	if (scalarType == name ) \
	{ \
		typedef itk::Image<PixType,IDimension> InputImageType; \
		typename InputImageType::Pointer inputImage = InputImageType::New( ); \
		inputImage->SetOrigin( &origin[ 0 ] ); \
		inputImage->SetSpacing( &spacing[ 0 ] ); \
		inputImage->SetRegions( region ); \
		inputImage->GetPixelContainer( )->SetImportPointer(  \
			(typename InputImageType::PixelType*) (scalarPointer),  \
			region.GetNumberOfPixels( ) ); \
		typedef itk::CastImageFilter< InputImageType, ImageType > FilterType; \
		typename FilterType::Pointer filter = FilterType::New(); \
		filter->SetInput( inputImage ); \
		filter->Update( ); \
		m_Data = filter->GetOutput( ); \
	}

template <class ImageType>
ITKImageImpl<ImageType>::ITKImageImpl()
{
	ResetData();

	SetComponentType( char, "char" )
	else SetComponentType( unsigned char, "unsigned char" )
	else SetComponentType( int, "int" )
	else SetComponentType( unsigned int, "unsigned int" )
	else SetComponentType( short, "short" )
	else SetComponentType( unsigned short, "unsigned short" )
	else SetComponentType( float, "float" )
	else SetComponentType( double, "double" )
	else SetComponentType( long, "long" )
	else SetComponentType( unsigned long, "unsigned long" )
}

template <class ImageType>
ITKImageImpl<ImageType>::~ITKImageImpl()
{
}

template <class ImageType>
boost::any Core::ITKImageImpl<ImageType>::GetDataPtr() const
{
	return m_Data;
}

template <class ImageType>
void Core::ITKImageImpl<ImageType>::ResetData()
{
	m_Data = ImageType::New();
}

template <class ImageType>
void Core::ITKImageImpl<ImageType>::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<typename ImageType::Pointer> ( val );
}

template <class ImageType>
bool Core::ITKImageImpl<ImageType>::IsValidType( const std::type_info &type )
{
	return ( type == typeid( typename ImageType::Pointer ) ||
			 type == typeid( ImageType* ) );
}

template <class ImageType>
bool Core::ITKImageImpl<ImageType>::SetDataPtr( boost::any val )
{
	bool success = false;

	if ( val.type() == typeid( typename ImageType::Pointer ) )
	{
		m_Data = boost::any_cast<typename ImageType::Pointer> ( val );
		success = true;
	}
	else if ( val.type() == typeid( ImageType* ) )
	{
		m_Data = boost::any_cast<ImageType*> ( val );
		success = true;
	}

	return success;
}

template <class ImageType>
bool Core::ITKImageImpl<ImageType>::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tag;

	ResetData();

	tag = tagMap->FindTagByName( "Origin" );
	if ( tag.IsNull() ) return false;
	std::vector<double> origin = tag->GetValueCasted< std::vector<double> >();

	tag = tagMap->FindTagByName( "Spacing" );
	if ( tag.IsNull() ) return false;
	std::vector<double> spacing = tag->GetValueCasted< std::vector<double> >();

	tag = tagMap->FindTagByName( "Dimensions" );
	if ( tag.IsNull() ) return false;
	std::vector<int> dimensions = tag->GetValueCasted< std::vector<int> >();
	typename ImageType::RegionType region;
	for ( int i = 0 ; i < ImageType::ImageDimension ; i++ )
	{
		region.SetSize( i, dimensions[ i ] );
	}

	tag = tagMap->FindTagByName( "ScalarPointer" );
	if ( tag.IsNull() ) return false;
	void *scalarPointer = tag->GetValueCasted<void*>();

	tag = tagMap->FindTagByName( "ScalarType" );
	if ( tag.IsNull() ) return false;
	std::string scalarType = tag->GetValueCasted<std::string>();

	tag = tagMap->FindTagByName( "NumberOfComponents" );
	if ( tag.IsNull() ) return false;
	int numComponents = tag->GetValueCasted<int>();
	if ( numComponents != 1 )
	{
		return false;
	}

	CastData( "char", char, ImageType::ImageDimension )
	else CastData( "unsigned char", unsigned char, ImageType::ImageDimension )
	else CastData( "int", int, ImageType::ImageDimension )
	else CastData( "unsigned int", unsigned int, ImageType::ImageDimension )
	else CastData( "short", short, ImageType::ImageDimension )
	else CastData( "unsigned short", unsigned short, ImageType::ImageDimension )
	else CastData( "float", float, ImageType::ImageDimension )
	else CastData( "double", double, ImageType::ImageDimension )
	else CastData( "long", long, ImageType::ImageDimension )
	else CastData( "unsigned long", unsigned long, ImageType::ImageDimension )

	return true;
}

template <class ImageType>
bool Core::ITKImageImpl<ImageType>::GetData( blTagMap::Pointer tagMap )
{
	std::vector<double> temp1;
	std::vector<int> temp2;

	const typename ImageType::PointType & itkOrigin = m_Data->GetOrigin();
	temp1.assign( &itkOrigin[ 0 ], &itkOrigin[ 0 ] + ImageType::ImageDimension );
	tagMap->AddTag( "Origin", temp1 );

	const typename ImageType::SpacingType & itkspacing = m_Data->GetSpacing();
	temp1.assign( &itkspacing[ 0 ], &itkspacing[ 0 ] + ImageType::ImageDimension );
	tagMap->AddTag( "Spacing", temp1 );

	temp2.resize( ImageType::ImageDimension );
	for ( int i = 0 ; i < ImageType::ImageDimension ; i++ )
	{
		temp2[ i ] = m_Data->GetLargestPossibleRegion().GetSize().GetSize()[i];
	}
	tagMap->AddTag( "Dimensions", temp2 );

	tagMap->AddTag( "ScalarType", m_ComponentType );

	tagMap->AddTag( "ScalarPointer", (void*) m_Data->GetBufferPointer() );

	size_t sizeInBytes = m_Data->GetPixelContainer( )->Size( ) * sizeof( typename ImageType::PixelType );
	tagMap->AddTag( "ScalarSizeInBytes", sizeInBytes );

	int numComponents = 1;
	const std::type_info &pixelTypeID = typeid( typename ImageType::PixelType );
	std::string name = pixelTypeID.name( );
	if ( pixelTypeID == typeid( char ) ||
		 pixelTypeID == typeid( unsigned char ) ||
		 pixelTypeID == typeid( int ) ||
		 pixelTypeID == typeid( unsigned int ) ||
		 pixelTypeID == typeid( short ) ||
		 pixelTypeID == typeid( unsigned short ) ||
		 pixelTypeID == typeid( float ) ||
		 pixelTypeID == typeid( double ) ||
		 pixelTypeID == typeid( long ) ||
		 pixelTypeID == typeid( unsigned long ) )
	{
		numComponents = 1;
	}
	else 
	{
		return false;
	}
	tagMap->AddTag( "NumberOfComponents", numComponents );

	return true;
}

