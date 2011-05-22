/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencilData.h>
#include <vtkImageStencil.h>
#include <vtkImageData.h>
#include <vtkStructuredPointsWriter.h>
#include <string.h>

int main( int argc, char* argv[] )
{
	double bounds[6]; // Mesh bounding box
	double spacing[3]; // Output image voxel spacing
	double origin[3]; // Output image origin
	int extent[6]; // Output image size in voxels (from..to voxel index, hence the 6 elements)

	// Default spacing
	spacing[0] = 1.0;
	spacing[1] = 1.0;
	spacing[2] = 1.0;

	if( argc < 2 )
	{
		std::cerr
			<< "Error: Not enough arguments." << std::endl
			<< std::endl
			<< "Usage:" << std::endl
			<< argv[0] << " [polyData] [XYZspacing]" << std::endl
			<< argv[0] << " [polyData] [XYspacing] [Zspacing]" << std::endl
			<< argv[0] << " [polyData] [Xspacing] [Yspacing] [Zspacing]" << std::endl
			<< std::endl
			<< "Abnormal program termination." << std::endl;
		exit( 1 );
	}

	std::string filename = argv[1];

	if( argc == 3 ) // One argument: isotropic voxels
	{
		spacing[0] = atof( argv[2] );
		spacing[1] = spacing[0];
		spacing[2] = spacing[0];
	}
	if( argc == 4 ) // Two arguments: in-plane isotropy, out-of plane anisotropy
	{
		spacing[0] = atof( argv[2] );
		spacing[1] = spacing[0];
		spacing[2] = atof( argv[3] );
	}
	if( argc == 5 ) // Three arguments: total anisotropy
	{
		spacing[0] = atof( argv[2] );
		spacing[1] = atof( argv[3] );
		spacing[2] = atof( argv[4] );
	}

	vtkXMLPolyDataReader* dataReader			= vtkXMLPolyDataReader::New();
	vtkPolyData* shape							= vtkPolyData::New();
	vtkPolyDataToImageStencil* stencilFilter	= vtkPolyDataToImageStencil::New();
	vtkImageStencilData* stencilData			= vtkImageStencilData::New();
	vtkImageStencil* imageStencil				= vtkImageStencil::New();
	vtkImageData* outputImage					= vtkImageData::New();
	vtkStructuredPointsWriter* spWriter			= vtkStructuredPointsWriter::New();

	if( !dataReader->CanReadFile( filename.c_str() ) )
	{
		std::cerr
			<< "Error: could not read data file:" << std::endl
			<< argv[1] << std::endl
			<< "Abnormal program termination." << std::endl;
		exit( 1 );
	}
	dataReader->SetFileName( filename.c_str() );
	dataReader->Update();

	shape = dataReader->GetOutput();
	shape->GetBounds( bounds );

	// Set the output image origin and size in voxels; the size is
	// now by default set to the mesh bounding box plus two full
	// voxels on each side.
	origin[0] = bounds[0] - 2 * spacing[0];
	origin[1] = bounds[2] - 2 * spacing[1];
	origin[2] = bounds[4] - 2 * spacing[2];
	extent[0] = 0;
	extent[1] = (bounds[1] - origin[0]) / spacing[0] + 2;
	extent[2] = 0;
	extent[3] = (bounds[3] - origin[1]) / spacing[1] + 2;
	extent[4] = 0;
	extent[5] = (bounds[5] - origin[2]) / spacing[2] + 2;

	stencilFilter->SetInput( shape );
	stencilFilter->SetOutputOrigin( origin );
	stencilFilter->SetOutputSpacing( spacing );
	stencilFilter->SetOutputWholeExtent( extent );
	stencilFilter->Update();

	outputImage->SetOrigin( origin );
	outputImage->SetSpacing( spacing );
	outputImage->SetExtent( extent );
	outputImage->SetScalarTypeToUnsignedChar();
	outputImage->SetNumberOfScalarComponents( 1 );
	outputImage->AllocateScalars();

	imageStencil->SetInput( outputImage );
	imageStencil->SetStencil( stencilFilter->GetOutput() );

	// A problem regarding a difference in behavior of the debug
	// and release modes arose here; the stencilFilter produces
	// 0 values for voxels outside the mesh in debug mode, while
	// it produces 0 values for voxels INSIDE the mesh in release
	// mode.
//	imageStencil->SetBackgroundValue( 0 ); // in debug mode, this produces white meshes on a black background
	imageStencil->SetBackgroundValue( 255 ); // in release mode, this produces black meshes on a white background
	imageStencil->Update();

	outputImage = imageStencil->GetOutput();

	spWriter->SetFileTypeToBinary();
	spWriter->SetInput( outputImage );
	spWriter->SetFileName( (filename+".vtk").c_str() );
	spWriter->Write();
}
