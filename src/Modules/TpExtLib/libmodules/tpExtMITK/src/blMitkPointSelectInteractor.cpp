/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkPointSelectInteractor.h"
#include "vtkOBBTree.h"
#include "vtkSmartPointer.h"
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include "vtkCamera.h"
#include "vtkLine.h"
#include "vtkMath.h"

#include <mitkPointSet.h>
#include <mitkPointOperation.h>
#include <mitkPositionEvent.h>
#include <mitkStateEvent.h>
#include <mitkOperationEvent.h>
#include <mitkUndoController.h>
#include <mitkDataTreeNode.h>
#include <mitkInteractionConst.h>
#include <mitkAction.h>
#include <mitkProperties.h>
#include <mitkSurface.h>
#include <mitkBaseRenderer.h>
#include <mitkRenderingManager.h>
#include "mitkBoundingObject.h"
#include "mitkGlobalInteraction.h"
//#include "mitkOpenGLRenderer.h"
#include "mitkVtkPropRenderer.h"

#include "blMITKUtils.h"

using namespace mitk;

blMitkPointSelectInteractor::blMitkPointSelectInteractor(
	const char * type, 
	mitk::DataTreeNode* pointSetNode,
	mitk::DataTreeNode* dataTreeSet,
	double relativeSelectedPointSize,
	double relativeMarginAroundBoundingBox
)
: Interactor(type, pointSetNode)
{
	m_MeshTreeNode = dataTreeSet;
	m_Direction = 0;

	// Find out how large the point set is, and set the mitk point size accordingly
	for( int i = 0; i < 6; ++i )
	{
		m_BoundingBox[ i ] = 0;
	}
	if ( m_MeshTreeNode.IsNotNull() && m_MeshTreeNode->GetData() )
	{
		blMITKUtils::ComputeBoundingBox( m_MeshTreeNode->GetData(), m_BoundingBox );

		double pointSize;
		blMITKUtils::ComputePointSize( m_MeshTreeNode->GetData(), pointSize, relativeSelectedPointSize);
		this->m_DataTreeNode->SetProperty("pointsize", mitk::FloatProperty::New(pointSize));
	}

	m_CutType = SKELETON_TYPE;
	m_cellId = 0;
}

blMitkPointSelectInteractor::~blMitkPointSelectInteractor()
{
}

int blMitkPointSelectInteractor::GetDirection( )
{
	return m_Direction;
}

void blMitkPointSelectInteractor::SetBoundingBox(double const bounds[ 6 ]) 
{ 
	m_BoundingBox[ 0 ] = bounds[ 0 ];
	m_BoundingBox[ 1 ] = bounds[ 1 ];
	m_BoundingBox[ 2 ] = bounds[ 2 ];
	m_BoundingBox[ 3 ] = bounds[ 3 ];
	m_BoundingBox[ 4 ] = bounds[ 4 ];
	m_BoundingBox[ 5 ] = bounds[ 5 ];

	if ( m_MeshTreeNode.IsNotNull() && m_MeshTreeNode->GetData() )
	{
		double pointSize;
		blMITKUtils::ComputePointSize( m_MeshTreeNode->GetData(), pointSize, 0.005 );
		this->m_DataTreeNode->SetProperty("pointsize", mitk::FloatProperty::New(pointSize));
	}
}

bool blMitkPointSelectInteractor::ExecuteAction( Action* action, mitk::StateEvent const* stateEvent )
{
	bool ok = false;//for return type bool

	//-----------------------------------------------------------------
	//checking corresponding Data; has to be a PointSet or a subclass
	mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if (pointSet == NULL)
		return false;

	//-----------------------------------------------------------------
	//get the timestep to also support 3D+T
	const mitk::Event *theEvent = stateEvent->GetEvent();
	int timeStep = 0;
	mitk::ScalarType timeInMS = 0.0;

	//-----------------------------------------------------------------
	if ( theEvent )
	{
		if (theEvent->GetSender() != NULL)
		{
			timeStep = theEvent->GetSender()->GetTimeStep();
			timeInMS = theEvent->GetSender()->GetTime();
		}
	}

	/*Each case must watch the type of the event!*/
	switch (action->GetActionId())
	{

		//-----------------------------------------------------------------
		// Check the vtkPolyData is selected
	case AcCHECKELEMENT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			mitk::Point3D selectedPoint;
			bool bPointIsOk = GetPointPosition( stateEvent, selectedPoint );

			if ( bPointIsOk )
			{
				//new Event with information YES and with the correct point
				mitk::StateEvent* newStateEvent = new mitk::StateEvent(EIDYES, posEvent);
				//call HandleEvent to leave the guard-state
				this->HandleEvent( newStateEvent );
				ok = true;
			}
			else
			{
				//new Event with information NO
				mitk::StateEvent* newStateEvent = new mitk::StateEvent(EIDNO, posEvent);
				this->HandleEvent(newStateEvent );
				ok = true;
			}

			break;
		}

		//-----------------------------------------------------------------
		// Check the selected point is hit, to change the direction
	case AcCHECKOBJECT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			mitk::Point3D selectedPoint;
			bool bPointIsOk = GetPointPosition( stateEvent, selectedPoint );


			//------------------------------------------------------------
			// In this code, we need to use a bounding mesh for the skeleton
			// and check the bounding box of this mesh
			if ( bPointIsOk )
			{
				//new Event with information YES and with the correct point
				mitk::StateEvent* newStateEvent = new mitk::StateEvent(EIDYES, posEvent);
				//call HandleEvent to leave the guard-state
				this->HandleEvent( newStateEvent );
				ok = true;
			}
			else
			{
				//new Event with information NO
				mitk::StateEvent* newStateEvent = new mitk::StateEvent(EIDNO, posEvent);
				this->HandleEvent(newStateEvent );
				ok = true;
			}

			break;
		}


		//-----------------------------------------------------------------
	case AcADDPOINT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			//---------------------------------------------------
			mitk::Point3D selectedPoint;
			bool bPointIsOk = GetPointPosition( stateEvent, selectedPoint );

			//---------------------------------------------------
			// Move last point
			if ( bPointIsOk && pointSet->GetSize() )
			{
				PointSet::PointType currentPoint;
				currentPoint = pointSet->GetPoint( 0 );

				PointOperation* doOp = new mitk::PointOperation(
					OpMOVE, timeInMS, selectedPoint, 0);

				if (m_UndoEnabled)  //write to UndoMechanism
				{
					mitk::Point3D itkPoint;
					itkPoint[0] = currentPoint[0];
					itkPoint[1] = currentPoint[1];
					itkPoint[2] = currentPoint[2];

					PointOperation* undoOp = new mitk::PointOperation(
						OpMOVE, timeInMS, itkPoint, 0);
					OperationEvent *operationEvent = new OperationEvent(pointSet, 
						doOp, undoOp, "Move point");
					m_UndoController->SetOperationEvent(operationEvent);
				}

				pointSet->ExecuteOperation(doOp);
			}
			else if ( bPointIsOk )
			{
				PointOperation* doOp = new mitk::PointOperation(
					OpINSERT, timeInMS, selectedPoint, 0, true, PTUNDEFINED);

				//---------------------------------------------------
				//Undo
				if (m_UndoEnabled)
				{
					// difference between OpDELETE and OpREMOVE is, that OpDELETE deletes 
					// a point at the end, and OpREMOVE deletes it from the given position
					// remove is better, cause we need the position to add or remove the 
					// point anyway. We can get the last position from size()
					PointOperation *undoOp = new mitk::PointOperation(
						OpREMOVE, timeInMS, selectedPoint, 0);
					OperationEvent *operationEvent = 
						new OperationEvent(pointSet, doOp, undoOp, "Add point");
					m_UndoController->SetOperationEvent(operationEvent);
				}

				//---------------------------------------------------
				//execute the Operation
				pointSet->ExecuteOperation(doOp);
			}

			ok = true;

			//---------------------------------------------------
			//UpdatePlane( pointSet->GetPoint( 0 ) );

			//---------------------------------------------------
			// Update the display
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();

			break;
		}

  // Remove Point that have been set at once. 
  case AcREMOVEPOINT:
	  {
		  if ( pointSet->GetSize() )
		  {
			  PointSet::PointType selectedPoint;
			  selectedPoint = pointSet->GetPoint( 0 );

			  PointOperation* doOp = new mitk::PointOperation(
				  OpREMOVE, timeInMS, selectedPoint, 0);

			  if (m_UndoEnabled)  //write to UndoMechanism
			  {
				  PointOperation* undoOp = new mitk::PointOperation(OpINSERT,
					  timeInMS, selectedPoint, 0, true, PTUNDEFINED );
				  OperationEvent *operationEvent = new OperationEvent(pointSet, 
					  doOp, undoOp, "Remove point");
				  m_UndoController->SetOperationEvent(operationEvent);
			  }
			  pointSet->ExecuteOperation(doOp);

		  }

		  // Update the display
		  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		  break;
	  }

		//-----------------------------------------------------------------
		// Change the selected part of the vtkPolyData, changing the color of the plane
	case AcSELECTSUBOBJECT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			if ( pointSet->GetSize() )
			{
				PointSet::PointType selectedPoint;
				selectedPoint = pointSet->GetPoint( 0 );

				mitk::Point3D itkPoint;
				itkPoint[0] = selectedPoint[0];
				itkPoint[1] = selectedPoint[1];
				itkPoint[2] = selectedPoint[2];

				m_Direction = m_Direction + 1;

				//---------------------------------------------------
				// Only to update the plane
				PointOperation* doOp = new mitk::PointOperation(
					OpMOVE, timeInMS, itkPoint, 0 );
				pointSet->ExecuteOperation(doOp);

				//---------------------------------------------------
				// Update the display
				mitk::RenderingManager::GetInstance()->RequestUpdateAll();
			}

			break;
		}

		//-----------------------------------------------------------------
	default:
		return Superclass::ExecuteAction( action, stateEvent );
	}

	return ok;
}

blMitkPointSelectInteractor::Pointer blMitkPointSelectInteractor::New( 
	const char * type, 
	mitk::DataTreeNode* dataTreeNode, 
	mitk::DataTreeNode* meshPointSet, 
	double relativeSelectedPointSize, 
	double relativeMarginAroundBoundingBox
)
{
	Pointer smartPtr = new Self( type, dataTreeNode, meshPointSet, relativeSelectedPointSize, 
		relativeMarginAroundBoundingBox);
	smartPtr->UnRegister();
	return smartPtr;
}

bool blMitkPointSelectInteractor::GetPickedPoint( 
	mitk::StateEvent const* stateEvent,
	mitk::Point3D &worldPoint )
{
	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent * > 
		(stateEvent->GetEvent());

	if (posEvent == NULL) 
		return false;

	// Get position of the mouse using PointPicking MODE
	// If mode is set to WorldPointPicking, when selecting a point
	// of a skeleton, the picking is not working unless the user
	// hits exactly at a point or line of the skeleton
	mitk::BaseRenderer* renderer = stateEvent->GetEvent()->GetSender();
	if ( !renderer )
	{
		worldPoint = posEvent->GetWorldPosition( );
		return true;
	}

	mitk::VtkPropRenderer* propRenderer = dynamic_cast<mitk::VtkPropRenderer*>( renderer );
	mitk::VtkPropRenderer::PickingMode	oldPickingMode;
	oldPickingMode = propRenderer->GetPickingMode( );
	propRenderer->SetPickingMode( mitk::VtkPropRenderer::PointPicking );
	propRenderer->PickWorldPoint(posEvent->GetDisplayPosition(), worldPoint);
	propRenderer->SetPickingMode( oldPickingMode );

	return true;
}

void blMitkPointSelectInteractor::SetCuttingType(CUT_TYPE cutting_type)
{
    m_CutType = cutting_type;
}

vtkIdType blMitkPointSelectInteractor::GetCellId()
{
	return this->m_cellId;
}


bool blMitkPointSelectInteractor::GetPointPosition( 
	mitk::StateEvent const* stateEvent, 
	mitk::Point3D &selectedPoint )
{

	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent * > 
		(stateEvent->GetEvent());


	mitk::Point3D worldPoint;
	switch (m_CutType)
	{
	case SKELETON_TYPE:	
		GetPickedPoint( stateEvent, worldPoint );
		break;
	case RING_TYPE:
		worldPoint= posEvent->GetWorldPosition();
		break;
	default:
		break;
	}



	//---------------------------------------------------
	bool bPointIsOk = false;
	if ( m_MeshTreeNode.IsNotNull() && m_MeshTreeNode->GetData() )
	{
		// Get position of the closest point in the surface
		double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };

		int iPosition = -1;
		mitk::BaseData* data = m_MeshTreeNode->GetData();
		mitk::Surface* surface = dynamic_cast<mitk::Surface*> (data);
		vtkPolyData* vtkSurface = NULL;
		if ( surface )
		{
			int timeStep = std::min( GetTimeStep(), surface->GetSizeOfPolyDataSeries( ) );
			vtkSurface = surface->GetVtkPolyData( timeStep );
		}

		iPosition = blMITKUtils::ComputeClosestPoint( stateEvent, 
			pPoint,
			vtkSurface);

		// If point found
		if ( iPosition > 0 )
		{
			vtkSmartPointer<vtkIdList> cellIds =  vtkSmartPointer<vtkIdList>::New();
			vtkSurface->GetPointCells(iPosition, cellIds);
			//! We use the first Id on the list to set the cell the point belongs to
			if ( cellIds->GetNumberOfIds() != 0)
				m_cellId = cellIds->GetId(0);
			else
				return false;

			selectedPoint[ 0 ] = vtkSurface->GetPoint(iPosition)[ 0 ];
			selectedPoint[ 1 ] = vtkSurface->GetPoint(iPosition)[ 1 ];
			selectedPoint[ 2 ] = vtkSurface->GetPoint(iPosition)[ 2 ];
			bPointIsOk = true;
		}
	}
	else
	{
		selectedPoint[ 0 ] = worldPoint[ 0 ];
		selectedPoint[ 1 ] = worldPoint[ 1 ];
		selectedPoint[ 2 ] = worldPoint[ 2 ];
		bPointIsOk = true;
	}


	//------------------------------------------------------------
	// In this code, we need to use a bounding mesh for the skeleton
	// and check the bounding box of this mesh
	if ( bPointIsOk )
	{
		if (selectedPoint[ 0 ] > m_BoundingBox[ 0 ] && selectedPoint[ 0 ] < m_BoundingBox[ 1 ] &&
			selectedPoint[ 1 ] > m_BoundingBox[ 2 ] && selectedPoint[ 1 ] < m_BoundingBox[ 3 ] &&
			selectedPoint[ 2 ] > m_BoundingBox[ 4 ] && selectedPoint[ 2 ] < m_BoundingBox[ 5 ] )
		{
			bPointIsOk = true;
		}
		else
		{
			bPointIsOk = false;
		}

	}

	return bPointIsOk;
}

float blMitkPointSelectInteractor::CalculateJurisdiction( mitk::StateEvent const* stateEvent ) const
{
	float returnValue = 0.0;
	//if it is a key event that can be handled in the current state, then return 0.5
	mitk::DisplayPositionEvent const  *disPosEvent =
		dynamic_cast <const mitk::DisplayPositionEvent *> (stateEvent->GetEvent());

	//Key event handling:
	if (disPosEvent == NULL)
	{
		//check, if the current state has a transition waiting for that key event.
		if (this->GetCurrentState( GetTimeStep() )->GetTransition(stateEvent->GetId())!=NULL)
		{
			return 0.5;
		}
		else
		{
			return 0;
		}
	}

	//on MouseMove do nothing!
	if (stateEvent->GetEvent()->GetType() == mitk::Type_MouseMove)
	{
		return 0;
	}

	//get the time of the sender to look for the right transition.
	mitk::BaseRenderer* sender = stateEvent->GetEvent()->GetSender();
	if (sender != NULL)
	{
		unsigned int timeStep = sender->GetTimeStep(m_DataTreeNode->GetData());
		//if the event can be understood and if there is a transition waiting for that event
		mitk::State const* state = this->GetCurrentState(timeStep);
		if (state!= NULL)
			if (state->GetTransition(stateEvent->GetId())!=NULL)
				returnValue = 0.5;//it can be understood

	}
	return returnValue;
}

