/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "MultiSliceReaderTest.h"

// DcmAPI
#include "dcmDataSetReader.h"
#include "dcmMultiSliceReader.h"
#include "dcmAbstractImageReader.h"
#include "dcmStandardImageReader.h"
// Predefined paths
#include "CISTIBToolkit.h"

void MultiSliceReaderTest::TestReadWithoutReader()
{
	// data path
	const std::string filePath = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DcmAPI/synthetic/colors/axial/files/color-a1_000.dcm";
	
	// Check if the file exists
	std::ifstream ifs( filePath.c_str() );
	TSM_ASSERT( "The input file does not exist.", !ifs.fail() );
	ifs.close();

	// read the structure of the file
	dcmAPI::DataSetReader::Pointer dataSetReader = dcmAPI::DataSetReader::New( );
    
    // Purposely not adding a reader
    //dcmAPI::StandardImageReader::Pointer standardReaderPtr( new dcmAPI::StandardImageReader() );
    //dataSetReader->AddReader(standardReaderPtr);
	
	TSM_ASSERT_THROWS_NOTHING("Should not throw.", dataSetReader->ReadDcmFile( filePath ));
	
	// read multislice image 
	std::vector< vtkSmartPointer<vtkImageData> > vtkVolumesVector;
	dcmAPI::MultiSliceReader::Pointer reader = dcmAPI::MultiSliceReader::New();
	
	// Purposely not adding a reader
    //reader->AddReader(standardReaderPtr);
	
	reader->SetPath( filePath );
	reader->SetDataSet( dataSetReader->GetDataSet() );
	
	TS_ASSERT_THROWS_EQUALS( reader->Update(), 
	    const dcmAPI::ImageReaderException &e, 
	    std::string(e.what()), "No Reader found for 'Standard' data." );
}

