/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "ReadSiemensTest.h"

// VTK
#include "vtkSmartPointer.h"
#include "vtkImageClip.h"
// DcmAPI
#include "dcmDataSetReader.h"
#include "dcmMultiSliceReader.h"
#include "dcmStandardImageReader.h"
// Predefined paths
#include "CISTIBToolkit.h"
// Other
#include "blVTKHelperTools.h"

void ReadSiemensTest::TestReadSiemensNuclearModality()
{
	// data to compare in test
	const unsigned int numberOfSlices = 49;
	const int numberOfRowsInSlice = 128; 
	const int numberOfColsInSlice = 128;
	const int numberOfFrames = 49;
	const double xPixelSpacing =  3.8953697681427002;
	const double yPixelSpacing =  3.8953697681427002;
	const double zPixelSpacing = 3.8953697681427002;
	const unsigned int numberOfVolumes = 1;

	// data path
	const std::string filePath = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DcmAPI/siemens/Siemens_NM/e.soft SIEMENS3.dcm";
	
	// Check if the file exists
	std::ifstream ifs( filePath.c_str() );
	TSM_ASSERT( "The input file does not exist.", !ifs.fail() );
	ifs.close();

	// read the structure of the file
	dcmAPI::DataSetReader::Pointer dataSetReader = dcmAPI::DataSetReader::New( );
    dcmAPI::StandardImageReader::Pointer standardReaderPtr( new dcmAPI::StandardImageReader() );
    dataSetReader->AddReader(standardReaderPtr);
	try
	{
	    dataSetReader->ReadDcmFile( filePath );
    }
    catch ( const std::exception& e )
    {
        TSM_ASSERT( e.what(), false );
    }
	
	// compare number of slices
	TS_ASSERT_EQUALS(dataSetReader->GetDataSet()->GetNumberOfSlices(), numberOfSlices);

	// read multislice image 
	std::vector< vtkSmartPointer<vtkImageData> > vtkVolumesVector;
	dcmAPI::MultiSliceReader::Pointer reader = dcmAPI::MultiSliceReader::New();
    reader->AddReader(standardReaderPtr);
	reader->SetPath( filePath );
	reader->SetDataSet( dataSetReader->GetDataSet() );
	try
	{
	    reader->Update( );
    }
    catch ( const std::exception& e )
    {
        TSM_ASSERT( e.what(), false );
    }
	vtkVolumesVector = reader->GetOutput( );
	
	// compare number of slices
	TS_ASSERT_EQUALS(vtkVolumesVector.size(), numberOfVolumes);

	vtkSmartPointer<vtkImageData> volume = vtkSmartPointer<vtkImageData>::New() ;
	volume = vtkVolumesVector.at(0);
	
	// compare dimensions
	int dim[3];
	volume->GetDimensions(dim);
	TS_ASSERT_EQUALS(dim[0], numberOfColsInSlice);
	TS_ASSERT_EQUALS(dim[1], numberOfRowsInSlice);
	TS_ASSERT_EQUALS(dim[2], numberOfFrames);
	// compare spacings
	double spacing[3];
	volume->GetSpacing(spacing);
	TS_ASSERT_EQUALS(spacing[0], xPixelSpacing);
	TS_ASSERT_EQUALS(spacing[1], yPixelSpacing);
	TS_ASSERT_EQUALS(spacing[2], zPixelSpacing);

	// get 2D slice from the volume
	unsigned int sliceNr = dataSetReader->GetDataSet()->GetNumberOfSlices()/2 ;
	vtkSmartPointer < vtkImageClip > imageClip = vtkSmartPointer< vtkImageClip >::New() ;
	imageClip->SetInput(volume);
	imageClip->ClipDataOn();
	imageClip->SetOutputWholeExtent(0, dim[0], 0, dim[1], sliceNr, sliceNr);
	imageClip->Update();
	vtkSmartPointer<vtkImageData> slice = vtkSmartPointer<vtkImageData>::New() ;
	slice = imageClip->GetOutput();

	// compare dimensions
	slice->GetDimensions(dim);
	TS_ASSERT_EQUALS(dim[0], numberOfColsInSlice);
	TS_ASSERT_EQUALS(dim[1], numberOfRowsInSlice);
}

