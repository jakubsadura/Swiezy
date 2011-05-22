/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blImageUtils.h"

#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkImageData.h"
#include "vtkImageReader.h"
#include "vtkImageWriter.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkFloatArray.h"
#include "vtkMetaImageReader.h"

#include "blV3DImageFileReader.h"
#include "blNumericUtils.h"
#include "boost/intrusive_ptr.hpp"
#include <boost/crc.hpp>      // for boost::crc_basic, boost::crc_optimal
#include <boost/cstdint.hpp>  // for boost::uint16_t

#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include <itkAnalyzeImageIO.h>
#include <itkNiftiImageIO.h>

/*
*/
bool blImageUtils::CheckSaveFileExtension(const char *imageFileName)
{
	bool bRes = false;

	if ( strlen(imageFileName) < 4 )
	{
		return false;
	}
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".vtk") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".vti") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".hdr") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".nii") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".dcm") == 0;
	return bRes;
}

/*
*/
void blImageUtils::SaveImageToFile(
						vtkStructuredPoints *imagePt, 
						const char *imageFileName,
						const char *strHeader,
						bool bSaveAsASCII )
{

	if (CheckSaveFileExtension(imageFileName))
	{
		if (!strcmp(imageFileName+strlen(imageFileName)-4,".vtk"))
		{
			vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
			writer->SetFileName(imageFileName);
			if ( bSaveAsASCII )
			{
				writer->SetFileTypeToASCII();
			}
			else
			{
				writer->SetFileTypeToBinary();
			}
			writer->SetInput(imagePt);
			if ( strHeader != NULL )
			{
				writer->SetHeader( strHeader );
			}
			writer->Update();
			writer->Delete();
		}
		else if (!strcmp(imageFileName+strlen(imageFileName)-4,".vti"))
		{
			vtkXMLImageDataWriter* writer = vtkXMLImageDataWriter::New();
			writer->SetFileName(imageFileName);
			writer->SetInput(imagePt);
			writer->Update();
			writer->Delete();
		}
	}
	else
	{
		// Unsupported data type for saving
		BaseLib::UnsupportedImageTypeException e("Unsupported data type. PLEASE REPORT IT");
		throw e;
	}
}

/*
*/
void blImageUtils::SaveImageToFile(
						vtkImageData *imagePt, 
						const char *imageFileName,
						const char *strHeader,
						bool bSaveAsASCII )
{

	if (CheckSaveFileExtension(imageFileName))
	{
		//no need to check the reading 
		bool checkReading = false;
		switch(GetTypeOfImageData(imageFileName,checkReading))
		{
		case VTKImageDataType:
			{
				vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
				writer->SetFileName(imageFileName);
				writer->SetInput(imagePt);
				if ( bSaveAsASCII )
				{
					writer->SetFileTypeToASCII();
				}
				else
				{
					writer->SetFileTypeToBinary();
				}
				if ( strHeader != NULL )
				{
					writer->SetHeader( strHeader );
				}
				writer->Update();
				writer->Delete();
			}
			break;
		case VTKXMLStructuredPointsType:
			{
				vtkXMLImageDataWriter* writer = vtkXMLImageDataWriter::New();
				writer->SetFileName(imageFileName);
				writer->SetInput(imagePt);
				writer->Update();
				writer->Delete();
			}
			break;
		case AnalyzeImageType:
		case NiftyImageType:
			{
				blImageUtils::WriteITKImage( imagePt, imageFileName );
			}
			break;
		case DicomType:
			{
				blImageUtils::WriteITKImage( imagePt, imageFileName, true );
			}
			break;
		case UnknownType:
		default:
			{
				// Unsupported data type for saving
				BaseLib::UnsupportedImageTypeException e("Unsupported data type. PLEASE REPORT IT");
				throw e;
			}
			break;
		}
	}
}

/*
*/
bool blImageUtils::CheckLoadFileExtension(const char *imageFileName)
{
	bool bRes = false;

	if ( strlen(imageFileName) < 4 )
	{
		return false;
	}

	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".v3d") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".vtk") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".vti") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".mhd") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".hdr") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".nii") == 0;
	bRes |= strcmp(imageFileName+strlen(imageFileName)-4,".dcm") == 0;

	return bRes;
}


/*
*/
blImageUtils::ImageFormats blImageUtils::GetTypeOfImageData(const char *imageFileName, bool checkReadingOfFile)
{
	blImageUtils::ImageFormats type = blImageUtils::UnknownType;

	if (!strcmp(imageFileName+strlen(imageFileName)-4,".v3d"))
	{
		type = blImageUtils::V3DImageType;
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".vti"))
	{
		if (checkReadingOfFile)
		{
			vtkXMLImageDataReader* reader = vtkXMLImageDataReader::New();
			if(reader->CanReadFile(imageFileName))
				type = blImageUtils::VTKXMLStructuredPointsType;
			reader->Delete();
		}
		else
		{
			type = blImageUtils::VTKXMLStructuredPointsType;
		}
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".vtk"))
	{
		if (checkReadingOfFile)
		{
			vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
			reader->SetFileName(imageFileName);
			if(reader->IsFileStructuredPoints()) 
				type = blImageUtils::VTKImageDataType;
			reader->Delete();
		}
		else
		{
			type = blImageUtils::VTKImageDataType;
		}
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".mhd"))
	{
		if (checkReadingOfFile)
		{
			vtkMetaImageReader* reader = vtkMetaImageReader::New();
			if(reader->CanReadFile(imageFileName))
				type = blImageUtils::MHDImageType;
			reader->Delete();
		}
		else
		{
			type = blImageUtils::MHDImageType;
		}
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".hdr"))
	{
		type = blImageUtils::AnalyzeImageType;
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".nii"))
	{
		type = blImageUtils::NiftyImageType;
	}
	else if (!strcmp(imageFileName+strlen(imageFileName)-4,".dcm"))
	{
		type = blImageUtils::DicomType;
	}
	// else defaults to blImageUtils::UnknownType

	return type;
}



/*
*/
vtkSmartPointer<vtkImageData> blImageUtils::LoadImageFromFileAsVTK(const char *imageFileName)
{
	vtkSmartPointer<vtkImageData> pImage;
	bool checkReading = true;

	if (CheckLoadFileExtension(imageFileName))
	{
		switch(GetTypeOfImageData(imageFileName,checkReading))
		{
		case VTKImageDataType:
			{
				vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
				reader->SetFileName(imageFileName);
				reader->Update();
				pImage = reader->GetOutput();
			}
			break;
		case VTKXMLStructuredPointsType:
			{
				vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
				reader->SetFileName(imageFileName);
				reader->Update();
				pImage = reader->GetOutput();
			}
			break;
		case V3DImageType:
			{
				blV3DImageFileReader::Pointer reader = blV3DImageFileReader::New();
				reader->SetFileName(imageFileName);
				reader->Update();
				/*vtkStructuredPoints* imagePtTemp = NULL;
				imagePtTemp = static_cast<vtkStructuredPoints*>(reader->GetVTKOutput());
				imagePt = vtkStructuredPoints::New();
				imagePt->DeepCopy(imagePtTemp);*/
				pImage = vtkSmartPointer<vtkImageData>::New();
				pImage->DeepCopy( reader->GetVTKOutput() );
			}
			break;

		case MHDImageType:
			{
				vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
				reader->SetFileName(imageFileName);
				reader->Update();
				//! Check that the .mhd image is a image 
				if ( reader->GetOutput()->GetPointData()->GetScalars()->GetNumberOfComponents() == 1 )
				{
					//imagePtTemp = static_cast<vtkStructuredPoints*>(reader->GetOutput());
					pImage = reader->GetOutput();
				}
			}
			break;

		case AnalyzeImageType:
		case NiftyImageType:
		case DicomType:
		case UnknownType: // this one should not be there, not sure we can support this format... (YM)
			pImage = LoadITKImage( imageFileName );
			break;
		default:
			{
				// Unsupported data type for reading
				BaseLib::UnsupportedImageTypeException e("Data type not supported. PLEASE REPORT IT");
				throw e;
			}
			break;
		}

	}
	//return imagePt;
	return pImage;
}


/** 
 */
bool blImageUtils::CompareImages( 
								 const char *imageFileName1, 
								 const char *imageFileName2,
								 float fEpsilon )
{
	// Read images
	vtkSmartPointer<vtkImageData> image1 = LoadImageFromFileAsVTK( imageFileName1 );
	vtkSmartPointer<vtkImageData> image2 = LoadImageFromFileAsVTK( imageFileName2 );

	// Perform comparison
	const bool bRes = blImageUtils::CompareImages( image1, image2, fEpsilon );
	if ( !bRes )
	{
	   std::cout << "[blImageUtils::CompareImages]" << std::endl;
	   std::cout << "imageFileName1: " << imageFileName1 << std::endl;
	   std::cout << "imageFileName2: " << imageFileName2 << std::endl;
		std::cout << "-> Images are not similar." << std::endl;
	}

	return bRes;
}

/**
 */
bool blImageUtils::CompareImages( 
						vtkSmartPointer<vtkImageData> image1,
						vtkSmartPointer<vtkImageData> image2,
						float fEpsilon )
{
	bool bRes = true;

	if ( image1.GetPointer() == NULL || image2.GetPointer() == NULL )
		return false;

	const unsigned int dimension = image1->GetDataDimension();
   if(dimension != image2->GetDataDimension())
   {
      std::cout << "Data dimension is not equal" << std::endl;
      return false;
   }
	int dim1[3], dim2[3]; //has to be 3 otherwise there is variable corruption
	image1->GetDimensions(dim1);
	image2->GetDimensions(dim2);
	for( size_t i = 0; i < dimension; ++i )
	{
	   if(dim1[i] != dim2[i])
	   {
		   std::cout << "Dimensions [" << i << "] are not equal: " << dim1[i] << " != " << dim2[i] << std::endl;
		   return false;
	   }
	}

	double spacing1[3], spacing2[3]; //has to be 3 otherwise there is variable corruption
	image1->GetSpacing(spacing1);
	image2->GetSpacing(spacing2);
	for( size_t i = 0; i < dimension; ++i )
	{
	   if(fabs(spacing1[i] - spacing2[i]) > fEpsilon)
	   {
		   std::cout << "Spacing [" << i << "] are not equal: " << spacing1[i] << " != " << spacing2[i] << std::endl;
		   return false;
	   }
	}
	
	// Check number of points
	const unsigned int numberOfPoints = image1->GetNumberOfPoints();
	if( numberOfPoints != image2->GetNumberOfPoints() )
	{
		std::cout << "Number of points are not equal: "
			<< image1->GetNumberOfPoints() << " != "
			<< image2->GetNumberOfPoints() << std::endl;
		return false;
	}
	
	// Check point positions
	bool coordComp = true;
	float coordMaxDifference = 0;
	int iMaxPointPosition = -1;
	double point1[ 3 ];
	double point2[ 3 ];
	for( unsigned int i = 0; i < numberOfPoints; i++ )
	{
		image1->GetPoint( i, point1 );
		image2->GetPoint( i, point2 );

		// Compare point coordinates
		for( unsigned int iCoord = 0; iCoord < dimension; iCoord++ )
		{
			if( !FLOAT_EQ_INCLUSIVE( point1[ iCoord ], point2[ iCoord ], fEpsilon ) )
			{
				std::cout << "Different points position at " << i << std::endl;
				std::cout.precision( 10 );
				
				std::cout << "point1:";
				for( unsigned int i = 0; i < dimension; ++i )
				{
					std::cout << " " << point1[i];
				}
				std::cout << std::endl;
				
				std::cout << "point2:";
				for( unsigned int i = 0; i < dimension; ++i )
				{
					std::cout << " " << point2[i];
				}
				std::cout << std::endl;
				return false;
			}
		}
	}

	// Compare voxel intensity values
	double dPixelValue1;
	double dPixelValue2;
	vtkDataArray *scalars1 = image1->GetPointData()->GetScalars();
	vtkDataArray *scalars2 = image2->GetPointData()->GetScalars();
	for( unsigned int i = 0; i < numberOfPoints; i++ )
	{
		dPixelValue1 = scalars1->GetTuple1( i );
		dPixelValue2 = scalars2->GetTuple1( i );
		if( !FLOAT_EQ_INCLUSIVE( dPixelValue1, dPixelValue2, fEpsilon ) )
		{
			std::cout << "Different intensity values in voxels: "
				<< dPixelValue1 << " != " << dPixelValue2
				<< " at index " << i << std::endl;
			return false;
		}
	}

	return bRes;
}

int blImageUtils::ComputeCRC( vtkImageData* image )
{
	int imageSize = image->GetNumberOfPoints() * image->GetScalarSize();
	boost::crc_basic<16> crc_ccitt1( 0x1021, 0xFFFF, 0, false, false );
	crc_ccitt1.process_bytes( image->GetScalarPointer(), imageSize );
	return crc_ccitt1.checksum();
}


vtkSmartPointer<vtkImageData> blImageUtils::LoadITKImage( const char *imageFileName,  bool orientedImage )
{
	// Read pixel type and number of dimensions to instantiate a new reader
	itk::ImageIOBase::Pointer imageIO;
	imageIO = itk::ImageIOFactory::CreateImageIO( 
		imageFileName, 
		itk::ImageIOFactory::ReadMode );
	if ( imageIO.IsNull() )
	{
		return NULL;
	}
	imageIO->SetFileName( imageFileName );
	imageIO->ReadImageInformation( );

	itk::ImageIOBase::IOComponentType componentType;
	//unsigned int numberOfDimensions;
	componentType = imageIO->GetComponentType( );
	const unsigned int numberOfDimensions = imageIO->GetNumberOfDimensions( );

	// Read image
	vtkSmartPointer<vtkImageData> pImage;
	if(numberOfDimensions == 2)
	{
		switch ( componentType )
		{
			blVtkTemplateMacro( 
				( pImage = LoadITKImage2DTemplate< PixelT >( imageFileName ) ) );
		}
	}
	else if (numberOfDimensions == 3)
	{
		//if(orientedImage == false)
		switch ( componentType )
		{
			blVtkTemplateMacro( 
			( pImage = LoadITKImage3DTemplate< PixelT >( imageFileName ) ) );
		}
		//else
		/*switch ( componentType )
		{
			blVtkTemplateMacro( 
				( pImage = LoadItkOrientedImageTemplate< PixelT, 3 >( imageFileName ) ) );
		}*/
	}

	return pImage;
}

void blImageUtils::WriteITKImage( vtkImageData* vtkInputImage, const string& imageFileName, bool flip)
{
	int dimension = vtkInputImage->GetDataDimension();
	if(dimension == 2)
	{
		// write 3D image
		switch ( vtkInputImage->GetScalarType() )
		{
		case VTK_CHAR:
			WriteITKImageTemplate< char, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_UNSIGNED_CHAR:
			WriteITKImageTemplate< unsigned char, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_SIGNED_CHAR:
			WriteITKImageTemplate< char, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_SHORT:
			WriteITKImageTemplate< short, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_UNSIGNED_SHORT:
			WriteITKImageTemplate< unsigned short, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_LONG:
			WriteITKImageTemplate< long, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_INT:
			WriteITKImageTemplate< int, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_DOUBLE:
			WriteITKImageTemplate< double, 2 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_FLOAT:
			WriteITKImageTemplate< float, 2 >( vtkInputImage, imageFileName, flip );
			break;
		default:
			{
				BaseLib::UnsupportedImagePixelTypeException e("Pixel Type not converted. PLEASE REPORT IT");
				throw e;
			}
		}
	}
	else if(dimension == 3)
	{
		// write 3D image
		switch ( vtkInputImage->GetScalarType() )
		{
		case VTK_CHAR:
			WriteITKImageTemplate< char, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_UNSIGNED_CHAR:
			WriteITKImageTemplate< unsigned char, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_SIGNED_CHAR:
			WriteITKImageTemplate< char, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_SHORT:
			WriteITKImageTemplate< short, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_UNSIGNED_SHORT:
			WriteITKImageTemplate< unsigned short, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_LONG:
			WriteITKImageTemplate< long, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_INT:
			WriteITKImageTemplate< int, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_DOUBLE:
			WriteITKImageTemplate< double, 3 >( vtkInputImage, imageFileName, flip );
			break;
		case VTK_FLOAT:
			WriteITKImageTemplate< float, 3 >( vtkInputImage, imageFileName, flip );
			break;
		default:
			{
			BaseLib::UnsupportedImagePixelTypeException e("Pixel Type not converted. PLEASE REPORT IT");
			throw e;
			}
		}
	}
}
