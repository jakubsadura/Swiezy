/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blITKImageUtils.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkOrientedImage.h"
#include "itkOrientImageFilter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkFlipImageFilter.h"

#include <blLogger.h>

/**
 *	Save image to file (move it somewhere else)
 */
// -------------------------------------------------------------------
template <class TInputImage>
void blITKImageUtils::SaveImageToFile( 
	const typename TInputImage::Pointer paramImage, 
	const char* paramFileName)
{
	typedef itk::ImageFileWriter< TInputImage > WriterType;
	typename WriterType::Pointer writer = WriterType::New();
	writer->SetInput (paramImage);
	writer->SetFileName(paramFileName);
	writer->Update();
}

template <class TInputImage>
typename TInputImage::Pointer blITKImageUtils::LoadImageFromFile( const char* paramFileName)
{
	typedef itk::ImageFileReader< TInputImage > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(paramFileName);
	reader->Update();
	return reader->GetOutput( );
}

template < typename ImageType > 
typename ImageType::Pointer blITKImageUtils::FlipItkImage(
 const ImageType* itkImage )
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("baselib"), "FlipItkImage");
	
	typename ImageType::Pointer img = NULL;
	typename itk::FlipImageFilter< ImageType >::Pointer flipper = itk::FlipImageFilter< ImageType >::New();
	flipper->SetInput (itkImage);

	typename itk::FlipImageFilter< ImageType >::FlipAxesArrayType axis;
	int dim = itkImage->GetImageDimension();
	if(dim == 2)
	{
	    LOG4CPLUS_INFO(blLogger::getInstance("baselib"), "Flipping Y axis of 2D image.");
		axis[0] = false;
		axis[1] = true;
	}
	if(dim == 3)
	{
	    LOG4CPLUS_INFO(blLogger::getInstance("baselib"), "Flipping X and Y axis of 3D image.");
		axis[0] = true;
		axis[1] = true;
		axis[2] = false;
	}
	flipper->SetFlipAxes (axis);
	flipper->Update();
	img = flipper->GetOutput();

	return img;
}

template < typename ImageType > 
typename ImageType::Pointer blITKImageUtils::ApplyOrientationToImage(
	typename ImageType::Pointer itkImage )
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("baselib"), "ApplyOrientationToImage");
	LOG4CPLUS_INFO(blLogger::getInstance("baselib"), "from: " << itkImage->GetDirection());
	LOG4CPLUS_INFO(blLogger::getInstance("baselib"), "to: ITK_COORDINATE_ORIENTATION_RAI (Standard)");
	
	typename itk::OrientImageFilter<ImageType, ImageType>::Pointer orienter;
	orienter = itk::OrientImageFilter<ImageType, ImageType>::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(itkImage);
	orienter->Update();
	return orienter->GetOutput();
}

template< typename ThreeDInputImageType, typename TwoDOutputImageType> 
typename TwoDOutputImageType::Pointer 
blITKImageUtils::ExtractSliceFromThreeDItkImage(
	typename ThreeDInputImageType::Pointer threeDInputImage,
	int sliceNr )
{
	typename TwoDOutputImageType::Pointer outputImage;

	typedef itk::ExtractImageFilter< ThreeDInputImageType, TwoDOutputImageType > FilterType;
	typename FilterType::Pointer filter = FilterType::New();
	filter->SetInput( threeDInputImage );
	
	//limit to 2D 
	typename ThreeDInputImageType::RegionType inputRegion = threeDInputImage->GetLargestPossibleRegion();
	typename ThreeDInputImageType::SizeType threeDSize = inputRegion.GetSize();
	typename ThreeDInputImageType::SizeType limitedSize = threeDSize;
	limitedSize[2] =0;

	//tell which 2D slice you want to retrieve
	typename ThreeDInputImageType::IndexType start = inputRegion.GetIndex();
	// slices should have id number from 1 to Number of Slices
	sliceNr = sliceNr - 1;
	
	if(sliceNr >= threeDSize[2])
		return NULL;

	//create extraction region
	start[2] = sliceNr;
	typename ThreeDInputImageType::RegionType desiredRegion;
	desiredRegion.SetSize(  limitedSize  );
	desiredRegion.SetIndex( start );
	//prepare filter
	filter->SetExtractionRegion( desiredRegion );
	filter->Update();
	outputImage = filter->GetOutput();

	return outputImage;
}

template< typename TInputImage >
bool blITKImageUtils::CompareImages( 
   const typename TInputImage::Pointer image1, 
   const typename TInputImage::Pointer image2,
   double tolerance, 
   double toleranceValue)
{
	if(toleranceValue < 0.)
	{
		toleranceValue = tolerance;
	}

	// Same pointer?
	if( image1.GetPointer() == image2.GetPointer() )
	{
		return true;
	}
   
    // Same number os dimensions: implied by template
    const size_t dimension = image1->GetImageDimension();
	if( dimension != image2->GetImageDimension() )
	{
		std::cerr << "Images have different dimension: "
		    << dimension << " != " << image2->GetImageDimension() << std::endl;
		return false;
	}
   
    // Same dimensions?
    const typename TInputImage::RegionType region1 = image1->GetLargestPossibleRegion();
    const typename TInputImage::RegionType region2 = image1->GetLargestPossibleRegion();
    for( size_t i = 0; i < dimension; ++i )
    {
        if( region1.GetSize()[ i ] != region2.GetSize()[ i ] )
        {
		    std::cerr << "Images have different size[" << i << "]: "
		        << region1.GetSize()[ i ] << " != " << region2.GetSize()[ i ] << std::endl;
            return false;
        }
    }

    // Same spacing?
    const typename TInputImage::SpacingType spacing1 = image1->GetSpacing();
    const typename TInputImage::SpacingType spacing2 = image2->GetSpacing();
    for( unsigned int  i = 0; i < dimension; ++i )
    {
        if( fabs( spacing1[ i ] - spacing2[ i ] ) > tolerance )
        {
		    std::cerr << "Images have different spacing[" << i << "]: "
		        << spacing1[ i ] << " != " << spacing2[ i ] << std::endl;
            return false;
        }
    }

    // Same data?
    typename TInputImage::IndexType index;
    double diff = 0;
    double diffTotal = 0;
    double diffMax = 0;
    double diffMin = std::numeric_limits<double>::max();
    double image1Max = -std::numeric_limits<double>::max();
    double image1Min = std::numeric_limits<double>::max();
    int count = 0;
    for( size_t k = 0; k < region1.GetSize()[2]; ++k )
    {
        index[2] = k;
        for( size_t j = 0; j < region1.GetSize()[1]; ++j )
        {
            index[1] = j;
            for( size_t i = 0; i < region1.GetSize()[0]; ++i )
            {
                index[0] = i;
                if( (*image1)[ index ] > image1Max ) image1Max = (*image1)[ index ];
                if( (*image1)[ index ] < image1Min ) image1Min = (*image1)[ index ];
                if( (*image1)[ index ] != (*image2)[ index ] )
                {
		            diff = fabs( (*image1)[ index ] - (*image2)[ index ] );
		            diffTotal += diff;
		            if( diff > diffMax ) diffMax = diff;
		            if( diff < diffMin ) diffMin = diff;
		            ++count;
                }
            }
        }
    }
    if( count != 0 )
    {
        const int total = region1.GetSize()[2] * region1.GetSize()[1] * region1.GetSize()[0];
        const double range = image1Max - image1Min;
        const double diffMean = diffTotal/count;
        std::cerr << "[WARNING] Images have different intensities: " 
            << "% pixels=" << count*100/total
            << ", mean=" << diffMean << "(" << diffMean*100/range << "% of range)"
            << ", max=" << diffMax << "(" << diffMax*100/range << "% of range)"
            << ", min=" << diffMin << "(" << diffMin*100/range << "% of range)"
            << " (range=" << range << ")" << std::endl;
        
        if( diffMean*100/range > toleranceValue ) return false;
    }

    return true;
}

