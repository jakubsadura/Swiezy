/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "coreSkeletonCutProcessor.h"

#include "coreDataEntityHelper.h"

#include "vtkCleanPolyData.h"
#include "meRingCut.h"

Core::SkeletonCutProcessor::SkeletonCutProcessor( )
{
	SetNumberOfInputs( 3 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 2 )->SetName( "Input point" );
	GetInputPort( 2 )->SetDataEntityType( Core::PointSetTypeId );
	GetInputPort( 1 )->SetName( "Input skeleton" );
	GetInputPort( 1 )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	SetNumberOfOutputs( 2 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "Cutted Mesh" );
	GetOutputPort( 1 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 1 )->SetReuseOutput( true );
	GetOutputPort( 1 )->SetDataEntityName( "Plane" );

	SetName( "SkeletonCutProcessor" );
	m_plane = DirectionalPlanePtr(new DirectionalPlane);
	m_meRingCut = meRingCut::New();
	
}

void Core::SkeletonCutProcessor::Update()
{
	try
	{

		SetState(Core::Runtime::APP_STATE_PROCESSING);
	
		
		meRingCut::Pointer	ringCut;
		Core::vtkPolyDataPtr	vtkSelectedMesh;
		Core::vtkPolyDataPtr	vtkSelectedSkeleton;
		Core::vtkPolyDataPtr	vtkSelectedPoint;

		//----------------------------------------------------
		ringCut = GetRingCut();
		GetProcessingData( 0 ,vtkSelectedMesh);
		GetProcessingData( 1 ,vtkSelectedSkeleton);
		GetProcessingData( 2 ,vtkSelectedPoint);
		
		
		if ( vtkSelectedPoint->GetNumberOfPoints() == 0 )
		{
			Core::LoggerHelper::ShowMessage( 
				"Please select a point of the skeleton using shift+left "\
				"mouse button", 
				Core::LoggerHelper::MESSAGE_TYPE_ERROR );
			return;
		}
		if ( !m_plane->IsValidPlane() )
		{
			Core::LoggerHelper::ShowMessage( 
				"Please select a point of the skeleton that has exactly "\
				"two neighbors", 
				Core::LoggerHelper::MESSAGE_TYPE_ERROR );
			return;
		}

		ringCut->Update( );

		GetOutputPort( 0 )->UpdateOutput( ringCut->GetFirstOutput(), 0, GetInputDataEntity(0) );

	/*
		UpdateOutput(1,
			ring->GetFirstOutput(),
			"Surface2",
			false,
			1,
			GetInputDataEntity(0));*/
		SetState(Core::Runtime::APP_STATE_IDLE);
		
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::SkeletonCutProcessor::Update)
}

meRingCut::Pointer 
Core::SkeletonCutProcessor::GetRingCut() const
{
	return m_meRingCut;
}

void Core::SkeletonCutProcessor::UpdatePlane()
{
	try
	{
		
		double			skeletonPoint[ 3 ];
		meRingCut::Pointer	ringCut;
		double			normalVector[ 3 ];
		Core::vtkPolyDataPtr	vtkSelectedMesh;
		Core::vtkPolyDataPtr	vtkSelectedSkeleton;
		Core::vtkPolyDataPtr	vtkSelectedPoint;


		//----------------------------------------------------
		ringCut = GetRingCut();
		//ringCut = geoAPI::geoRingCut::New();
		GetProcessingData( 0 ,vtkSelectedMesh);
		GetProcessingData( 1 ,vtkSelectedSkeleton);
		GetProcessingData( 2 ,vtkSelectedPoint);

		if ( !(vtkSelectedPoint->GetNumberOfPoints()>0 &&
			GetInputDataEntity( 2 )->GetFather() == GetInputDataEntity( 1 ) ))
		{
			throw Core::Exceptions::Exception(
				"SkeletonCutProcessor::UpdatePlane",
				"Landmarks is not correct" );
		}

		
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		m_plane->SetValidPlane( true );
		ringCut->SetInput( vtkSelectedMesh );

		skeletonPoint[ 0 ] = vtkSelectedPoint->GetPoint( 0 )[ 0 ];
		skeletonPoint[ 1 ] = vtkSelectedPoint->GetPoint( 0 )[ 1 ];
		skeletonPoint[ 2 ] = vtkSelectedPoint->GetPoint( 0 )[ 2 ];
		ringCut->SetSkeletonPoint( skeletonPoint );


		//---------------------------------------------------------
		// If the selected point is the point used to cut the mesh
		// in the last time, don't update the plane.
		bool bLastSelectedPointForCuttingStillSelected;
/*		bool bValidPoint = false;
		double dPointCutted[ 3 ];
		//m_WorkingData->GetSkeletonLastSelectedPointCutted( bValidPoint, dPointCutted );
		bLastSelectedPointForCuttingStillSelected =
			bValidPoint && 
			dPointCutted[ 0 ] == skeletonPoint[ 0 ] &&
			dPointCutted[ 1 ] == skeletonPoint[ 1 ] &&
			dPointCutted[ 2 ] == skeletonPoint[ 2 ];
*/
		bLastSelectedPointForCuttingStillSelected = false;
		//---------------------------------------------------------
		if ( !bLastSelectedPointForCuttingStillSelected  )
		{
			//m_WorkingData->SetSkeletonLastSelectedPointCutted( false, skeletonPoint );

			//---------------------------------------------------------
			// Check a valid selected point
			m_plane->UpdateNormalVector( skeletonPoint, vtkSelectedSkeleton );

			if ( m_plane->IsValidPlane() )
			{
				m_plane->GetNormalVector( normalVector );
				ringCut->SetNormal( normalVector );

				// Get the results from the API
				m_plane->SetNormalVector( ringCut->GetNormal() );
				m_plane->SetCenter( ringCut->GetCenter() );
				m_plane->SetRadius( ringCut->GetRadius() );
			}
		}

		GetOutputPort( 1 )->UpdateOutput( m_plane->GetVtkPolyData(), 0, GetInputDataEntity(0) );
	}
	catch( ... )
	{
		SetState(Core::Runtime::APP_STATE_IDLE);
		throw;
	}

	SetState(Core::Runtime::APP_STATE_IDLE);
}

bool Core::SkeletonCutProcessor::CheckInputs()
{
	
	Core::vtkPolyDataPtr polydata ;
	GetProcessingData( 0 , polydata );
	Core::vtkPolyDataPtr skeleton ;
	GetProcessingData( 1 , skeleton);
	return true;
	
}
