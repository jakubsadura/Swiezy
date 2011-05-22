// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blImageUtilsTest.h"
#include "blImageUtils.h"

#include "vtkImageData.h"
#include "vtkStructuredPoints.h"
#include "vtkSmartPointer.h"
#include "vtkImageResample.h"

// Predefined paths
#include "CISTIBToolkit.h"

//void blImageUtilsTest::TestLoadImageFromFileAsVTK()
//{
	// NOT WORKING...

	//const std::string filename("data/demo-US-one-chamber.vtk");
	//vtkSmartPointer<vtkImageData> image;
	//image.TakeReference( blImageUtils::LoadImageFromFileAsVTK( filename.c_str() ) );
	//TS_ASSERT( image );

	//// Check CRC of the loaded image
	//const int expected = 28765;
	//TS_ASSERT_EQUALS( blImageUtils::ComputeCRC(image), expected );

    //// At some point, copying the memory of the image resulted in a crash. Therefore, test if we can still
	//// copy the image (don't remove this part!).
	//int imageSize = image->GetNumberOfPoints() * image->GetScalarSize();
	//unsigned char* test = new unsigned char[imageSize];
	//memcpy(test, image->GetScalarPointer(), imageSize);
//}

template< class T >
bool checkValue( vtkSmartPointer<vtkImageData> image, 
   size_t iMin, size_t iMax,
   size_t jMin, size_t jMax, 
   size_t kMin, size_t kMax, 
   const T& valueRef )
{
	T value;
	for( size_t k = kMin; k < kMax; ++k )
	{
	   for( size_t j = jMin; j < jMax; ++j )
	   {
	      for( size_t i = iMin; i < iMax; ++i )
	      {
	         value = *static_cast< T* >(image->GetScalarPointer( i, j, k ) );
	         if( value != valueRef )
	         {
	            std::cout << "Values are not equal: " << value << " != " << valueRef << std::endl;
	            return false;
	         }
         }
      }
   }
	return true;
}

void blImageUtilsTest::TestLoadVTKImage()
{
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/vtk/axial_colors.vtk";
	vtkSmartPointer<vtkImageData> image = blImageUtils::LoadImageFromFileAsVTK( filename.c_str() );
	
	// check data type
	TSM_ASSERT( "Scalar type is not expected.", image->GetScalarType() == VTK_UNSIGNED_SHORT );
	// check size
	int dim[3];
	image->GetDimensions( dim );
	TSM_ASSERT( "Number of columns is not expected.", dim[0] == 128 );
	TSM_ASSERT( "Number of rows is not expected.", dim[1] == 128 );
	TSM_ASSERT( "Number of slice is not expected.", dim[2] == 128 );
	
	// test specific point intensities
	
	// First quarter
	TSM_ASSERT( "Values of the first quarter are not equal.", 
	   checkValue< unsigned short >( image, 0, dim[0]/2, 0, dim[1]/2, 0, dim[2]/2, 80 ) );
	// Second quarter
	TSM_ASSERT( "Values of the second quarter are not equal", 
	   checkValue< unsigned short >( image, dim[0]/2, dim[0], 0, dim[1]/2, 0, dim[2]/2, 0 ) );
	// Third quarter
	TSM_ASSERT( "Values of the third quarter are not equal", 
	   checkValue< unsigned short >( image, 0, dim[0]/2, dim[1]/2, dim[1], 0, dim[2]/2, 119 ) );
	// Fourth quarter
	TSM_ASSERT( "Values of the fourth quarter are not equal", 
	   checkValue< unsigned short >( image, dim[0]/2, dim[0], dim[1]/2, dim[1], 0, dim[2]/2, 120 ) );

	// Fifth quarter
	TSM_ASSERT( "Values of the fifth quarter are not equal.", 
	   checkValue< unsigned short >( image, 0, dim[0]/2, 0, dim[1]/2, dim[2]/2, dim[2], 81 ) );
	// Sixth quarter
	TSM_ASSERT( "Values of the sixth quarter are not equal", 
	   checkValue< unsigned short >( image, dim[0]/2, dim[0], 0, dim[1]/2, dim[2]/2, dim[2], 99 ) );
	// Seventh quarter
	TSM_ASSERT( "Values of the seventh quarter are not equal", 
	   checkValue< unsigned short >( image, 0, dim[0]/2, dim[1]/2, dim[1], dim[2]/2, dim[2], 0 ) );
	// Eighth quarter
	TSM_ASSERT( "Values of the eighth quarter are not equal", 
	   checkValue< unsigned short >( image, dim[0]/2, dim[0], dim[1]/2, dim[1], dim[2]/2, dim[2], 100 ) );
}

void blImageUtilsTest::TestLoadWriteAnalyzeImage()
{
	// delta for double comparison
	const double delta = 1e-5;

	// Read the data
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/analyze/IMAFUNE216.hdr";
	vtkSmartPointer< vtkImageData > image;
	image = blImageUtils::LoadITKImage( filename.c_str() );
	TS_ASSERT( image );
	
	// Check content
	const int dimX = 64;
	const int dimY = dimX;
	const int dimZ = dimX;
	const double spacingX = 4.423999;
	const double spacingY = spacingX;
	const double spacingZ = spacingX;
	
	const int* dim = image->GetDimensions();
	TS_ASSERT_EQUALS( dim[0], dimX );
	TS_ASSERT_EQUALS( dim[1], dimY );
	TS_ASSERT_EQUALS( dim[2], dimZ );
	const double* spacing = image->GetSpacing();
	TS_ASSERT_DELTA( spacing[0], spacingX, delta );
	TS_ASSERT_DELTA( spacing[1], spacingY, delta );
	TS_ASSERT_DELTA( spacing[2], spacingZ, delta );

	// The resampling is causing problems when writing...

	// Modify content
	//const double factorX = 1.25;
	//const double factorY = 0.75;
	//const double factorZ = 0.5;
	//const int newDimX = dimX * factorX;
	//const int newDimY = dimY * factorY;
	//const int newDimZ = dimZ * factorZ;
	//const double newSpacingX = spacingX * factorX;
	//const double newSpacingY = spacingY * factorY;
	//const double newSpacingZ = spacingZ * factorZ;
	
	//vtkImageResample* resample = vtkImageResample::New();
	//resample->SetInput( image );
	//resample->SetDimensionality( 3 );
	//resample->SetAxisMagnificationFactor( 0, factorX );
	//resample->SetAxisMagnificationFactor( 1, factorY );
	//resample->SetAxisMagnificationFactor( 2, factorZ );
	
	// Write the data
	const std::string outFilename("IMAFUNE216_modified.hdr");
	//blImageUtils::WriteITKImage( resample->GetOutput(), outFilename.c_str() );
	blImageUtils::WriteITKImage( image, outFilename.c_str() );
	
	// Read it back
	vtkSmartPointer< vtkImageData > image2;
	image2 = blImageUtils::LoadITKImage( outFilename.c_str() );
	TS_ASSERT( image2 );
	
	// Check content
	const int* dim2 = image2->GetDimensions();
	TS_ASSERT_EQUALS( dim2[0], dimX );
	TS_ASSERT_EQUALS( dim2[1], dimY );
	TS_ASSERT_EQUALS( dim2[2], dimZ );
	const double* spacing2 = image2->GetSpacing();
	TS_ASSERT_DELTA( spacing2[0], spacingX, delta );
	TS_ASSERT_DELTA( spacing2[1], spacingY, delta );
	TS_ASSERT_DELTA( spacing2[2], spacingZ, delta );

	// Clean up
	if( remove( outFilename.c_str() ) != 0 )
	{
		perror( "Error deleting file." );
      TSM_ASSERT( "Error deleting file.", false );
	}
}

void blImageUtilsTest::TestLoadWriteNiftiImage()
{
	// delta for double comparison
	const double delta = 1e-5;

	// Read the data
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/avg152T1_LR_nifti.nii";
	vtkSmartPointer< vtkImageData > image;
	image = blImageUtils::LoadITKImage( filename.c_str() );
	TS_ASSERT( image );
	
	// Check content
	const int dimX = 91;
	const int dimY = 109;
	const int dimZ = 91;
	const double spacingX = 2;
	const double spacingY = spacingX;
	const double spacingZ = spacingX;
	
	const int* dim = image->GetDimensions();
	TS_ASSERT_EQUALS( dim[0], dimX );
	TS_ASSERT_EQUALS( dim[1], dimY );
	TS_ASSERT_EQUALS( dim[2], dimZ );
	const double* spacing = image->GetSpacing();
	TS_ASSERT_DELTA( spacing[0], spacingX, delta );
	TS_ASSERT_DELTA( spacing[1], spacingY, delta );
	TS_ASSERT_DELTA( spacing[2], spacingZ, delta );

	// The resampling is causing problems when writing...

	// Modify content
	//const double factorX = 1.25;
	//const double factorY = 0.75;
	//const double factorZ = 0.5;
	//const int newDimX = dimX * factorX;
	//const int newDimY = dimY * factorY;
	//const int newDimZ = dimZ * factorZ;
	//const double newSpacingX = spacingX * factorX;
	//const double newSpacingY = spacingY * factorY;
	//const double newSpacingZ = spacingZ * factorZ;
	
	//vtkImageResample* resample = vtkImageResample::New();
	//resample->SetInput( image );
	//resample->SetDimensionality( 3 );
	//resample->SetAxisMagnificationFactor( 0, factorX );
	//resample->SetAxisMagnificationFactor( 1, factorY );
	//resample->SetAxisMagnificationFactor( 2, factorZ );
	
	// Write the data
	const std::string outFilename("avg152T1_LR_nifti_modified.hdr");
	//blImageUtils::WriteITKImage( resample->GetOutput(), outFilename.c_str() );
	blImageUtils::WriteITKImage( image, outFilename.c_str() );
	
	// Read it back
	vtkSmartPointer< vtkImageData > image2;
	image2 = blImageUtils::LoadITKImage( outFilename.c_str() );
	TS_ASSERT( image2 );
	
	// Check content
	const int* dim2 = image2->GetDimensions();
	TS_ASSERT_EQUALS( dim2[0], dimX );
	TS_ASSERT_EQUALS( dim2[1], dimZ );
	TS_ASSERT_EQUALS( dim2[2], dimY );
	const double* spacing2 = image2->GetSpacing();
	TS_ASSERT_DELTA( spacing2[0], spacingX, delta );
	TS_ASSERT_DELTA( spacing2[1], spacingY, delta );
	TS_ASSERT_DELTA( spacing2[2], spacingZ, delta );

	// Clean up
	if( remove( outFilename.c_str() ) != 0 )
	{
		perror( "Error deleting file." );
      TSM_ASSERT( "Error deleting file.", false );
	}
}

void blImageUtilsTest::TestNiftiImageOrientationAxial()
{
	// delta for double comparison
	const double delta = 1e-5;
	// base file
	const std::string filenameA0 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/vtk/axial_colors.vtk";

	// Read the data
	const std::string filenameA1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/axial_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameA1.c_str(), filenameA0.c_str(), delta) );

	// Read the data
	const std::string filenameA2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/axial2_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameA2.c_str(), filenameA0.c_str(), delta) );
}

void blImageUtilsTest::TestNiftiImageOrientationCoronal()
{
	// delta for double comparison
	const double delta = 1e-5;
	// base file
	const std::string filenameA0 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/vtk/axial_colors.vtk";

	// Read the data
	const std::string filenameC1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/coronal_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameC1.c_str(), filenameA0.c_str(), delta) );

	// Read the data
	const std::string filenameC2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/coronal2_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameC2.c_str(), filenameA0.c_str(), delta) );
}

void blImageUtilsTest::TestNiftiImageOrientationSagittal()
{
	// delta for double comparison
	const double delta = 1e-5;
	// base file
	const std::string filenameA0 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/vtk/axial_colors.vtk";

	// Read the data
	const std::string filenameS1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/sagittal_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameS1.c_str(), filenameA0.c_str(), delta) );

	// Read the data
	const std::string filenameS2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/nifti/sagittal2_colors.nii";
	// Compare it with Test one
	TS_ASSERT( blImageUtils::CompareImages( filenameS2.c_str(), filenameA0.c_str(), delta) );
}

void blImageUtilsTest::TestLoadWriteV3dImage()
{
	// delta for double comparison
	const double delta = 1e-3;

	// Read the data
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/v3d/Image.v3d";
	const std::string refFilename =std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/BaseLib/v3d/v3dImageRef.vtk";
	vtkSmartPointer< vtkImageData > image;
	image = blImageUtils::LoadImageFromFileAsVTK( filename.c_str() );
	TS_ASSERT( image );

	// Check content
	const int dimX = 256;
	const int dimY = dimX;
	const int dimZ = dimX;
	const double spacingX = 0.1210;
	const double spacingY = spacingX;
	const double spacingZ = spacingX;

	const int* dim = image->GetDimensions();
	TS_ASSERT_EQUALS( dim[0], dimX );
	TS_ASSERT_EQUALS( dim[1], dimY );
	TS_ASSERT_EQUALS( dim[2], dimZ );
	const double* spacing = image->GetSpacing();
	TS_ASSERT_DELTA( spacing[0], spacingX, delta );
	TS_ASSERT_DELTA( spacing[1], spacingY, delta );
	TS_ASSERT_DELTA( spacing[2], spacingZ, delta );

	// Write the data to a vtk
	const std::string outFilename("v3dImage.vtk");
	blImageUtils::SaveImageToFile( image, outFilename.c_str(), "" );

	// Compare it with Test one
	bool res = blImageUtils::CompareImages(
						outFilename.c_str(),
						refFilename.c_str(),
						delta);
	TS_ASSERT(res);
	
	// Clean up
	if( remove( outFilename.c_str() ) != 0 )
	{
		perror( "Error deleting file." );
      TSM_ASSERT( "Error deleting file.", false );
	}

}
