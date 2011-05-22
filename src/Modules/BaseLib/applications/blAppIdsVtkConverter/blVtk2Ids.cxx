/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blIDSFileWriter.h"
#include "blImageUtils.h"
#include "vtkImageReader.h"


#include <stdio.h>

/**
The following example illustrates 
*/
int main( int argc, char* argv[] )
{

	if( argc < 2 )
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " inputFilename  outputFilename " 
			<< std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		// Read the .vtk input file
		vtkSmartPointer<vtkImageReader> reader = vtkSmartPointer<vtkImageReader>::New();
		reader->SetFileName( argv[1] );
		reader->Update();

		//vtkStructuredPoints* aux  = blImageUtils::LoadImageFromFileAsVTK(argv[1]);
		vtkImageData* aux = blImageUtils::LoadITKImage(argv[1]);

		blIDSFileWriter::Pointer writer = blIDSFileWriter::New();
		writer->SetFileName(argv[2]);
		writer->SetInput (/*reader->GetOutput()*/aux);
		writer->Update();
	
	}
	catch(std::exception& e ){ 
		std::cout << e.what() << std::endl;
	}

	return 0;
}




