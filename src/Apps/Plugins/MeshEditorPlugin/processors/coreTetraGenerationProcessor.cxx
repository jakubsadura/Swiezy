/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreTetraGenerationProcessor.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityHelper.h"
#include "coreVTKUnstructuredGridHolder.h"

Core::TetraGenerationProcessor::TetraGenerationProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output surface" );
	SetName( "TetraGenerationProcessor" );

	m_FilterTet = vtkSmartPointer<meVTKTetraGenerationFilter>::New();
	m_Filter = meNGTetraFilter::New();
	m_type = false; // true: netgen, false: tetgen
}

void Core::TetraGenerationProcessor::Update()
{
	Core::vtkPolyDataPtr polyInit;
	GetProcessingData( 0, polyInit);
	Core::vtkPolyDataPtr temp = Core::vtkPolyDataPtr::New();
	temp->DeepCopy(polyInit);

	if (!CheckInput())
	{
		throw Core::Exceptions::Exception( 
			"TetraGenerationProcessor::Update", 
			"Input Mesh has holes!" );
	}

	if (m_type)
	{
		meNetgenMesh::Pointer netgenMesh;
		GetProcessingData( 0, netgenMesh);
	
		Core::vtkUnstructuredGridPtr volume;

		m_Filter->SetInput( netgenMesh );

		m_Filter->Update();
		
		volume = m_Filter->GetVTK();


		UpdateOutput(0,
			volume,
			"Tetra Netgen generated",
			false,
			1,
			GetInputDataEntity(0));
	}
	else
	{
		
		if (m_FilterTet->GetParams()->m_AspectRatio != 0 &&
			m_FilterTet->GetParams()->m_AspectRatio  < 1.0)
		{
			throw Core::Exceptions::Exception( 
				"TetraGenerationProcessor::Update", 
				"Aspect Ration cannot be lower than 1.0" );
		}
		Core::vtkPolyDataPtr poly;
		GetProcessingData( 0, poly);

		m_FilterTet->SetInput( poly);

		m_FilterTet->Update();

		Core::vtkUnstructuredGridPtr volume;
		volume = m_FilterTet->GetVtkOutput();

		UpdateOutput(0,
			volume,
			"Tetra Tetgen generated",
			false,
			1,
			GetInputDataEntity(0));
	}

	Core::vtkPolyDataPtr poly;
	GetProcessingData( 0, poly);
	poly->DeepCopy(temp);
}

void Core::TetraGenerationProcessor::SetParams(meMeshTetraGenerationParamsPtr param)
{
	m_FilterTet->SetParams(param);
}

void Core::TetraGenerationProcessor::SetNetgenParams( meMeshParamsOptimizePtr param )
{
	m_Filter->SetOptParams(param);
}
meMeshTetraGenerationParamsPtr Core::TetraGenerationProcessor::GetParams()
{
	return m_FilterTet->GetParams( );
}

meMeshParamsOptimizePtr Core::TetraGenerationProcessor::GetNetgenParams()
{
	return m_Filter->GetOptParams();
}
void Core::TetraGenerationProcessor::SetFlag( bool val )
{
	m_type = val;
}

bool Core::TetraGenerationProcessor::GetFlag()
{
	return m_type;
}

bool Core::TetraGenerationProcessor::CheckInput()
{
	Core::vtkPolyDataPtr poly;
	GetProcessingData( 0, poly);
	int numberOfHoles = blShapeUtils::ShapeUtils::ComputeNumberOfHoles(poly);
	if (numberOfHoles != 0)
		return false;

	return true;
	

}