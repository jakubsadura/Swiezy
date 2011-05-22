/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmImageUtilities_txx
#define _dcmImageUtilities_txx

#include <boost/algorithm/string/trim.hpp>

#include <blLogger.h>

#include "blITKImagesDefines.h"
#include "blITKImageUtils.h"
#include "itkImageSeriesReader.h"
#include "blImageUtils.h"

#include <itkResampleImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkCubicRootImageFilter.h>

#include <gdcmFile.h>
#include <gdcmValEntry.h>
#include "dcmTypes.h"

namespace dcmAPI
{

template< typename ImageType > void ImageUtilities::AssureCorrectSpacing
(
 typename ImageType::Pointer itkImage
 )
{
	log4cplus::Logger logger = blLogger::getInstance("dcmapi.ImageUtilities");
	LOG4CPLUS_TRACE_METHOD(logger, "AssureCorrectSpacing");

	typename ImageType::SpacingType s = itkImage->GetSpacing();
	if(s[2] < 0)
	{
        LOG4CPLUS_WARN(logger, "Z spacing is negative, making it positive.");
		s[2] = fabs(s[2]);
		itkImage->SetSpacing(s);
		
		typename ImageType::DirectionType d = itkImage->GetDirection();
		if(d[0][0]==1 && d[0][1]==0 && d[0][2]==0 && d[1][0]==0 && d[1][1]==1 && d[1][2]==0) //axial
		{
	        LOG4CPLUS_WARN(logger, "Compensating with a negative z direction.");
			d[2][0] = 0; 
			d[2][1] = 0;	
			d[2][2] = -1;
			itkImage->SetDirection(d);
		}
		//
		//else throw exception "unsupported data type"

		//	d[1][1] = -1;//d[0][0] = -1;
		//else if(d[0][0]==-1 && d[0][1]==0 && d[0][2]==0 && d[1][0]==0 && d[1][1]==1 && d[1][2]==0) //axial opposite
		//	d[0][0] = 1;
		//else if(d[0][0]==1 && d[0][1]==0 && d[0][2]==0 && d[1][0]==0 && d[1][1]==0 && d[1][2]==-1) //coronal 
		//	d[0][0] = -1;
		//else if(d[0][0]==-1 && d[0][1]==0 && d[0][2]==0 && d[1][0]==0 && d[1][1]==0 && d[1][2]==-1) //coronal opposite
		//	d[0][0] = 1;
		//else if(d[0][0]==0 && d[0][1]==0 && d[0][2]==1 && d[1][0]==0 && d[1][1]==1 && d[1][2]==0) //sagital
		//	d[0][2] = -1;
		//else if(d[0][0]==0 && d[0][1]==0 && d[0][2]==-1 && d[1][0]==0 && d[1][1]==1 && d[1][2]==0) //sagital opposite
		//	d[0][2] = 1;
		//else
		//	return;
		//
		//vnl_vector<double> rowD(3), colD(3);
		//rowD[0] = d[0][0];	rowD[1] = d[0][1];	rowD[2] = d[0][2];
		//colD[0] = d[1][0];	colD[1] = d[1][1];	colD[2] = d[1][2];
		//vnl_vector<double> sliceD = vnl_cross_3d(rowD, colD);

		//d[0][0] = rowD[0];	d[0][1] = rowD[1];	d[0][2] = rowD[2];
		//d[1][0] = colD[0];	d[1][1] = colD[1];	d[1][2] = colD[2];
		//d[2][0] = sliceD[0]; d[2][1] = sliceD[1];	d[2][2] = sliceD[2];
	}

}

template< typename ImageType > 
typename ImageType::Pointer ImageUtilities::itkReadMultiSliceImageFromFiles(
	const std::vector< std::string >& sliceFilePaths )
{
 	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "itkReadMultiSliceImageFromFiles");
    typedef itk::ImageSeriesReader< ImageType > ReaderType;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileNames( sliceFilePaths );
    itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
    reader->SetImageIO( gdcmImageIO );
    reader->Update();
    return reader->GetOutput();
}

template< typename ImageType > 
typename ImageType::Pointer ImageUtilities::ReadMultiSliceImageFromFiles(
	const std::vector< std::string > &sliceFilePaths,
	bool isTaggedMR )
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "ReadMultiSliceImageFromFiles");

	typename ImageType::Pointer image;

	// tagged MR case: 
	// - only use images where the ImageType contains "M_FFE"
	// - only for 3D images
	// - use smallest spacing for all directions
	// - use "SAGITTAL" images as base orientation
	if( isTaggedMR )
	{
	    LOG4CPLUS_INFO(blLogger::getInstance("dcmapi.ImageUtilities"), "Tagged MR case.");
	    // sort files
	    std::vector< std::vector< std::string > > fileList(3);
	    for( unsigned int i = 0; i < sliceFilePaths.size(); ++i )
	    {
            gdcm::File gdcmFile;
            gdcmFile.SetFileName( sliceFilePaths[i] );
            gdcmFile.Load();
            gdcm::ValEntry imageType = gdcmFile.GetDocEntry(tags::ImageType.m_group, tags::ImageType.m_element);
            // only keep data when the imageType contains "M_FFE"
            if( imageType.GetValue().find("M_FFE") != std::string::npos)
            {
                gdcm::ValEntry orientation = gdcmFile.GetDocEntry(tags::SliceOrientation.m_group, tags::SliceOrientation.m_element);
                std::string tmp = boost::algorithm::trim_copy(orientation.GetValue());
                if( tmp == "SAGITTAL" )
                {
                    fileList[0].push_back( sliceFilePaths[i] );
                }
                else if( tmp == "TRANSVERSAL" )
                {
                    fileList[1].push_back( sliceFilePaths[i] );
                }
                else if( tmp == "CORONAL" )
                {
                    fileList[2].push_back( sliceFilePaths[i] );
                }
                else
                {
                    // error...
                    LOG4CPLUS_ERROR(blLogger::getInstance("dcmapi.ImageUtilities"), "Unknown orientation: " << tmp);
                }
            }
	    }
	    
	    // read each orientation
	    std::vector< typename ImageType::Pointer > images(3);
	    LOG4CPLUS_DEBUG(blLogger::getInstance("dcmapi.ImageUtilities"), "Read SAGITTAL: " << fileList[0].size());
	    images[0] = itkReadMultiSliceImageFromFiles< ImageType >(fileList[0]);
	    LOG4CPLUS_DEBUG(blLogger::getInstance("dcmapi.ImageUtilities"), "Read TRANSVERSAL: " << fileList[1].size());
	    images[1] = itkReadMultiSliceImageFromFiles< ImageType >(fileList[1]);
	    LOG4CPLUS_DEBUG(blLogger::getInstance("dcmapi.ImageUtilities"), "Read CORONAL: " << fileList[2].size());
	    images[2] = itkReadMultiSliceImageFromFiles< ImageType >(fileList[2]);
	    
	    // resample to same orientation
        typedef itk::ResampleImageFilter< ImageType, ImageType, double > ResampleFilterType;
        
        const typename ImageType::DirectionType outputDirection = images[0]->GetDirection();
        const typename ImageType::PointType outputOrigin = images[0]->GetOrigin();
        
        const typename ImageType::SpacingType spacing0 = images[0]->GetSpacing();
        double minSpacing = spacing0[0];
        if( spacing0[1] < minSpacing ) minSpacing = spacing0[1];
        if( spacing0[2] < minSpacing ) minSpacing = spacing0[2];
        double outputSpacing[3];
        outputSpacing[0] = minSpacing;
        outputSpacing[1] = minSpacing;
        outputSpacing[2] = minSpacing;

        const typename ImageType::RegionType outRegion = images[0]->GetBufferedRegion();
        typename ImageType::SizeType outSize;
        for (unsigned int d=0; d<3; d++)
        {
            outSize[d] = static_cast<unsigned int>( images[0]->GetSpacing()[d] * static_cast<double>(outRegion.GetSize()[d]) / outputSpacing[d]);
        }

        typename ResampleFilterType::Pointer resample0 = ResampleFilterType::New();
        resample0->SetInput( images[0] );
        resample0->SetOutputSpacing( outputSpacing );
        resample0->SetOutputDirection( outputDirection );
        resample0->SetOutputOrigin( outputOrigin );
        resample0->SetSize( outSize );

        typename ResampleFilterType::Pointer resample1 = ResampleFilterType::New();
        resample1->SetInput( images[1] );
        resample1->SetOutputSpacing( outputSpacing );
        resample1->SetOutputDirection( outputDirection );
        resample1->SetOutputOrigin( outputOrigin );
        resample1->SetSize( outSize );
	    
        typename ResampleFilterType::Pointer resample2 = ResampleFilterType::New();
        resample2->SetInput( images[2] );
        resample2->SetOutputSpacing( outputSpacing );
        resample2->SetOutputDirection( outputDirection );
        resample2->SetOutputOrigin( outputOrigin );
        resample2->SetSize( outSize );
        
	    // merge all orientations
        typedef itk::MultiplyImageFilter< ImageType, ImageType, ImageType > MultiplyingImageFilter;
        
        typename MultiplyingImageFilter::Pointer mult1 = MultiplyingImageFilter::New();
        mult1->SetInput1( resample0->GetOutput() );
        mult1->SetInput2( resample1->GetOutput() );
        
        typename MultiplyingImageFilter::Pointer mult2 = MultiplyingImageFilter::New();
        mult2->SetInput1( mult1->GetOutput() );
        mult2->SetInput2( resample2->GetOutput() );
        
        // cubic root
        typedef itk::CubicRootImageFilter< ImageType, ImageType > CubicRootImageFilter;
        typename CubicRootImageFilter::Pointer ruth = CubicRootImageFilter::New();
        ruth->SetInput( mult2->GetOutput() ); 
        
        // run the chain
        ruth->Update();
        
        // result
	    image = ruth->GetOutput();
	}
	// standard data
	else
	{
	    image = itkReadMultiSliceImageFromFiles< ImageType >(sliceFilePaths);
    }

	if(image->GetImageDimension() == 3)
	{
		AssureCorrectSpacing< ImageType >(image);
	}

	return image;
};


template< typename ImageType > 
typename ImageType::Pointer ImageUtilities::ReadImageFromFile(
	const char *imageFileName )
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "ReadImageFromFile");

	typename ImageType::Pointer image;

	//read data into itk image
	typedef itk::ImageFileReader< ImageType > ReaderType;
	typename ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName( imageFileName );
	itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
	reader->SetImageIO( gdcmImageIO );
	reader->Update();
	image = reader->GetOutput();

	if(image->GetImageDimension() == 3)
	{
		AssureCorrectSpacing< ImageType >(image);
	}

	return image;
};

template <class PixelType>
vtkSmartPointer<vtkImageData> ImageUtilities::Read2DVtkImageFromFile(
	const char *imageFileName)
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "Read2DVtkImageFromFile");

	typedef itk::Image<PixelType, 2> ImageType;

	typename ImageType::Pointer image;
	image = ReadImageFromFile < ImageType > (imageFileName);

	// convert to vtk image
	vtkSmartPointer<vtkImageData> vtkImage;
	vtkImage = blImageUtils::ConvertFromItkImageToVtkImage < ImageType > ( image );

	return vtkImage;
}



template <class PixelType, unsigned int VImageDimension>
vtkSmartPointer<vtkImageData> ImageUtilities::ReadVtkImageFromFile( 
	const char *imageFileName,
	bool reorientData 
	)
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "ReadVtkImageFromFile");

	typedef itk::Image<PixelType, VImageDimension> ImageType;

	typename ImageType::Pointer volume;
	volume = ReadImageFromFile < ImageType > (imageFileName);

	typename ImageType::Pointer itkFinalImg = NULL;

	//check if user wants to apply orientation filter
	if( reorientData == true && volume->GetImageDimension() > 2)
		itkFinalImg = blITKImageUtils::ApplyOrientationToImage < ImageType > (volume);
	else
		itkFinalImg = volume;

	// convert to vtk image
	vtkSmartPointer<vtkImageData> vtkImage;
	vtkImage = blImageUtils::ConvertFromItkImageToVtkImage < ImageType > ( itkFinalImg );

	return vtkImage;
}

template <class PixelType, unsigned int VImageDimension>
vtkSmartPointer<vtkImageData> ImageUtilities::ReadMultiSliceVtkImageFromFiles( 
	std::vector< std::string > sliceFilePaths,
	bool reorientData,
	bool isTagedMR )
{
	log4cplus::Logger logger = blLogger::getInstance("dcmapi.ImageUtilities");
	LOG4CPLUS_TRACE_METHOD(logger, "ReadMultiSliceVtkImageFromFiles");

	typedef itk::Image<PixelType, VImageDimension> ImageType;

	typename ImageType::Pointer volume;
	volume = ReadMultiSliceImageFromFiles< ImageType >( sliceFilePaths, isTagedMR );

	typename ImageType::Pointer itkFinalImg = NULL;

	//check if user wants to apply orientation filter
	if( reorientData == true )
		itkFinalImg = blITKImageUtils::ApplyOrientationToImage < ImageType > (volume);
	else
		itkFinalImg = volume;

	// convert to vtk image because itk 3d image currently is not rendered in gimias
	vtkSmartPointer<vtkImageData> vtkImage;
	vtkImage = blImageUtils::ConvertFromItkImageToVtkImage < ImageType > ( itkFinalImg );

	return vtkImage;
}

template <class PixelType>
blSliceImage::Pointer ImageUtilities::ReadMultiSliceImageSliceFromFiles( 
	std::vector< std::string > sliceFilePaths )
{
	LOG4CPLUS_TRACE_METHOD(blLogger::getInstance("dcmapi.ImageUtilities"), "ReadMultiSliceImageSliceFromFiles");

	typedef itk::Image<PixelType, 2> ImageType;
	typename ImageType::Pointer image;
	blSliceImage::Pointer sliceImage = blSliceImage::New();
	sliceImage->SetNumberOfSlices(sliceFilePaths.size());
	
	for ( size_t id=0; id < sliceFilePaths.size(); id++)
	{	
		image = ReadImageFromFile< ImageType >( sliceFilePaths.at(id).c_str() );
		sliceImage->SetInternalImage<PixelType>( image, id );
	}
		
	return sliceImage;
}


} // namespace dcmAPI

#endif // _dcmImageUtilities_txx
