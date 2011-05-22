// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blShapeUtilsTest.h"
#include "blShapeUtils.h"
#include "CISTIBToolkit.h"
#include "vtkSmartPointer.h"

void blShapeUtilsTest::TestLoadVTPShapeFromFile()
{
	// Cube.vtp is a polydata
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/Cube.vtp";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	if( polyData == NULL )
	{
	   std::cout << "Loading VTP polydata file: " << filename << std::endl;
	}
	TS_ASSERT( polyData != NULL );
	polyData->Delete();
}

void blShapeUtilsTest::TestLoadVTKShapeFromFile()
{
	// TetraMeshLV.vtk isn't a polydata
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/TetraMeshLV.vtk";
	
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	if( polyData != NULL )
	{
	   std::cout << "Loading Unstructured grid file: " << filename << std::endl;
	}
	TS_ASSERT( polyData == NULL );
	
	vtkUnstructuredGrid* unstructuredGrid = blShapeUtils::ShapeUtils::LoadVolumeFromFile(filename.c_str());
	if( unstructuredGrid == NULL )
	{
	   std::cout << "Loading Unstructured grid file: " << filename << std::endl;
	}
	TS_ASSERT( unstructuredGrid != NULL );
	unstructuredGrid->Delete();
}

void blShapeUtilsTest::TestLoadVTKPolyShapeFromFile()
{
	// aneu.vtk is a polydata
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	if( polyData == NULL )
	{
	   std::cout << "Loading VTK polydata file: " << filename << std::endl;
	}
	TS_ASSERT( polyData != NULL );
	polyData->Delete();
}

void blShapeUtilsTest::TestLoadStlShapeFromFile()
{
	// LV_Myo_opt.stl is a polydata
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/vessel.stl";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	if( polyData == NULL )
	{
	   std::cout << "Loading STL polydata file: " << filename << std::endl;
	}
	TS_ASSERT( polyData != NULL );
	polyData->Delete();
}


void blShapeUtilsTest::TestComputeVolume()
{
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/Cube.vtk";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	
	const double volume = blShapeUtils::ShapeUtils::GetClosedSurfaceVolume( polyData, false, false);
	TS_ASSERT_DELTA( volume, 1.0, 1e-6 );
   
   polyData->Delete();
}

void blShapeUtilsTest::TestComputeNumberOfHoles()
{
	const std::string filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/vessel.stl";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );

	const unsigned int holes = blShapeUtils::ShapeUtils::ComputeNumberOfHoles( polyData);
	TS_ASSERT_EQUALS( holes,6);
	
	polyData->Delete();
}