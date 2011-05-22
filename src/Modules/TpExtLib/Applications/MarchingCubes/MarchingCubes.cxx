/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "MarchingCubesCLP.h"
#include "vtkMarchingCubes.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredPointsReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkStructuredPoints.h"

int main( int argc, char *argv[] )
{
	PARSE_ARGS;

	try
	{

		vtkSmartPointer<vtkStructuredPointsReader> reader;
		reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
		reader->SetFileName( inputVolume.c_str( ) );
		reader->Update();

		vtkSmartPointer<vtkMarchingCubes> mcubes;
		mcubes = vtkSmartPointer<vtkMarchingCubes>::New();
		mcubes->SetInput( reader->GetOutput() );
		mcubes->SetComputeScalars( ComputeScalars );
		mcubes->SetComputeGradients( ComputeGradients );
		mcubes->SetComputeNormals( ComputeNormals );
		mcubes->SetValue( 0, Value );
		mcubes->Update();

		vtkSmartPointer<vtkPolyDataWriter> writer;
		writer = vtkSmartPointer<vtkPolyDataWriter>::New();
		writer->SetFileName( outputMesh.c_str( ) );
		writer->SetInput( mcubes->GetOutput( ) );
		writer->Write();
	}
	catch (vtkstd::exception& e)
	{
		std::cerr << argv[0] << ": exception caught !" << std::endl;
		std::cerr << e.what( ) << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
