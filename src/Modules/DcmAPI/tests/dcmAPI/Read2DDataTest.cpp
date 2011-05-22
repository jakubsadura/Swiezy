/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "Read2DDataTest.h"

#include <itkImageToVTKImageFilter.h>
#include "dcmImageUtilities.h"
// Predefined paths
#include "CISTIBToolkit.h"

bool Read2DDataTest::ReadAndCompare( 
      const std::string& dcmInputNameEnd, 
      const std::string& vtkFileNameEnd ) const
{
    try
    {
        // Working directory of the test
        const std::string dcmFileName = std::string(CISTIB_TOOLKIT_FOLDER) + dcmInputNameEnd;
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
            dcmAPI::ImageUtilities::Read2DVtkImageFromFile< unsigned short >( dcmFileName.c_str() );

        // Read using BaseLib
        vtkSmartPointer<vtkImageData> vtkImage = blImageUtils::LoadImageFromFileAsVTK( vtkFileName.c_str() );

        // Compare image content
        return blImageUtils::CompareImages( vtkDicomImage, vtkImage, 1e-5 );
    }
    catch(...)
    {
        TS_FAIL("Read2DDataTest::TestReadImage failed: exception was thrown");
        return false;
    }
}

void Read2DDataTest::TestReadSingleSliceFile()
{
    TSM_ASSERT( "Images (from single slice file) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/files/color-a1_000.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1_000.vtk") );
}

void Read2DDataTest::TestReadRescale()
{
    TSM_ASSERT( "Images (from single slice file) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/rescale/color-rescale_000.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/rescale/color-rescale_000.vtk") );
}

