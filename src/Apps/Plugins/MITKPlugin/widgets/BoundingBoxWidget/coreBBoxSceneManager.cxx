/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBBoxSceneManager.h"

#include "vtkCubeSource.h"

#include "mitkVtkRepresentationProperty.h"
#include "mitkRenderingManager.h"
#include "mitkProperties.h"
#include "coreRenderingTreeMITK.h"


Core::BBoxSceneManager::BBoxSceneManager(
	BoundingBoxHolder::Pointer dataHolder,
	Core::RenderingTree::Pointer renderingTree
)
{
	ConstructDataHolder(dataHolder);
	ConstructBoundingBoxDataEntity();
	ConstructRenderingTree(renderingTree);
	ConstructHandles();
	m_Enable = false;

	UpdateBoundingBoxActor();
	UpdateHandleLocations();
}

void Core::BBoxSceneManager::OnCroppingDataChanged()
{
	Core::BoundingBox::Pointer croppingData = m_DataHolder->GetSubject();
	if( croppingData.IsNull() || !IsEnabled() )
		return;

	UpdateBoundingBoxActor();
	SetHandleSize(
		ScalarType(0.04 * m_DataHolder->GetSubject( )->GetMeanSize( ) )
	);
	UpdateHandleLocations();
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdateAll();
}

Core::BBoxSceneManager::~BBoxSceneManager()
{
	m_RenderingTree->Remove(m_BoundingBoxDataEntity, false);

	for ( int i = 0 ; i < m_Handles.size() ; i ++ )
	{
		m_Handles[ i ]->GetHandlePointSet( )->RemoveObserver( 
			m_HandleObserverHandleWasMoved[ i ] );
	}

	for ( int i = 0 ; i < m_Handles.size() ; i ++ )
	{
		m_Handles[ i ] = NULL;
	}

}

void Core::BBoxSceneManager::ConstructBoundingBoxDataEntity()
{
	m_BoundingBox = mitk::Cuboid::New();
	m_BoundingBoxDataEntity = Core::DataEntity::New(Core::MITKDataTypeId);
	m_BoundingBoxDataEntity->GetMetadata( )->SetName( "CroppingBox" );
	m_BoundingBoxDataEntity->AddTimeStep(m_BoundingBox);
	m_BoundingBoxDataEntity->SetRenderingData(mitk::BaseData::Pointer(m_BoundingBox.GetPointer()));
}

void Core::BBoxSceneManager::ConstructDataHolder( Core::BoundingBoxHolder::Pointer dataHolder )
{
	m_DataHolder = dataHolder;
	if( m_DataHolder.IsNull() )
	{
		m_DataHolder = Core::BoundingBoxHolder::New();
	}
	m_DataHolder->AddObserver(this, &Core::BBoxSceneManager::OnCroppingDataChanged);
}

void Core::BBoxSceneManager::ConstructRenderingTree( 
	Core::RenderingTree::Pointer &renderingTree )
{
	m_RenderingTree = renderingTree;
	if( renderingTree.IsNull() )
	{
		m_RenderingTree = Core::RenderingTreeMITK::New();
	}
}

void Core::BBoxSceneManager::SetHandleSize( float handleSize )
{
	for( unsigned int iHandle = 0; iHandle < m_Handles.size(); ++iHandle )
	{
		m_Handles[iHandle]->SetHandleSize(handleSize);
	}
}

void Core::BBoxSceneManager::UpdateBoundingBoxActor()
{
	if ( m_DataHolder->GetSubject().IsNull( ) )
	{
		return;
	}

	vtkCubeSource* cube = vtkCubeSource::New();

	double bounds[ 6 ];
	m_DataHolder->GetSubject( )->GetBounds( bounds );
	cube->SetBounds( bounds );
	cube->Update();
	m_BoundingBox->SetVtkPolyData(cube->GetOutput());
	
	cube->Delete();
}

void Core::BBoxSceneManager::UpdateHandleLocations()
{
	Core::BoundingBox::Pointer boundingBox = m_DataHolder->GetSubject();
	if( boundingBox.IsNull() || !IsEnabled() )
		return;

	itk::Point<double> pointMin;
	itk::Point<double> pointMax;

	boundingBox->GetHandleLocation( 0, pointMin, pointMax );
	m_Handles[ 0 ]->SetLocation( pointMin );
	m_Handles[ 1 ]->SetLocation( pointMax );

	boundingBox->GetHandleLocation( 1, pointMin, pointMax );
	m_Handles[ 2 ]->SetLocation( pointMin );
	m_Handles[ 3 ]->SetLocation( pointMax );

	boundingBox->GetHandleLocation( 2, pointMin, pointMax );
	m_Handles[ 4 ]->SetLocation( pointMin );
	m_Handles[ 5 ]->SetLocation( pointMax );
}

void Core::BBoxSceneManager::UpdateBondingBoxFromHandlers()
{
	Core::BoundingBox::Pointer boundingBox = m_DataHolder->GetSubject();
	if( boundingBox.IsNull() || !IsEnabled() )
		return;

	itk::Point<double> pointMin;
	itk::Point<double> pointMax;
	
	pointMin = m_Handles[ 0 ]->GetLocation( );
	pointMax = m_Handles[ 1 ]->GetLocation( );
	boundingBox->SetHandleLocation( 0, pointMin, pointMax );

	pointMin = m_Handles[ 2 ]->GetLocation( );
	pointMax = m_Handles[ 3 ]->GetLocation( );
	boundingBox->SetHandleLocation( 1, pointMin, pointMax );

	pointMin = m_Handles[ 4 ]->GetLocation( );
	pointMax = m_Handles[ 5 ]->GetLocation( );
	boundingBox->SetHandleLocation( 2, pointMin, pointMax );
}

void Core::BBoxSceneManager::ConstructHandles()
{
	while( m_Handles.size() < 6 )
	{
		BBoxSceneManagerHandler::Pointer handle;
		handle = BBoxSceneManagerHandler::New(
				m_RenderingTree, 
				m_Handles.size(), 
				m_DataHolder->GetSubject(),
				m_BoundingBoxDataEntity );

		//! Call back called when the user moves a point
		itk::SimpleMemberCommand<Core::BBoxSceneManager>::Pointer handleWasMovedCommand;

		unsigned long handleWasMovedTag;
		handleWasMovedCommand = itk::SimpleMemberCommand<Core::BBoxSceneManager>::New();
		handleWasMovedCommand->SetCallbackFunction(
			this, 
			&Core::BBoxSceneManager::OnHandleWasMoved
			);
		handleWasMovedTag = handle->GetHandlePointSet( )->AddObserver(
			mitk::PointSetMoveEvent(), 
			handleWasMovedCommand);

		m_HandleObserverHandleWasMoved.push_back( handleWasMovedTag );

		m_Handles.push_back(handle);
	}
}

void Core::BBoxSceneManager::Enable( bool enable /*= true */ )
{
	if ( enable )
	{
		m_RenderingTree->Add(m_BoundingBoxDataEntity, true, false);
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( m_RenderingTree->GetNode(m_BoundingBoxDataEntity), node );
		node->SetOpacity(0.3f);
	}
	else
	{
		m_RenderingTree->Remove( m_BoundingBoxDataEntity, false );
	}

	m_Enable = enable;

	// Update locations before enabling them
	UpdateBoundingBoxActor();
	UpdateHandleLocations();

	for( unsigned int iHandle = 0; iHandle < m_Handles.size(); ++iHandle )
	{
		m_Handles[iHandle]->Enable( enable, enable );
	}

	// Update precision of interactor one the rendering is enabled
	OnCroppingDataChanged();
}

bool Core::BBoxSceneManager::IsEnabled()
{
	return m_Enable;
}

Core::DataEntity::Pointer Core::BBoxSceneManager::GetInputDataEntity() const
{
	return m_InputDataEntity;
}

void Core::BBoxSceneManager::SetInputDataEntity( Core::DataEntity::Pointer val )
{
	m_InputDataEntity = val;
}

void Core::BBoxSceneManager::OnHandleWasMoved()
{
	UpdateBondingBoxFromHandlers( );
	m_DataHolder->NotifyObservers( );
}

Core::DataEntity::Pointer Core::BBoxSceneManager::GetBoundingBoxDataEntity() const
{
	return m_BoundingBoxDataEntity;
}