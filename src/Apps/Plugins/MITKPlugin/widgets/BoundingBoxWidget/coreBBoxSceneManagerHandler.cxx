/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBBoxSceneManagerHandler.h"

//mitk
#include "mitkGlobalInteraction.h"
#include <mitkProperties.h>


Core::BBoxSceneManagerHandler::BBoxSceneManagerHandler( 
	Core::RenderingTree::Pointer renderingTree, 
	int position, 
	Core::BoundingBox::Pointer croppingData,
	Core::DataEntity::Pointer parentDataEntity )
{
	m_RenderingTree = renderingTree;
	m_Position = position;
	m_CroppingData = croppingData;

	m_HandlePointSet = mitk::PointSet::New();
	m_HandlePointSet->InsertPoint(0, mitk::PointSet::PointType());

	m_DataEntity = Core::DataEntity::New(Core::MITKDataTypeId);
	m_DataEntity->SetFather( parentDataEntity );
	m_DataEntity->AddTimeStep(m_HandlePointSet);
	m_DataEntity->SetRenderingData(mitk::BaseData::Pointer(m_HandlePointSet));
	char str[256];
	sprintf( str, "Handler%d", position );
	m_DataEntity->GetMetadata()->SetName( str );

	m_HandleWasMovedCommand = itk::SimpleMemberCommand<Core::BBoxSceneManagerHandler>::New();
	m_HandleWasMovedCommand->SetCallbackFunction(
		this, 
		&Core::BBoxSceneManagerHandler::OnHandleWasMoved
		);
	m_HandleWasMovedTag = m_HandlePointSet->AddObserver(itk::EndEvent(), m_HandleWasMovedCommand);
}

Core::BBoxSceneManagerHandler::~BBoxSceneManagerHandler()
{
	m_HandlePointSet->RemoveObserver( m_HandleWasMovedTag );
	m_RenderingTree->Remove(m_DataEntity, false);
	mitk::GlobalInteraction::GetInstance()->RemoveInteractor(m_Interactor);
}

void Core::BBoxSceneManagerHandler::Enable( 
	bool enable /*= true */,
	bool show /*= true*/)
{

	if (enable)
	{
		m_RenderingTree->Add(m_DataEntity, show, false );

		mitk::Point3D offset, direction;
		direction[0] = float( m_Position == 0 || m_Position == 1 ? 1 : 0 );
		direction[1] = float( m_Position == 2 || m_Position == 3 ? 1 : 0 );
		direction[2] = float( m_Position == 4 || m_Position == 5 ? 1 : 0 );
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( m_RenderingTree->GetNode(m_DataEntity), node );
		m_Interactor = blMitkLineBoundPointInteractor::New(
			node.GetPointer(),
			offset, 
			direction
			);

		// Set also the size for the interactor
		//SetHandleSize( 1.0 );

		mitk::GlobalInteraction::GetInstance()->AddInteractor(m_Interactor);
	}
	else
	{
		m_RenderingTree->Remove(m_DataEntity, false);
		mitk::GlobalInteraction::GetInstance()->RemoveInteractor(m_Interactor);
	}
}

void Core::BBoxSceneManagerHandler::OnHandleWasMoved()
{
	mitk::PointSet::PointType location = m_HandlePointSet->GetPoint(0);
}

void Core::BBoxSceneManagerHandler::SetHandleSize( float handleSize )
{
	m_RenderingTree->SetProperty( m_DataEntity, blTag::New( "pointsize", handleSize ) );
	if ( m_Interactor.IsNotNull() )
	{
		m_Interactor->SetPrecision( handleSize );
	}
}

void Core::BBoxSceneManagerHandler::SetLocation( mitk::PointSet::PointType& point )
{
	m_HandlePointSet->SetPoint(0, point);
	if ( m_Interactor.IsNotNull() )
	{
		m_Interactor->SetOffset(point);
	}
}

void Core::BBoxSceneManagerHandler::SetLocation( itk::Point<double>& point )
{
	mitk::PointSet::PointType mitkPoint;
	mitkPoint[ 0 ] = point[ 0 ];
	mitkPoint[ 1 ] = point[ 1 ];
	mitkPoint[ 2 ] = point[ 2 ];
	SetLocation( mitkPoint );
}

void Core::BBoxSceneManagerHandler::Show( bool visible )
{
	m_RenderingTree->Show(m_DataEntity, visible);
}

mitk::PointSet::Pointer Core::BBoxSceneManagerHandler::GetHandlePointSet() const
{
	return m_HandlePointSet;
}

itk::Point<double> Core::BBoxSceneManagerHandler::GetLocation()
{
	mitk::PointSet::PointType mitkPoint = GetHandlePointSet()->GetPoint( 0 );

	itk::Point<double> point;
	point[ 0 ] = mitkPoint[ 0 ];
	point[ 1 ] = mitkPoint[ 1 ];
	point[ 2 ] = mitkPoint[ 2 ];
	
	return point;
}

