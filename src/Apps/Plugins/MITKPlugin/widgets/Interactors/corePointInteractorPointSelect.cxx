/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePointInteractorPointSelect.h"
#include "mitkSurface.h"
#include "coreVTKPolyDataHolder.h"

/**
 */
Core::PointInteractorPointSelect::PointInteractorPointSelect(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
	:PointInteractor( renderingTree, selectedPoints, selectedData )
{
	m_CuttingType = SKELETON_TYPE;
	for( int i = 0; i < 6; ++i )
	{
		m_BoundingBox[ i ] = 0;
	}
}


/**
 */
Core::PointInteractorPointSelect::~PointInteractorPointSelect( )
{
	DisconnectFromDataTreeNode();
}

void Core::PointInteractorPointSelect::CreateInteractor()
{
	if ( m_pointSelectInteractor.IsNull() )
	{
		m_pointSelectInteractor = blMitkPointSelectInteractor::New( 
			"PointSelectInteractor", 
			GetSelectedPointsNode(),
			GetSelectedDataEntityNode() );
		SetCuttingType( m_CuttingType );
		// If bounding box is empty, it is computed in the constructor
		// using polyData
		if ( m_BoundingBox[ 0 ] != 0 || m_BoundingBox[ 1 ] != 0 || m_BoundingBox[ 2 ] != 0 ||
			 m_BoundingBox[ 3 ] != 0 || m_BoundingBox[ 4 ] != 0 || m_BoundingBox[ 5 ] != 0 )
		{
			SetBoundingBox( m_BoundingBox );
		}
	}
}

mitk::Interactor* Core::PointInteractorPointSelect::GetInternalInteractor()
{
	return m_pointSelectInteractor;
}

void Core::PointInteractorPointSelect::DestroyInteractor()
{
	m_pointSelectInteractor = NULL ;
}

void Core::PointInteractorPointSelect::SetCuttingType( CUT_TYPE cutting_type )
{
	m_CuttingType = cutting_type;

	if ( m_pointSelectInteractor.IsNotNull() )
	{
		m_pointSelectInteractor->SetCuttingType(cutting_type);
	}
}

int Core::PointInteractorPointSelect::GetDirection( )
{
	int iDirection = -1;
	if ( m_pointSelectInteractor.IsNotNull() )
	{
		iDirection = m_pointSelectInteractor->GetDirection( );
	}
	return iDirection;
}


void Core::PointInteractorPointSelect::SetBoundingBox( double bounds[ 6 ] )
{
	for ( int i = 0 ; i < 6 ; i++ )
	{
		m_BoundingBox[ i ] = bounds[ i ];
	}

	if ( m_pointSelectInteractor.IsNotNull() )
	{
		m_pointSelectInteractor->SetBoundingBox( m_BoundingBox );
	}
}

void Core::PointInteractorPointSelect::SetBoundingBox( 
	Core::DataEntity::Pointer dataEntity )
{
	double bounds[6];
	for( int i = 0; i < 6; ++i )
	{
		bounds[ i ] = 0;
	}
	if ( dataEntity.IsNotNull() )
	{
		Core::vtkPolyDataPtr mesh;
		dataEntity->GetProcessingData( mesh );
		if ( mesh != NULL )
		{ 
			mesh->GetBounds(bounds);
		}
	}
	SetBoundingBox( bounds );
}

int Core::PointInteractorPointSelect::GetCellId()
{
	return this->m_pointSelectInteractor->GetCellId();
}

Core::DataEntityType Core::PointInteractorPointSelect::GetInputTypes( )
{
	return Core::DataEntityType( 
		Core::SurfaceMeshTypeId | 
		Core::SkeletonTypeId |
		Core::VolumeMeshTypeId );
}

void Core::PointInteractorPointSelect::OnInteractorConnected()
{
	if ( GetSelectedDataEntity( )->IsSkeletonMesh() &&
		 GetSelectedDataEntity( )->GetFather().IsNotNull() )
	{
		Core::DataEntity::Pointer fatherDataEntity;
		fatherDataEntity = GetSelectedDataEntity( )->GetFather();

		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( GetRenderingTree()->GetNode( fatherDataEntity ), node );
		if ( node.IsNull() )
		{
			Core::CastAnyProcessingData( GetRenderingTree()->Add( 
				fatherDataEntity, 
				false ), node );
		}

		node->SetOpacity( 0.4 );

		GetRenderingTree()->Show( fatherDataEntity, true );
	}
}

int Core::PointInteractorPointSelect::GetTimeStep()
{
	return m_pointSelectInteractor->GetTimeStep( );
}
