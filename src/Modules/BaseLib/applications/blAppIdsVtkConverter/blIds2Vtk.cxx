/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blIDSFileReader.h"
#include "blITKImageUtils.h"

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

		//base::UnsignedShortVolumeType::Pointer imageOutput;
		blIDSFileReader::Pointer reader = blIDSFileReader::New();
		reader->SetFileName( argv[1] );
		reader->Update();
		
		
		blITKImageUtils::SaveImageToFile<base::UnsignedShortVolumeType>( 
			reader->GetOutput(),
			argv[2]); 
	
	}
	catch (...)
	{
		std::cout << "Conversion Failed..." << std::endl;
	}

	return 0;
}




