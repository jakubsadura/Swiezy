/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginShapeScaleProcessor.h"

#include <string>

#include "vtkPolyData.h"

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"
#include "coreVTKPolyDataHolder.h"

#include "blVTKHelperTools.h"

SandboxPlugin::ShapeScaleProcessor::ShapeScaleProcessor( )
{
	SetName( "ShapeScaleProcessor" );

	BaseProcessor::SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Surface mesh" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );

	BaseProcessor::SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );

	m_ParametersHolder = Core::DataHolder<float>::New();
	m_ParametersHolder->SetSubject( 0.5 );
}

SandboxPlugin::ShapeScaleProcessor::~ShapeScaleProcessor()
{
}

void SandboxPlugin::ShapeScaleProcessor::Update()
{
	// Get the mesh 
	Core::vtkPolyDataPtr vtkInputMesh;
	GetProcessingData( 0, vtkInputMesh );

	// Call the function
	Core::vtkPolyDataPtr vtkOutputMesh = Core::vtkPolyDataPtr::New();
	blVTKHelperTools::ScaleShape( 
		vtkInputMesh, 
		vtkOutputMesh, 
		GetParametersHolder()->GetSubject( ) );

	// Set the output to the output of this processor
	UpdateOutput( 
		0, 
		vtkOutputMesh, 
		"ScaleShape",
		true, 
		1,
		GetInputDataEntity( 0 ) );
		
}

SandboxPlugin::ShapeScaleProcessor::ParametersHolder::Pointer 
SandboxPlugin::ShapeScaleProcessor::GetParametersHolder() const
{
	return m_ParametersHolder;
}

