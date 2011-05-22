// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "meMeshFiltersTest.h"
#include "blShapeUtils.h"
#include "CISTIBToolkit.h"
#include "vtkSmartPointer.h"
#include "meVTKExtractMainSurfaceFilter.h"
#include "meVTKSmoothFilter.h"
#include "meVTKVolumeClosingFilter.h"
#include "meVTKTetraGenerationFilter.h"
#include "meVTKLoopSubdivisionRefinerFilter.h"
#include "meVTKLocalRefinerFilter.h"
#include "meVTKEdgeSwappingFilter.h"
#include "meVTKSkeletonizationFilter.h"
#include "meVTKVolumeClippingFilter.h"

#include "vtkCellData.h"

void meMeshFiltersTest::TestExtractMainSurface()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout<< std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/GARmesh.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/ExtractedMainSurfaceRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );
	vtkSmartPointer<meVTKExtractMainSurfaceFilter> filter = 
		vtkSmartPointer<meVTKExtractMainSurfaceFilter>::New();
	filter->SetInput(polyData);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Extracted Main Surface : " << res << std::endl;
}

void meMeshFiltersTest::TestVolumeClosing()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu_cut.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/ClosedSurfaceRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	//outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	meMeshVolumeClosingParamsPtr parameters = meMeshVolumeClosingParamsPtr(new meMeshVolumeClosingParams);
	vtkSmartPointer<meVTKVolumeClosingFilter> filter = 
		vtkSmartPointer<meVTKVolumeClosingFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams(parameters);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	blShapeUtils::ShapeUtils::SaveShapeToFile(filter->GetOutput(), outfilename.c_str());
	/*float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
 	TS_ASSERT( res );
 	std::cout << "Volume Closing : " << res << std::endl;*/

}

void meMeshFiltersTest::TestSmoothingTaubin()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/TaubinSmoothedRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	meMeshSmoothingParamsPtr parameters = meMeshSmoothingParamsPtr(new meMeshSmoothingParams);
	parameters->m_Type = MESH_SMOOTHING_TYPE_TAUBIN;
	vtkSmartPointer<meVTKSmoothFilter> filter = vtkSmartPointer<meVTKSmoothFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams( parameters );
	filter->Update();
	
	TS_ASSERT( filter->GetOutput() != NULL);
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Taubin Smoothed Surface : " << res << std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu_select.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/TaubinSmoothedRefSelect.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );
	filter->SetInput(polyData);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Taubin Smoothed Surface with Selection : " << res << std::endl;
}	

void meMeshFiltersTest::TestSmoothingUmbrella()
{
	vtkSmartPointer<vtkPolyData>		polyData;
	std::string filename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	TS_ASSERT( polyData != NULL );
	std::cout << "polyData: " << polyData << std::endl;
}

void meMeshFiltersTest::TestLoopSubdivisionRefiner()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/Sphere.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/LoopSubdivisionRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	meMeshLoopSubdivisionParamsPtr parameters = meMeshLoopSubdivisionParamsPtr(
				new meMeshLoopSubdivisionParams);
	parameters->m_NumberOfSubdivisions= 5;
	vtkSmartPointer<meVTKLoopSubdivisionRefinerFilter> filter = 
		vtkSmartPointer<meVTKLoopSubdivisionRefinerFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams(parameters);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
 	float difference;
 	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
 	TS_ASSERT( res );
 	std::cout << "Loop Subdivision Refiner Surface : " << res << std::endl;
}

void meMeshFiltersTest::TestLocalRefiner()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/Cube.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/LocalRefinedRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	/*meMeshLocalRefinerParamsPtr parameters = 
		meMeshLocalRefinerParamsPtr(new meMeshLocalRefinerParams);
	parameters->m_DensityFactor = 1.414;
	parameters->m_MinimumEdgesSwapped =10;
		
	vtkSmartPointer<meVTKLocalRefinerFilter> filter = 
		vtkSmartPointer<meVTKLocalRefinerFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams( parameters );
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Local Refined Surface : " << res << std::endl;*/
}

void meMeshFiltersTest::TestEdgeSwapping()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/Sphere.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/EdgeSwappedRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	meMeshEdgeSwappingParamsPtr parameters = 
		meMeshEdgeSwappingParamsPtr(new meMeshEdgeSwappingParams);

	vtkSmartPointer<meVTKEdgeSwappingFilter> filter = 
		vtkSmartPointer<meVTKEdgeSwappingFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams( parameters );
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Edge Swapped Surface : " << res << std::endl;
}

void meMeshFiltersTest::TestTetraGeneration()
{
	vtkSmartPointer<vtkPolyData>		polyData;
	vtkSmartPointer< vtkUnstructuredGrid> unstructuredGrid;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	//filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/Volume.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	//unstructuredGrid = blShapeUtils::ShapeUtils::LoadVolumeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	/*meMeshTetraGenerationParamsPtr parameters = 
		meMeshTetraGenerationParamsPtr(new meMeshTetraGenerationParams);
	vtkSmartPointer<meVTKTetraGenerationFilter> filter = 
		vtkSmartPointer<meVTKTetraGenerationFilter>::New();
	filter->SetInput(polyData);
	filter->SetParams( parameters );
	filter->Update();*/

	//TS_ASSERT( filter->GetOutput() != NULL);
	//float difference;
	//blShapeUtils::ShapeUtils::SaveVolumeToFile(filter->GetOutput(),outfilename.c_str());
	//bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	//TS_ASSERT( res );
	//std::cout << "Taubin Smoothed Surface : " << res << std::endl;
}

void meMeshFiltersTest::TestSkeletonization()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	std::string filename, outfilename;
	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/Skeleton.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	vtkSmartPointer<meVTKSkeletonizationFilter> filter = 
		vtkSmartPointer<meVTKSkeletonizationFilter>::New();
	filter->SetInput(polyData);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	//blShapeUtils::ShapeUtils::SaveShapeToFile(filter->GetOutput(),outfilename.c_str() );
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Skeletonization : " << res << std::endl;
}

void meMeshFiltersTest::TestVolumeClipping()
{
	vtkSmartPointer<vtkPolyData>		polyData, outputPoly;
	vtkSmartPointer<vtkImplicitFunction>		implicit;
	std::string filename, outfilename;

	std::cout <<std::endl;

	filename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/blShapeUtils/aneu.vtk";
	outfilename = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/MeshLib/ClippedRef.vtk";
	polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile( filename.c_str() );
	outputPoly = blShapeUtils::ShapeUtils::LoadShapeFromFile( outfilename.c_str() );
	TS_ASSERT( polyData != NULL );

	vtkSmartPointer<meVTKVolumeClippingFilter> filter = 
		vtkSmartPointer<meVTKVolumeClippingFilter>::New();
	filter->SetInput(polyData);
	filter->SetImplicitFunction(implicit, true);
	filter->Update();

	TS_ASSERT( filter->GetOutput() != NULL);
	float difference;
	bool res = blShapeUtils::ShapeUtils::CompareShapes(filter->GetOutput(), outputPoly, difference);
	TS_ASSERT( res );
	std::cout << "Clipped Surface : " << res << std::endl;

}
