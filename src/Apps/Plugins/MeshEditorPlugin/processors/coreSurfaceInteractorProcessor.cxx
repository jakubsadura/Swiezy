/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSurfaceInteractorProcessor.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"

#include "vtkPointData.h"
#include "vtkCellData.h"
#include "blVTKHelperTools.h"
#include "blShapeUtils.h"


Core::SurfaceInteractorProcessor::SurfaceInteractorProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output surface" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "SurfaceInteractor" );

	SetName( "SurfaceInteractorProcessor" );

}

void Core::SurfaceInteractorProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		// Get the mesh 
		Core::vtkPolyDataPtr vtkInputMesh;
		GetProcessingData( 0, vtkInputMesh );

		// Call the function
		Core::vtkPolyDataPtr outputMesh  = Core::vtkPolyDataPtr::New();
		outputMesh->DeepCopy( vtkInputMesh);
	
		if ( outputMesh.GetPointer() == NULL ||
			!outputMesh->GetCellData()->HasArray("select"))
		{
			throw Exception( "SurfaceInteractor cannot be applied" );
		}

		switch (m_processorType)
		{
		case RemoveCells:
			{
				outputMesh = blShapeUtils::ShapeUtils::GetShapeRegion(vtkInputMesh,0,0, "select");
				outputMesh->GetCellData()->RemoveArray("select");
				GetOutputPort( 0 )->UpdateOutput( outputMesh, 0, GetInputDataEntity(0) );
			}
			break;
		case LocalRefiner:
			{
				LocalRefinerProcessor::Pointer refProc =
					LocalRefinerProcessor::New();
				refProc->SetInputDataEntity(0,GetInputDataEntity(0));
				refProc->Update();
				GetOutputPort(0)->UpdateOutput(
					refProc->GetOutputPort(0)->GetDataEntity()->GetProcessingData(),
					0,
					GetInputDataEntity(0));
			}
			break;
		case LoopSubdivision:
			{
				LoopSubdivisionProcessor::Pointer loopProc =
					LoopSubdivisionProcessor::New();
				loopProc->SetInputDataEntity(0,GetInputDataEntity(0));
				loopProc->Update();
				GetOutputPort(0)->UpdateOutput(
					loopProc->GetOutputPort(0)->GetDataEntity()->GetProcessingData(),
					0,
					GetInputDataEntity(0));
			}
			break;
		case EdgeSwap:
			{
				EdgeSwappingProcessor::Pointer swapProc =
					EdgeSwappingProcessor::New();
				swapProc->SetInputDataEntity(0,GetInputDataEntity(0));
				swapProc->Update();
				GetOutputPort(0)->UpdateOutput(
					swapProc->GetOutputPort(0)->GetDataEntity()->GetProcessingData(),
					0,
					GetInputDataEntity(0));
			}
			break;
		case TaubinSmooth:
			{
				TaubinSmoothProcessor::Pointer smoothProc =
					TaubinSmoothProcessor::New();
				smoothProc->SetInputDataEntity(0,GetInputDataEntity(0));
				smoothProc->Update();
				GetOutputPort(0)->UpdateOutput(
					smoothProc->GetOutputPort(0)->GetDataEntity()->GetProcessingData(),
					0,
					GetInputDataEntity(0));
			}
			break;
		}

	
		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	catch( ... )
	{
		SetState(Core::Runtime::APP_STATE_IDLE);
		throw;
	}
}

void Core::SurfaceInteractorProcessor::SetProcessorType( ProcessorType type )
{
	m_processorType = type;
}