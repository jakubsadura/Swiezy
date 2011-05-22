/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "ModuleDescriptionParser.h"
#include "ModuleDescription.h"
#include "ModuleFactory.h"
#include "dynFilter.h"
#include "dynModuleExecution.h"

#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkStructuredPoints.h"
#include "vtkImageThreshold.h"
#include "vtkSmartPointer.h"

#include "itksys/SystemTools.hxx"

#ifdef _WIN32
  const char* filename =  "I:/Data/SmallImage.vtk";
  const char* outfilenameVtk = "I:/Data/outImageVtk.vtk";
  const char* outfilenameModule = "I:/Data/outImageModule.vtk";
  const char* outfilenameCLP = "I:/Data/outImageCLP.vtk";
#else
  const char* filename = "/home/cistib/demo-MRI-head.vtk";
  const char* outfilenameVtk = "/home/cistib/outImageVtk.vtk";
  const char* outfilenameModule = "/home/cistib/outImageModule.vtk";
  const char* outfilenameCLP = "/home/cistib/outImageCLP.vtk";
#endif

vtkSmartPointer<vtkImageData> ExecuteFilterVtk( vtkStructuredPoints* image )
{
	vtkImageThreshold* vtkFilter = vtkImageThreshold::New();
	vtkFilter->SetInput( image );
	vtkFilter->ThresholdByUpper( 10 );
	vtkFilter->ThresholdByLower( 5 );
	vtkFilter->SetOutValue( 0 );
	vtkFilter->Update();
	return vtkFilter->GetOutput( 0 );
}

void ExecuteFilterModule( 
	vtkStructuredPoints* image,
	ModuleDescription &module,
	vtkSmartPointer<vtkImageData> &outImage )
{
	dynModuleExecution::Pointer moduleExecution = dynModuleExecution::New();
	moduleExecution->SetModule( &module );
	ModuleParameter* paramIn = moduleExecution->FindParameter( "input", 0 );
	paramIn->SetDefault( dynModuleExecutionImpl::PointerToString( image ) );
	moduleExecution->Update();
	ModuleParameter* paramOut = moduleExecution->FindParameter( "output", 0 );
	outImage = reinterpret_cast<vtkImageData*> ( dynModuleExecutionImpl::StringToPointer( paramOut->GetDefault( ) ) );
}

void ExecuteCLPModule( 
						 std::string filename,
						 ModuleDescription &module,
						 std::string outFilename )
{
	dynModuleExecution::Pointer moduleExecution = dynModuleExecution::New();
	moduleExecution->SetModule( &module );
	ModuleParameter* paramIn = moduleExecution->FindParameter( "input", 0 );
	paramIn->SetDefault( filename );
	ModuleParameter* paramOut = moduleExecution->FindParameter( "output", 0 );
	paramOut->SetDefault( outFilename );
	moduleExecution->Update();
}

int main (int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: path filterName" << std::endl;
		return EXIT_FAILURE;
	}

	ModuleFactory factory;
	factory.SetSearchPaths( argv[1] );
	factory.Scan( );

	ModuleDescription module = factory.GetModuleDescription( argv[2] );

	try
	{
		vtkStructuredPoints* image;
		vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
		reader->SetFileName( filename );
		reader->Update();
		image = reader->GetOutput();

		vtkStructuredPointsWriter* writer;
		writer = vtkStructuredPointsWriter::New();
		
		vtkSmartPointer<vtkImageData> outImageVtk = ExecuteFilterVtk( image );
		writer->SetFileName( outfilenameVtk );
		writer->SetInput( outImageVtk );
		writer->Update();

		//vtkSmartPointer<vtkImageData> outImageModule;
		//ExecuteFilterModule( image, module, outImageModule );
		//writer->SetFileName( outfilenameModule );
		//writer->SetInput( outImageModule );
		//writer->Update();

		vtkSmartPointer<vtkImageData> outImageModule;
		ExecuteCLPModule( filename, module, outfilenameCLP );

	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
	}


	return EXIT_SUCCESS;
}


