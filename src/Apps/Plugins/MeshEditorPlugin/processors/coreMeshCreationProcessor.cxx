/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMeshCreationProcessor.h"
#include "coreDataContainer.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityList.h"
#include "coreDataEntityUtilities.h"
#include "coreLoggerHelper.h"
#include "coreDataEntityHelper.h"

#include "itkOtsuThresholdImageFilter.h"

#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkMarchingCubes.h"
#include "vtkCleanPolyData.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkDelaunay3D.h"
#include "vtkGeometryFilter.h"

#include "mitkImage.h"
#include "mitkImageWriter.h"
#include "mitkImageDataItem.h"

#include "itkSmartPointerForwardReference.txx"

using namespace Core;

//!
MeshCreationProcessor::MeshCreationProcessor(void)
{
	m_bDelaunay = false;
	m_bMarchingCubes = false;
	SetNumberOfInputs(1);
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId | Core::ROITypeId );
	GetInputPort( 0 )->SetName( "Input Mask Image" );
	SetNumberOfOutputs(1);
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Output mesh" );
	GetOutputPort( 0 )->SetDataEntityName( "Output mesh" );
}

//!
MeshCreationProcessor::~MeshCreationProcessor(void)
{
}

//!
void MeshCreationProcessor::Update( )
{

	if (!( GetInputDataEntity(0)->GetType() & Core::ROITypeId ))
	{
		throw Core::Exceptions::Exception(
			"MeshCreationProcessor::Update",
			"You must select a Volume ROI Image input from the Processing browser" );
	}

	std::string name;
	if (m_bMarchingCubes) name = "Marching Cubes Mesh";
	else if (m_bDelaunay) name = "Delaunay Mesh";

	Core::DataEntity::Pointer dataEntityMesh;
	dataEntityMesh = Core::DataEntity::New( Core::SurfaceMeshTypeId );
	dataEntityMesh->GetMetadata()->SetName( name );
	dataEntityMesh->SetFather( GetInputDataEntity( 0 ) );
	dataEntityMesh->Resize( GetInputDataNumberOfTimeSteps( 0 ), typeid( vtkPolyDataPtr ) );
	SetOutputDataEntity(0,dataEntityMesh);

	for ( unsigned i = 0 ; i < GetInputDataNumberOfTimeSteps( 0 ) ; i++ )
	{

		vtkImageDataPtr inputImage;
		Core::DataEntityHelper::GetProcessingData(
			GetInputDataEntityHolder(0),
			inputImage, 
			i);

		Core::vtkPolyDataPtr mesh;
		Core::DataEntityHelper::GetProcessingData(
			GetOutputDataEntityHolder(0),
			mesh,
			i );

		GetSurfaceFromSegmentation( inputImage, mesh );

	}

}

//!
void MeshCreationProcessor::GetSurfaceFromSegmentation( 
	vtkImageDataPtr inputImage,
	vtkPolyDataPtr surface )
{

	//The filter runs faster if gradient and normal calculations are turned off.
	vtkSmartPointer<vtkMarchingCubes> mcubes = vtkMarchingCubes::New();
	mcubes->SetInput( inputImage );
	mcubes->ComputeScalarsOff();
	mcubes->ComputeGradientsOff();
	mcubes->ComputeNormalsOff();
	mcubes->SetValue( 0, 0.5 );
	mcubes->Update();

	if ( mcubes->GetOutput()->GetNumberOfPolys() == 0) {
		surface->DeepCopy( mcubes->GetOutput() );
		mcubes->Delete();
		return;
	}

	vtkSmartPointer<vtkPolyData> algoOutput = vtkPolyData::New();
	
	if (m_bDelaunay)
	{
		vtkSmartPointer<vtkDelaunay3D> delaunay = vtkSmartPointer<vtkDelaunay3D>::New();
		//	delaunay->SetInput( normals->GetOutput() );
		delaunay->SetInput( mcubes->GetOutput() );
		delaunay->BoundingTriangulationOff();
		delaunay->Update();

		vtkSmartPointer<vtkGeometryFilter> gridToPoly = vtkSmartPointer<vtkGeometryFilter>::New();
		gridToPoly->SetInputConnection(delaunay->GetOutputPort());
		gridToPoly->Update();
		algoOutput->DeepCopy(gridToPoly->GetOutput());
	}
	
	if (m_bMarchingCubes)
	{
		algoOutput->DeepCopy(mcubes->GetOutput());
	}

	vtkCleanPolyData *cleanMesh = vtkCleanPolyData::New();
	cleanMesh->SetInput(algoOutput);
	cleanMesh->ConvertPolysToLinesOff();
	cleanMesh->SetTolerance (1.0e-10);
	cleanMesh->SetAbsoluteTolerance (1.0e-10);
	cleanMesh->Update();

	vtkSmartPointer<vtkPolyDataNormals> normals = vtkPolyDataNormals::New();
	normals->SetInput( cleanMesh->GetOutput() );
	normals->SplittingOff();	//to avoid duplicate vertices due to sharp edges
	normals->Update();


	surface->DeepCopy(normals->GetOutput());
}

/**
*/
void MeshCreationProcessor::SetDelaunayType( bool typeValue)
{
	m_bDelaunay = typeValue;
}

/**
*/
void MeshCreationProcessor::SetMarchingCubesType( bool typeValue)
{
	m_bMarchingCubes = typeValue;
}
