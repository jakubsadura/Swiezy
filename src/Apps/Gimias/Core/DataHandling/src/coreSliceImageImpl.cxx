/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSliceImageImpl.h"
#include "coreITKImageImpl.h"

Core::SliceImageImpl::SliceImageImpl( )
{
	ResetData();
}

Core::SliceImageImpl::~SliceImageImpl()
{

}

boost::any Core::SliceImageImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::SliceImageImpl::ResetData()
{
	m_Data = blSliceImage::New( );
}

void Core::SliceImageImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

bool Core::SliceImageImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem /*= gmCopyMemory */ )
{
	return false;
}

#define AddSliceTags( sliceTags, sliceImage, index, typeAsString, pixelType ) \
	if ( m_Data->GetBaseSlice( 0 ).pixelTypeAsString == typeAsString ) \
	{ \
		typedef itk::Image<pixelType,2> ImageType; \
		Core::ITKImageImpl<ImageType>::Pointer impl = Core::ITKImageImpl<ImageType>::New( ); \
		impl->SetDataPtr( sliceImage->GetSliceImage<pixelType>( index ).image ); \
		impl->GetData( sliceTags ); \
		sliceTags->AddTag( "ScalarType", std::string( typeAsString ) ); \
	}

bool Core::SliceImageImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "NumberOfSlices", m_Data->GetNumberOfSlices() );
	for ( int i = 0 ; i < m_Data->GetNumberOfSlices() ; i++ )
	{
		blTagMap::Pointer sliceTags = blTagMap::New( );
		sliceTags->AddTag( "Position", m_Data->GetBaseSlice( i ).position );
		sliceTags->AddTag( "Orientation", m_Data->GetBaseSlice( i ).orientation );

		AddSliceTags( sliceTags, m_Data, i, "double", double );

		std::stringstream str;
		str << "Slice" << i;
		tagMap->AddTag( str.str(), sliceTags );
	}

	return true;
}
