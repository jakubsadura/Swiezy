/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreThresholdProcessor.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"

#include "vtkPointData.h"
#include "blVTKHelperTools.h"

Core::ThresholdProcessor::ThresholdProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output surface" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "Threshold" );

	SetName( "ThresholdProcessor" );

	m_LowerThreshold = 0;
	m_UpperThreshold = 0;
	m_ScalarsArrayName = "";
}

void Core::ThresholdProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		// Get the mesh 
		Core::vtkPolyDataPtr vtkInputMesh;
		GetProcessingData( 0, vtkInputMesh );

		// Call the function
		Core::vtkPolyDataPtr outputMesh;
		if (std::strcmp(m_ScalarsArrayName.c_str(), ""))
			vtkInputMesh->GetPointData()->SetActiveScalars( m_ScalarsArrayName.c_str( ) );
		outputMesh.TakeReference( blVTKHelperTools::GetShapeSubSurface( 
			vtkInputMesh, 
			m_LowerThreshold, 
			m_UpperThreshold ) );

		if ( outputMesh.GetPointer() == NULL )
		{
			throw Exception( "Threshold cannot be applied" );
		}

		Core::vtkPolyDataPtr vtkOutputMesh = Core::vtkPolyDataPtr::New();
		vtkOutputMesh->DeepCopy( outputMesh );

		// Set the output to the output of this processor
		GetOutputPort( 0 )->UpdateOutput( vtkOutputMesh, 0, GetInputDataEntity(0) );

		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	catch( ... )
	{
		SetState(Core::Runtime::APP_STATE_IDLE);
		throw;
	}
}

const char* Core::ThresholdProcessor::GetScalarsArrayName() const
{
	return m_ScalarsArrayName.c_str();
}

void Core::ThresholdProcessor::SetScalarsArrayName( const char* name )
{
	m_ScalarsArrayName = name;
}

unsigned int Core::ThresholdProcessor::GetLowerThreshold() const
{
	return m_LowerThreshold;
}

void Core::ThresholdProcessor::SetLowerThreshold( unsigned int val )
{
	m_LowerThreshold = val;
}

unsigned int Core::ThresholdProcessor::GetUpperThreshold() const
{
	return m_UpperThreshold;
}

void Core::ThresholdProcessor::SetUpperThreshold( unsigned int val )
{
	m_UpperThreshold = val;
}
