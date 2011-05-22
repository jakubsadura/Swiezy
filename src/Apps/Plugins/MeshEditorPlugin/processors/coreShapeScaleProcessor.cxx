/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreShapeScaleProcessor.h"

#include <string>

#include "vtkPolyData.h"

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreVTKPolyDataHolder.h"

#include "blVTKHelperTools.h"

Core::ShapeScaleProcessor::ShapeScaleProcessor( )
{
	m_ParametersHolder = Core::DataHolder<float>::New();
	m_ParametersHolder->SetSubject( 0.5 );

	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Surface mesh" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetName( "Scaled Shape" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetDataEntityName( "Scaled Shape" );
}

Core::ShapeScaleProcessor::~ShapeScaleProcessor()
{
}

void Core::ShapeScaleProcessor::Update()
{
	try
	{
		// Set state to processing (dialog box)
		SetState( Core::Runtime::APP_STATE_PROCESSING );

		// Get the mesh 
		Core::vtkPolyDataPtr vtkInputMesh;
		GetProcessingData( 0, vtkInputMesh );


		// Call the function
		Core::vtkPolyDataPtr vtkOutputMesh = Core::vtkPolyDataPtr::New();
		blVTKHelperTools::ScaleShape( 
			vtkInputMesh, 
			vtkOutputMesh, 
			GetParametersHolder()->GetSubject( ), 
			true);


		// Set the output to the output of this processor
		GetOutputPort( 0 )->UpdateOutput( 
			vtkOutputMesh, 
			0,
			GetInputDataEntity( 0 ) );
		
	}
	catch( ... )
	{
		// Throw the exception again to be catched by the Widget and
		// show a message box with the error message
		SetState( Core::Runtime::APP_STATE_IDLE );
		throw;
	}

	SetState( Core::Runtime::APP_STATE_IDLE );

}

Core::ShapeScaleProcessor::ParametersHolder::Pointer 
Core::ShapeScaleProcessor::GetParametersHolder() const
{
	return m_ParametersHolder;
}

