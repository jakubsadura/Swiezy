/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiImageImpl.h"
#include "CmguiImageUtilities.h"
#include "CmguiPlaneFactory.h"

extern "C" {
#include "general/image_utilities.h"
}

CmguiPlugin::CmguiImageImpl::CmguiImageImpl( Cmiss_region *rootRegion, const std::string &name )
{
	m_Name = name;
	m_RootRegion = rootRegion;
	m_Data = NULL;
	ResetData();
}

CmguiPlugin::CmguiImageImpl::~CmguiImageImpl()
{
	if ( m_Data.IsNotNull() )
	{
		DestroyImage3D( m_Data );
	}
}

void CmguiPlugin::CmguiImageImpl::DestroyImage3D( CmguiImage3D::Pointer image )
{
	std::vector<CmguiImage2D::Pointer>::iterator it;
	for ( it = image->m_Slices.begin( ) ; it != image->m_Slices.end() ; it++ )
	{
		DestroyImage2D( *it );
	}

	Cmiss_region_remove_child_region( m_RootRegion, image->m_Region );
	Cmiss_region_destroy( &image->m_Region );
}

void CmguiPlugin::CmguiImageImpl::DestroyImage2D( CmguiImage2D::Pointer image )
{
	if ( image->m_CmguiImage )
	{
		DESTROY(Cmgui_image)( &(image->m_CmguiImage) );
		Cmiss_region_remove_child_region( m_Data->m_Region, image->m_Region );
		Cmiss_region_destroy( &image->m_Region );
	}
}

boost::any CmguiPlugin::CmguiImageImpl::GetDataPtr() const
{
	return m_Data;
}

void CmguiPlugin::CmguiImageImpl::ResetData()
{
	if ( m_Data.IsNotNull() )
	{
		DestroyImage3D( m_Data );
	}

	m_Data = CmguiImage3D::New();
	m_Data->m_Region = Cmiss_region_create();
	if ( Cmiss_region_add_child_region(m_RootRegion, m_Data->m_Region, m_Name.c_str(), 0) == 0 )
	{
		std::stringstream str;
		str << m_Name << " is not a valid name for a region or there's another error";
		throw Core::Exceptions::Exception( "CmguiImageImpl::ResetData", str.str().c_str() );
	}
}

void CmguiPlugin::CmguiImageImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

bool CmguiPlugin::CmguiImageImpl::SetData( 
	blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		DataType srcData;
		if ( tagDataPtr->GetValue<DataType>( srcData ) )
		{
			m_Data = srcData;
			return true;
		}
	}

	if ( SetDataFromSliceImage( tagMap, mem ) )
	{
		return true;
	}

	if ( SetDataFrom3DImage( tagMap, mem ) )
	{
		return true;
	}

	return false;
}

bool CmguiPlugin::CmguiImageImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataPtr", m_Data ); 
	return true;
}

bool CmguiPlugin::CmguiImageImpl::SetDataFromSliceImage( 
	blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tag;
	tag = tagMap->FindTagByName( "NumberOfSlices" );
	if ( tag.IsNull() ) return false;

	int numberOfSlices = tagMap->GetTagValue< int >( "NumberOfSlices" );
	for ( int i = 0 ; i < numberOfSlices ; i++ )
	{
		std::stringstream str;
		str << "Slice" << i;
		blTagMap::Pointer sliceTags = tagMap->GetTagValue<blTagMap::Pointer>( str.str() );
		if ( sliceTags.IsNull() )
		{
			return false;
		}

		std::stringstream str1;
		str1 << m_Name << i;
		std::string regionName = str1.str();
		CmguiImage2D::Pointer image2D = CmguiImage2D::New();

		tag = sliceTags->FindTagByName( "Dimensions" );
		if ( tag.IsNull() ) return false;
		std::vector<int> dimensions = tag->GetValueCasted< std::vector<int> >();

		tag = sliceTags->FindTagByName( "Position" );
		if ( tag.IsNull() ) return false;
		std::vector<float> position = tag->GetValueCasted< std::vector<float> >();

		tag = sliceTags->FindTagByName( "Spacing" );
		if ( tag.IsNull() ) return false;
		std::vector<double> spacing = tag->GetValueCasted< std::vector<double> >();

		tag = sliceTags->FindTagByName( "Orientation" );
		if ( tag.IsNull() ) return false;
		std::vector<float> orientation = tag->GetValueCasted< std::vector<float> >();

		typedef itk::Image<unsigned char,2> ImageType;
		ImageType::Pointer image = ConvertImageToUChar<ImageType>( sliceTags );
		if ( image.IsNull( ) )
		{
			return false;
		}
		void *scalarPtr = image->GetBufferPointer( );
		int typeSize = sizeof( unsigned char );

		// std::cout << position[ 0 ] << ", " << position[ 1 ] << ", " << position[2 ] << std::endl;
		// std::cout << orientation[ 0 ] << ", " << orientation[ 1 ] << ", " << orientation[2 ] << ", ";
		// std::cout << orientation[ 3 ] << ", " << orientation[ 4 ] << ", " << orientation[5 ] << std::endl;

		// Create Cmgui_image
		struct Cmgui_image *cmgui_image;
		cmgui_image = Cmgui_image_constitute( 
			dimensions[ 0 ], 
			dimensions[ 1 ], 
			1, 
			typeSize, 
			dimensions[ 0 ]*1*typeSize, 
			(unsigned char* ) scalarPtr );
		image2D->m_CmguiImage = cmgui_image;

		// Create region
		image2D->m_Region = Cmiss_region_create();
		Cmiss_region_add_child_region( m_Data->m_Region, image2D->m_Region, regionName.c_str(), 0 );

		PlaneFactory::Pointer planeFactory = PlaneFactory::New( );
		planeFactory->SetRegion( image2D->m_Region );
		PlaneFactory::CreatePointFrom2DImage( 0, position, spacing, dimensions, orientation, planeFactory->GetPoint( 0 ) );
		PlaneFactory::CreatePointFrom2DImage( 1, position, spacing, dimensions, orientation, planeFactory->GetPoint( 1 ) );
		PlaneFactory::CreatePointFrom2DImage( 2, position, spacing, dimensions, orientation, planeFactory->GetPoint( 2 ) );
		PlaneFactory::CreatePointFrom2DImage( 3, position, spacing, dimensions, orientation, planeFactory->GetPoint( 3 ) );
		planeFactory->Update();

		m_Data->m_Slices.push_back( image2D );
	}

	return true;
}

bool CmguiPlugin::CmguiImageImpl::SetDataFrom3DImage( blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem )
{

	blTag::Pointer tag;
	tag = tagMap->FindTagByName( "Origin" );
	if ( tag.IsNull() ) return false;
	std::vector<double> origin;
	origin = tag->GetValueCasted< std::vector<double> >();

	tag = tagMap->FindTagByName( "Spacing" );
	if ( tag.IsNull() ) return false;
	std::vector<double> spacing;
	spacing = tag->GetValueCasted< std::vector<double> >();

	tag = tagMap->FindTagByName( "Dimensions" );
	if ( tag.IsNull() ) return false;
	std::vector<int> dimensions;
	dimensions = tag->GetValueCasted< std::vector<int> >();

	typedef itk::Image<unsigned char,3> ImageType;
	ImageType::Pointer image = ConvertImageToUChar<ImageType>( tagMap );
	if ( image.IsNull() )
	{
		return false;
	}

	// std::cout << origin[ 0 ] << ", " << origin[ 1 ] << ", " << origin[2 ] << std::endl;

	int typeSize = sizeof( unsigned char );
	for ( int z = 0 ; z < dimensions[ 2 ] ; z++ )
	{
		std::stringstream str;
		str << m_Name << z;
		std::string regionName = str.str();
		CmguiImage2D::Pointer image2D = CmguiImage2D::New();

		void *scalarPtr = image->GetBufferPointer( ) + dimensions[ 0 ]*dimensions[ 1 ] * z;

		// Create Cmgui_image
		struct Cmgui_image *cmgui_image;
		cmgui_image = Cmgui_image_constitute( 
			dimensions[ 0 ], 
			dimensions[ 1 ], 
			1, 
			typeSize, 
			dimensions[ 0 ]*1*typeSize, 
			(unsigned char* ) scalarPtr );
		image2D->m_CmguiImage = cmgui_image;

		// Create region
		image2D->m_Region = Cmiss_region_create();
		Cmiss_region_add_child_region( m_Data->m_Region, image2D->m_Region, regionName.c_str(), 0 );

		PlaneFactory::Pointer planeFactory = PlaneFactory::New( );
		planeFactory->SetRegion( image2D->m_Region );
		PlaneFactory::CreatePointFrom3DImage( 0, origin, spacing, dimensions, z, planeFactory->GetPoint( 0 ) );
		PlaneFactory::CreatePointFrom3DImage( 1, origin, spacing, dimensions, z, planeFactory->GetPoint( 1 ) );
		PlaneFactory::CreatePointFrom3DImage( 2, origin, spacing, dimensions, z, planeFactory->GetPoint( 2 ) );
		PlaneFactory::CreatePointFrom3DImage( 3, origin, spacing, dimensions, z, planeFactory->GetPoint( 3 ) );
		planeFactory->Update();

		m_Data->m_Slices.push_back( image2D );
	}	

	return true;
}
