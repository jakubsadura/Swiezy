/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePointInteractorPointSet.h"
#include "coreVTKPolyDataHolder.h"


vtkScalarValueCommand *vtkScalarValueCommand::New() 
{ 
	return new vtkScalarValueCommand; 
}

void vtkScalarValueCommand::SetInteractor( Core::PointInteractorPointSet* interactor )
{
	m_interactor = interactor;
}

void vtkScalarValueCommand::Execute(vtkObject* caller, unsigned long eventId, void* callData)
{
	m_interactor->OnScalarValueModified( );
}

Core::PointInteractorPointSet::PointInteractorPointSet(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
	:PointInteractor( renderingTree, selectedPoints, selectedData )
{
	m_bRestrictAddPointPosition = false;
	m_EnableContinuousMoveEvent = false;
	m_bIncrementalAdd = true;
	m_iMaxPoints = -1;
	m_bClickAndDragLeftMouseButton = true;

	m_ScalarValueCommand = vtkScalarValueCommand::New();
	m_ScalarValueCommand->SetInteractor( this );
	m_ScalarVectorObserverTag = 0;

}


/**
 */
Core::PointInteractorPointSet::~PointInteractorPointSet( )
{
	if ( m_pointSetInteractor.IsNotNull() )
	{
		vtkIntArray *scalarVector;
		scalarVector = m_pointSetInteractor->GetScalarVector();
		scalarVector->RemoveObserver( m_ScalarVectorObserverTag );
	}

	m_ScalarValueCommand->Delete( );

	DisconnectFromDataTreeNode();
}

void Core::PointInteractorPointSet::CreateInteractor()
{
	std::string strStateMachineName;
	if ( m_bClickAndDragLeftMouseButton )
	{
		strStateMachineName = "pointsetinteractorWithTime";
	}
	else
	{
		strStateMachineName = "pointsetinteractor";
	}
	if ( m_pointSetInteractor.IsNull() )
	{
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( GetRenderingTree()->GetNode( GetSelectedPointsDataEntity() ), node );
		m_pointSetInteractor = blMitkPointSetInteractor::New( 
			strStateMachineName.c_str(), 
			node,
			m_iMaxPoints );

		vtkIntArray *scalarVector;
		scalarVector = m_pointSetInteractor->GetScalarVector();
		m_ScalarVectorObserverTag = scalarVector->AddObserver( 
			vtkCommand::ModifiedEvent, 
			m_ScalarValueCommand );
	}

}

void Core::PointInteractorPointSet::DestroyInteractor()
{
	m_pointSetInteractor = NULL;
}

mitk::Interactor* Core::PointInteractorPointSet::GetInternalInteractor()
{
	return m_pointSetInteractor;
}

void Core::PointInteractorPointSet::OnScalarValueModified( )
{
	// Update the observers
	m_bUpdatingVtkPointSet = true;
	GetSelectedPointsHolder( )->NotifyObservers();
	m_bUpdatingVtkPointSet = false;
}

int Core::PointInteractorPointSet::GetScalarValue()
{
	int iValue = 0;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		iValue = m_pointSetInteractor->GetScalarVector()->GetValue( 0 );
	}

	return iValue;
}


bool Core::PointInteractorPointSet::IsCurrentActionAddPoint()
{
	bool bCurrentActionIsAddPoint = false;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		bCurrentActionIsAddPoint = m_pointSetInteractor->CheckCurrentActionIsAddPoint();
	}
	return bCurrentActionIsAddPoint;
}


bool Core::PointInteractorPointSet::IsCurrentActionMovePoint()
{
	bool bCurrentActionIsMovePoint = false;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		bCurrentActionIsMovePoint = m_pointSetInteractor->CheckCurrentStateIsMovePointSpace();
	}
	return bCurrentActionIsMovePoint;
}

void Core::PointInteractorPointSet::SetRestrictAddPointPosition( bool bVal )
{
	m_bRestrictAddPointPosition = bVal;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		m_pointSetInteractor->SetRestrictAddPointPosition( 
			m_bRestrictAddPointPosition,
			GetSelectedDataEntityNode() );
	}
}

void Core::PointInteractorPointSet::SetIncrementalAdd( bool val )
{
	m_bIncrementalAdd = val;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		m_pointSetInteractor->SetIncrementalAdd( m_bIncrementalAdd );
	}
}

void Core::PointInteractorPointSet::OnInteractorConnected()
{
	SetRestrictAddPointPosition( m_bRestrictAddPointPosition );

	SetIncrementalAdd( m_bIncrementalAdd );

	SetMaxPoints( m_iMaxPoints );
}

int Core::PointInteractorPointSet::GetMaxPoints() const
{
	return m_iMaxPoints;
}

void Core::PointInteractorPointSet::SetMaxPoints( int val )
{
	m_iMaxPoints = val;

	if ( m_pointSetInteractor.IsNotNull() )
	{
		m_pointSetInteractor->SetMaxPoints( m_iMaxPoints );
	}
}

bool Core::PointInteractorPointSet::GetLastClickedPlane( 
	mitk::Vector3D &normal, mitk::Point3D &origin )
{
	bool success = false;
	if ( m_pointSetInteractor.IsNotNull( ) )
	{
		success |= m_pointSetInteractor->GetLastClickedNormal( normal );
		success |= m_pointSetInteractor->GetLastClickedOrigin( origin );
	}

	return success;
}


void Core::PointInteractorPointSet::SetEnableContinuousMoveEvent( bool bVal )
{
	m_EnableContinuousMoveEvent = bVal;
}

bool Core::PointInteractorPointSet::GetClickAndDragLeftMouseButton() const
{
	return m_bClickAndDragLeftMouseButton;
}

void Core::PointInteractorPointSet::SetClickAndDragLeftMouseButton( bool val )
{
	m_bClickAndDragLeftMouseButton = val;
}

bool Core::PointInteractorPointSet::CheckUpdateDataOnEvent( INTERACTOR_EVENT_TYPE eventType )
{
	// If the user is moving the point, we need to check if this event is enabled
	bool bUpdateData = true;
	if ( eventType == INTERACTOR_EVENT_MOVE && 
		 m_pointSetInteractor->CheckCurrentStateIsMovePointSpace() && 
		 !m_EnableContinuousMoveEvent )
	{
		bUpdateData = false;
	}

	return bUpdateData;
}

void Core::PointInteractorPointSet::SetMeasurementPointSet(bool val)
{
	m_pointSetInteractor->SetMeasurementPointSet(val);
}

Core::DataEntityType Core::PointInteractorPointSet::GetInputTypes( )
{
	return Core::DataEntityType( 
		Core::ImageTypeId | 
		Core::ROITypeId |
		Core::SurfaceMeshTypeId );
}

int Core::PointInteractorPointSet::GetTimeStep()
{
	if ( m_pointSetInteractor.IsNull() )
	{
		return 0;
	}
	return m_pointSetInteractor->GetTimeStep( );
}

//! Set line contour color
void Core::PointInteractorPointSet::SetContourColor(double color[3])
{
	if(m_pointSetInteractor.IsNotNull())
		m_pointSetInteractor->SetContourColor(color);
}

//! Get line color
double *Core::PointInteractorPointSet::GetContourColor()
{
	if(m_pointSetInteractor.IsNotNull())
		return m_pointSetInteractor->GetContourColor();
	else
		return NULL;
}