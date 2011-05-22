/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ReadOrientationTest.h"

#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
// VTK
#include "vtkSmartPointer.h"
#include "vtkImageClip.h"
// DcmAPI
#include "dcmDataSetReader.h"
#include "dcmMultiSliceReader.h"
#include "dcmImageUtilities.h"
// Predefined paths
#include "CISTIBToolkit.h"
// Other
#include "blVTKHelperTools.h"

bool ReadOrientationTest::ReadAndCompare( 
    const std::string& dcmFileNameEnd, 
    const std::string& vtkFileNameEnd ) const
{
	try
	{
        // Working directory of the test
        const std::string dcmFileName = std::string(CISTIB_TOOLKIT_FOLDER) + dcmFileNameEnd;
        const std::string vtkFileName = std::string(CISTIB_TOOLKIT_FOLDER) + vtkFileNameEnd;

        // Check if the dicom file exists
        std::ifstream ifs1( dcmFileName.c_str() );
        TSM_ASSERT( "The input DICOM file does not exist.", !ifs1.fail() );
        if( ifs1.fail() ) return false;
        ifs1.close();
        
        // Check if the vtk file exists
        std::ifstream ifs2( vtkFileName.c_str() );
        TSM_ASSERT( "The input VTK file does not exist.", !ifs2.fail() );
        if( ifs2.fail() ) return false;
        ifs2.close();

        // Read using dcmAPI
        vtkSmartPointer<vtkImageData> vtkDicomImage =
            dcmAPI::ImageUtilities::ReadVtkImageFromFile< unsigned short, 3 >( dcmFileName.c_str(), true );

        // Read using BaseLib
        vtkSmartPointer<vtkImageData> vtkImage = blImageUtils::LoadImageFromFileAsVTK( vtkFileName.c_str() );

        // Compare image content
        return blImageUtils::CompareImages( vtkDicomImage, vtkImage, 1e-5 );
	}
	catch(...)
	{
		TS_FAIL("ReadOrientationTest::ReadAndCompare failed: thrown exception.");
		return false;
	}
}

void ReadOrientationTest::TestReadAxial()
{
    TSM_ASSERT( "Axial images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

void ReadOrientationTest::TestReadAxial2()
{
    TSM_ASSERT( "Axial2 images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial2/color-a2.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

void ReadOrientationTest::TestReadCoronal()
{
    TSM_ASSERT( "Coronal images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/coronal/color-c1.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

void ReadOrientationTest::TestReadCoronal2()
{
    TSM_ASSERT( "Coronal2 images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/coronal2/color-c2.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

void ReadOrientationTest::TestReadSagittal()
{
    TSM_ASSERT( "Sagittal images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/sagittal/color-s1.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

void ReadOrientationTest::TestReadSagittal2()
{
    TSM_ASSERT( "Sagittal2 images are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/sagittal2/color-s2.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk" ) );
}

