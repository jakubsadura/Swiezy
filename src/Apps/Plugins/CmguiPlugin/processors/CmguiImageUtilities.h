/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiImageUtilities_H
#define _CmguiImageUtilities_H

#include "itkImage.h"
#include "blTagMap.h"
#include "itkRescaleIntensityImageFilter.h"
#include "coreDataEntityImplFactory.h"


template<class InputImageType, class OutputImageType>
typename OutputImageType::Pointer ConvertImageToUCharTemplated( blTagMap::Pointer tagMap )
{
	typename InputImageType::Pointer inImage;
	typename OutputImageType::Pointer outImage;

	Core::DataEntityImplFactory::Pointer factory;
	factory = Core::DataEntityImplFactory::FindFactory( typeid(inImage) );
	if ( factory.IsNull() )
	{
		return NULL;
	}

	Core::DataEntityImpl::Pointer newData = factory->NewData();
	newData->SetData( tagMap, Core::gmReferenceMemory );
	Core::CastAnyProcessingData( newData->GetDataPtr(), inImage );

	typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>   FilterType;
	typename FilterType::Pointer scaler = FilterType::New(); 
	scaler->SetInput( inImage ); 
	scaler->SetOutputMaximum( 255 );
	scaler->SetOutputMinimum( 0 ); 
	scaler->Update( );
	outImage = scaler->GetOutput( );
	return outImage;
};

/** Convert input data to ImageType otherwise
the CMGUI cannot render it
*/
template <class ImageType>
typename ImageType::Pointer ConvertImageToUChar( blTagMap::Pointer tagMap )
{
	blTag::Pointer tag;
	tag = tagMap->FindTagByName( "ScalarType" );
	if ( tag.IsNull() ) return NULL;
	std::string scalarType = tag->GetValueCasted<std::string>();
	typedef ImageType OutputImageType;

	if (scalarType == "float" )
	{
		typedef itk::Image<float,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "double" )
	{
		typedef itk::Image<double,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "int" )
	{
		typedef itk::Image<int,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "unsigned int" )
	{
		typedef itk::Image<unsigned int,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "long" )
	{
		typedef itk::Image<long,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "unsigned long" )
	{
		typedef itk::Image<unsigned long,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "short" )
	{
		typedef itk::Image<short,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "unsigned short" )
	{
		typedef itk::Image<unsigned short,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "unsigned char" )
	{
		typedef itk::Image<unsigned char,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "signed char" )
	{
		typedef itk::Image<signed char,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}
	else if (scalarType == "char" )
	{
		typedef itk::Image<char,ImageType::ImageDimension> InputImageType;
		return ConvertImageToUCharTemplated<InputImageType,OutputImageType>( tagMap );
	}

	return NULL;
}

#endif // _CmguiImageUtilities_H
