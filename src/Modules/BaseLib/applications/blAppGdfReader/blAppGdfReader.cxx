/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blGDFFileReader.h"
#include "blGDFFileWriter.h"

#include "blSignalCollective.h"
#include "blSignalWriter.h"

#include <stdio.h>
#include <iostream>

/**
The following example illustrates 
*/
int main( int argc, char* argv[] )
{

	if( argc < 2 )
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " filename  outputFileName " 
			<< std::endl;
		return EXIT_FAILURE;
	}

//	try
//	{

		long x = 0x34333231;
		char *y = (char *) &x;

		if(strncmp(y,"1234",4))
		printf("Big Endian");
		else
		printf("little Endian");

		blGDFFileReader::Pointer reader = blGDFFileReader::New();
		reader->SetFilename( argv[1] );
		reader->Update();
		
		blSignalCollective::Pointer sigcoll = reader->GetOutput();
		blSignalWriter::Pointer sigwriter = blSignalWriter::New();
		sigwriter->SetFilename("C://testgdf.csv");
		sigwriter->SetInput(sigcoll);
		sigwriter->Update();

		blGDFFileWriter::Pointer writer = blGDFFileWriter::New();
		writer->SetFilename("C://testwriter.gdf");
		writer->SetInput (sigcoll);
		writer->Update();
	
//	}
//	catch{}

	return 0;
}




