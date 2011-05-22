/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BLIMAGEUTILS_txx
#define _BLIMAGEUTILS_txx

#include "blITKImageUtils.h"

//vtk
#include "vtkType.h"
//itk
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageToVTKImageFilter.h"
#include "itkVTKImageToImageFilter.h"
#include "itkOrientImageFilter.h"
#include <itkCastImageFilter.h>

/**
The blVtkTemplateMacro is used to centralize the set of types
 supported by LoadITKImageTemplate( ) function.  It also avoids duplication 
 of long switch statement case lists.
\note Is base in vtkTemplateMacro.
*/
#define blVtkTemplateMacroCase(typeN, typeP, callFucntion)     \
  case typeN: { typedef typeP PixelT; callFucntion; }; break;

#define blVtkTemplateMacro(call)                                          \
	blVtkTemplateMacroCase(itk::ImageIOBase::DOUBLE, double, call);       \
	blVtkTemplateMacroCase(itk::ImageIOBase::FLOAT, float, call);         \
	blVtkTemplateMacroCase(itk::ImageIOBase::LONG, long, call);           \
	blVtkTemplateMacroCase(itk::ImageIOBase::ULONG, unsigned long, call); \
	blVtkTemplateMacroCase(itk::ImageIOBase::INT, int, call);             \
	blVtkTemplateMacroCase(itk::ImageIOBase::UINT, unsigned int, call);   \
	blVtkTemplateMacroCase(itk::ImageIOBase::SHORT, short, call);         \
	blVtkTemplateMacroCase(itk::ImageIOBase::USHORT, unsigned short, call);\
	blVtkTemplateMacroCase(itk::ImageIOBase::CHAR, char, call);           \
	blVtkTemplateMacroCase(itk::ImageIOBase::UCHAR, unsigned char, call)


template <class PixelType>
vtkSmartPointer<vtkImageData> LoadITKImage2DTemplate( const char *imageFileName )
{
	typedef itk::Image<PixelType, 2> ImageType;

	// File reader
	typename itk::ImageFileReader<ImageType>::Pointer reader;
	reader = itk::ImageFileReader<ImageType>::New();
	reader->SetFileName( imageFileName );
	reader->Update();

	// Exit if no data read
	if ( reader->GetOutput( ) == NULL )
	{
		return NULL;
	}
	
	// Convert the itkImage to vtkImage, to adapt the converter's input
	typedef itk::ImageToVTKImageFilter<ImageType> ToVtkImageFilter;
	typename ToVtkImageFilter::Pointer imageAdaptor = ToVtkImageFilter::New();
	imageAdaptor->SetInput( reader->GetOutput( ) );
	imageAdaptor->Update();

	// Copy the output
	vtkSmartPointer<vtkImageData> pImage = vtkImageData::New( );
	pImage->DeepCopy( imageAdaptor->GetOutput( ) );
	
	return pImage;
}

template <class PixelType>
vtkSmartPointer<vtkImageData> LoadITKImage3DTemplate( const char *imageFileName )
{
	// does not seem to make a difference to use itk::OrientedImage
	typedef itk::Image<PixelType, 3> ImageType; 

	// File reader
	typename itk::ImageFileReader<ImageType>::Pointer reader;
	reader = itk::ImageFileReader<ImageType>::New();
	reader->SetFileName( imageFileName );
	reader->Update();

	// Exit if no data read
	if ( reader->GetOutput() == NULL )
	{
		return NULL;
	}
	
	// Reorient the data
	typename ImageType::Pointer orientedImage = 
	   blITKImageUtils::ApplyOrientationToImage< ImageType >( reader->GetOutput() );
			
	// Convert the itkImage to vtkImage, to adapt the converter's input
	typedef itk::ImageToVTKImageFilter<ImageType> ToVtkImageFilter;
	typename ToVtkImageFilter::Pointer imageAdaptor = ToVtkImageFilter::New();
	imageAdaptor->SetInput( orientedImage );
	imageAdaptor->Update();

	// Copy the output
	vtkSmartPointer<vtkImageData> pImage = vtkImageData::New( );
	pImage->DeepCopy( imageAdaptor->GetOutput() );
	
	return pImage;
}

template <class PixelType, unsigned int VImageDimension>
void WriteITKImageTemplate( vtkImageData* vtkInputImage, const std::string& filename, bool flip = false )
{
	typedef itk::Image<PixelType, VImageDimension> OutputImageType;
	typedef itk::ImageFileWriter< OutputImageType > WriterType;
	
	// Convert the itkImage to vtkImage, to adapt the converter's input
	typedef itk::VTKImageToImageFilter< OutputImageType > ToItkImageFilter;
	typename ToItkImageFilter::Pointer imageAdaptor = ToItkImageFilter::New();
	imageAdaptor->SetInput( vtkInputImage );
	imageAdaptor->Update();

	//flip the image because this version of gdcm doesnt generate proper image 
	/*typename const itk::Image<PixelType, VImageDimension>* itkFinalImage;
	if(flip)
			itkFinalImage = blITKImageUtils::FlipItkImage< itk::Image<PixelType, VImageDimension> >( imageAdaptor->GetOutput() );
	else
		itkFinalImage = imageAdaptor->GetOutput();*/

	// Write the ITK data to file
	typename WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( filename.c_str() );
	if(flip)
	{
			typename itk::Image<PixelType, VImageDimension>::Pointer itkFinalImage;
			itkFinalImage = blITKImageUtils::FlipItkImage< itk::Image<PixelType, VImageDimension> >( imageAdaptor->GetOutput() );
			writer->SetInput( itkFinalImage );
	}
	else
		writer->SetInput( imageAdaptor->GetOutput() );
	//writer->SetInput( itkFinalImage );
	writer->Update();
};

template <typename inputPixelType, typename OutputItkImageType>
typename OutputItkImageType::Pointer blImageUtils::VtkToItkImage(
	vtkSmartPointer<vtkImageData> currentVtkInputImage )
{
	typename OutputItkImageType::Pointer itkOutputImage;

	try
	{
		//Conversion from vtk to itk
		typedef itk::Image<inputPixelType,3> itkImageType;
		const itkImageType* itkFilterOutputImage;
		typename itk::VTKImageToImageFilter< itkImageType >::Pointer vtkToItkFilter =
											itk::VTKImageToImageFilter< itkImageType >::New();
		vtkToItkFilter->SetInput( currentVtkInputImage );	
		vtkToItkFilter->Update();
		itkFilterOutputImage = vtkToItkFilter->GetOutput();

		//Conversion inbetween itk image pixel types: from m_pixelType to outputItkImageType 
		typename itk::CastImageFilter< itkImageType, OutputItkImageType >::Pointer itkImageFilter = 
								itk::CastImageFilter< itkImageType, OutputItkImageType >::New();
		itkImageFilter->SetInput(itkFilterOutputImage);
		itkImageFilter->Update();
		itkOutputImage = itkImageFilter->GetOutput();

	}
	catch( std::exception& e )
	{
		std::cout << e.what();
	}

	return itkOutputImage;
}

template <typename outputItkImageType>
typename outputItkImageType::Pointer blImageUtils::ComputeVtkToItkImageAdaptation
(
	vtkImageData* vtkInputImage,
	int pixelType
)
{
	vtkSmartPointer<vtkImageData> currentVtkInputImage = vtkInputImage;
	int m_pixelType = pixelType;

	typename outputItkImageType::Pointer itkOutputImage = NULL;

	switch( m_pixelType )
	{

	case VTK_CHAR:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<char, outputItkImageType>( currentVtkInputImage );
		break;
	}

	case VTK_UNSIGNED_CHAR:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<unsigned char, outputItkImageType>( currentVtkInputImage );
		break;
	}
	
	case VTK_SIGNED_CHAR:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<signed char, outputItkImageType>( currentVtkInputImage );
		break;
	}

	case VTK_SHORT:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<short, outputItkImageType>( currentVtkInputImage );
		break;
	}
	
	case VTK_UNSIGNED_SHORT:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<unsigned short, outputItkImageType>( currentVtkInputImage );
		break;
	}

	case VTK_LONG:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<long, outputItkImageType>( currentVtkInputImage );
		break;
	}
	case VTK_INT:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<int, outputItkImageType>( currentVtkInputImage );
		break;
	}
	case VTK_DOUBLE:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<double, outputItkImageType>( currentVtkInputImage );
		break;
	}
	case VTK_FLOAT:
	{
		itkOutputImage = blImageUtils::VtkToItkImage<float, outputItkImageType>( currentVtkInputImage );
		break;
	}
	default:
		BaseLib::UnsupportedImagePixelTypeException e("Pixel Type not converted. PLEASE REPORT IT");
		throw e;
	}
	return itkOutputImage;
}


template<typename ImageType>
vtkSmartPointer<vtkImageData> 
blImageUtils::ConvertFromItkImageToVtkImage(
	typename ImageType::Pointer itkImage )
{
	vtkSmartPointer<vtkImageData> vtkImage = vtkSmartPointer<vtkImageData>::New();

	//convert to vtk image is this type is for sure rendered by mitk
	typename itk::ImageToVTKImageFilter< ImageType >::Pointer imageAdaptor;
	imageAdaptor = itk::ImageToVTKImageFilter< ImageType >::New();
	imageAdaptor->SetInput(itkImage);
	imageAdaptor->Update();
	vtkImage->DeepCopy( imageAdaptor->GetOutput() );

	return vtkImage;
};

template <class PixelType, unsigned int VImageDimension>
vtkSmartPointer<vtkImageData> LoadItkOrientedImageTemplate( const char *imageFileName )
{
	vtkSmartPointer<vtkImageData> pImage = vtkImageData::New( );
	//typedef itk::Image<PixelType, VImageDimension> ImageType;
	 typedef itk::OrientedImage< PixelType, VImageDimension > ImageType;

	typename itk::ImageFileReader<ImageType>::Pointer reader;
	reader = itk::ImageFileReader<ImageType>::New();
	reader->SetFileName( imageFileName );
	reader->Update();

	if ( reader->GetOutput( ) == NULL )
	{
		return NULL;
	}
	
	typename ImageType::Pointer orientedImage = NULL;
	orientedImage = blITKImageUtils::ApplyOrientationToImage < ImageType > (reader->GetOutput( ));	
			
	// Convert the itkImage to vtkImage, to adapt the converter's input
	typedef itk::ImageToVTKImageFilter<ImageType> ToVtkImageFilter;
	typename ToVtkImageFilter::Pointer imageAdaptor = ToVtkImageFilter::New();
	imageAdaptor->SetInput( orientedImage );
	imageAdaptor->Update();

	// Copy the output
	pImage->DeepCopy( imageAdaptor->GetOutput( ) );
	
	return pImage;
}


#endif // _BLIMAGEUTILS_txx
