/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkPointSetInteractor.h"

#include "mitkPointSetInteractor.h"
#include "mitkPointOperation.h"
#include "mitkPositionEvent.h"
#include "mitkPointSet.h"
#include "mitkStatusBar.h"
#include "mitkDataTreeNode.h"
#include "mitkInteractionConst.h"
#include "mitkAction.h"
#include "mitkStateEvent.h"
#include "mitkOperationEvent.h"
#include "mitkUndoController.h"
#include "mitkStateMachineFactory.h"
#include "mitkStateTransitionOperation.h"
#include "mitkBaseRenderer.h"
#include "mitkRenderingManager.h"
#include "mitkVtkPropRenderer.h"
#include "mitkEventMapper.h"
#include "mitkMaterialProperty.h"
#include "vtkIntArray.h"
#include "blMITKUtils.h"
#include "vtkLine.h"

blMitkPointSetInteractor::blMitkPointSetInteractor( 
	const char * type, 
	mitk::DataTreeNode* dataTreeNode, 
	int n /*= -1*/ ) 
	: mitk::PointSetInteractor( type, dataTreeNode, n )
{

	m_MeshPointSet = NULL;
	m_bRestrictAddPointPosition = false;
	m_bIncrementalAdd = false;
	m_CurrentAddPosition = 0;
	m_bAddingPoint = false;
	SetMaxPoints( n );
	m_ScalarVector = vtkIntArray::New( );
	m_ScalarVector->Resize( 1 );
	m_ScalarVector->SetValue( 0, 0 );

	for ( int i = 0 ; i < 3 ; i++ )
	{
		m_LastClickedNormal[ i ] = 0;
		m_LastClickedOrigin[ i] = 0;
	}
	m_LastClickedPlaneIsOk = false;
	m_ptSize = -1; //leave mitk default
	m_color[0] = 1; m_color[1] = 0; m_color[2] = 0; //mitk default red

}

blMitkPointSetInteractor::~blMitkPointSetInteractor()
{
	m_ScalarVector->Delete( );
}

bool blMitkPointSetInteractor::CheckCurrentStateIsMovePointSpace()
{
	const mitk::State *state = mitk::StateMachine::GetCurrentState( GetTimeStep( ) );
	//std::cout 
	//	<< "State: " << state->GetId() << " (" << state->GetName() << ") "
	//	<< "Adding: " << m_bAddingPoint << std::endl;
	// "move Point Space" ID="12"
	// "move Point Full" ID="22"
	// "increase scalar" ID = 51
	// "decrease scalar" ID = 52
	return state->GetId() == 12 || state->GetId() == 22 || state->GetId() == 51 || state->GetId() == 52;
}

bool blMitkPointSetInteractor::CheckCurrentActionIsAddPoint()
{
	return m_bAddingPoint;
}

bool blMitkPointSetInteractor
::ExecuteAction( mitk::Action* action, mitk::StateEvent const* stateEvent )
{
  bool ok = false;//for return type bool

  //checking corresponding Data; has to be a PointSet or a subclass
  mitk::PointSet* pointSet = 
    dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
  if ( pointSet == NULL )
  {
    return false;
  }

  //get the timestep to also support 3D+T
  const mitk::Event *theEvent = stateEvent->GetEvent();
  mitk::ScalarType timeInMS = 0.0;

  //check if the current timestep has to be changed
  if ( theEvent )
  {
	  if (theEvent->GetSender() != NULL)
	  {
		  //additionaly to m_TimeStep we need timeInMS to satisfy the execution of the operations
		  timeInMS = theEvent->GetSender()->GetTime();
	  }
  }


  //for reading on the points, Id's etc
  mitk::PointSet::DataType *itkPointSet = pointSet->GetPointSet( m_TimeStep );
  if ( itkPointSet == NULL )
  {
    return false;
  }

  mitk::PointSet::PointsContainer *points = itkPointSet->GetPoints();

  GetInteraction2DSlicePosition( stateEvent );
  if (m_MeshPointSet.IsNotNull() && m_MeshPointSet->GetData() )
  {

	  // resize point for mesh
	  blMITKUtils::ComputePointSize(m_MeshPointSet->GetData(),m_ptSize,0.005);

	  m_DataTreeNode->SetProperty("pointsize", mitk::FloatProperty::New( m_ptSize ) );
	  m_DataTreeNode->ReplaceProperty("contoursize", mitk::FloatProperty::New(m_ptSize/2));

	  // Set precision to select a point or to add a new point
	  SetPrecision( (unsigned int) ( m_ptSize * 2 ) );

	  blMITKUtils::ComputeBoundingBox( m_MeshPointSet->GetData(), m_BoundingBox );
  }
  else
  {
	  if(m_ptSize>0)
	  {
		  SetPrecision( (unsigned int) ( 4*m_ptSize ) );
		  
		  m_DataTreeNode->ReplaceProperty("pointsize", mitk::FloatProperty::New( m_ptSize ) );
		  m_DataTreeNode->ReplaceProperty("contoursize", mitk::FloatProperty::New(m_ptSize/2));
	  }
  }	  
  
  mitk::ColorProperty::Pointer colorProp = dynamic_cast<mitk::ColorProperty*>(m_DataTreeNode->GetPropertyList(NULL)->GetProperty("contourcolor"));
  if(colorProp.IsNull())
	  colorProp = mitk::ColorProperty::New();

  colorProp->SetColor(m_color[0],m_color[1],m_color[2]);
  m_DataTreeNode->GetPropertyList(NULL)->SetProperty("contourcolor",colorProp);
  //std::cout 
	 // << "StateEventID: " << stateEvent->GetId() << ", "
	 // << "Action: " << action->GetActionId() << ", "
	 // << "Next State: " << GetCurrentState()->GetName()
	 // << std::endl;
  

  /*Each case must watch the type of the event!*/
  switch (action->GetActionId())
  {

	  //-----------------------------------------------------------------
	  // Check the vtkPolyData is selected
  case mitk::AcCHECKELEMENT:
	  {
		  mitk::DisplayPositionEvent const *posEvent = 
			  dynamic_cast < const mitk::DisplayPositionEvent * > 
			  (stateEvent->GetEvent());

		  // For meshes
		  if ( m_bRestrictAddPointPosition && m_MeshPointSet.IsNotNull() )
		  {
			  if (posEvent == NULL) 
				  return false;

			  mitk::Point3D selectedPoint;
			  bool bPointIsOk = GetPointPosition( stateEvent, selectedPoint );

			  // Point is inside the mesh
			  if ( bPointIsOk )
			  {
				  //new Event with information YES and with the correct point

				  // Create a new event like if it was in the 2D view
				  // and let manage it by the superclass
				  mitk::Point2D displPoint;
				  displPoint[0] = selectedPoint[0]; displPoint[1] = selectedPoint[1];
				  mitk::PositionEvent const* newPosEvent = 
					  new mitk::PositionEvent(posEvent->GetSender(), mitk::Type_None, 
						mitk::BS_NoButton, mitk::BS_NoButton, mitk::Key_none, displPoint, selectedPoint);
				  mitk::StateEvent* newStateEvent =
					  new mitk::StateEvent(mitk::EIDYES, newPosEvent);
				  Superclass::ExecuteAction( action, newStateEvent );
				  delete newStateEvent;
				  ok = true;
			  }
			  else
			  {
				  //new Event with information NO -> Point is not OK
				  mitk::StateEvent* newStateEvent = new mitk::StateEvent(mitk::EIDNO, NULL);
				  this->HandleEvent(newStateEvent );
				  ok = true;
			  }
		  }
		  else
		  {
			// For images
			return Superclass::ExecuteAction( action, stateEvent );
		  }
	  }
	  break;

  case mitk::AcCHECKPOINTSETSIZE:
	  mitk::StateEvent* newStateEvent;
	  if ( pointSet->GetSize( m_TimeStep ) == 0 )
	  {
		  newStateEvent = new mitk::StateEvent(mitk::EIDPOINTSETEMPTY, stateEvent->GetEvent());
	  }
	  else if ( m_iMaxPoints > 0 && pointSet->GetSize( m_TimeStep ) >= m_iMaxPoints )
	  {
		  newStateEvent = new mitk::StateEvent(mitk::EIDPOINTSETFULL, stateEvent->GetEvent());
	  }
	  else
	  {
		  newStateEvent = new mitk::StateEvent(mitk::EIDPOINTSETSPACELEFT, stateEvent->GetEvent());
	  }

	  this->HandleEvent( newStateEvent );
	  delete newStateEvent;
	  ok = true;

	  break;

  case mitk::AcCHECKNMINUS1:
		// Behavior is the same as the parent class
		// or always add the point to the current position

		// number of points not limited->pass on
		// "Amount of points in Set is smaller then N-1"
	    // if m_bIncrementalAdd == false -> Only change current point position
	    // and don't add any new point
		if ( m_iMaxPoints < 0 || !m_bIncrementalAdd )
		{
			mitk::StateEvent* newStateEvent = 
				new mitk::StateEvent(mitk::EIDSTSMALERNMINUS1, stateEvent->GetEvent());
			this->HandleEvent( newStateEvent );
			delete newStateEvent;
			ok = true;
		}
		else
		{
			if (pointSet->GetSize( m_TimeStep ) < m_iMaxPoints-1 )
				//pointset after addition won't be full
			{
				mitk::StateEvent* newStateEvent = 
					new mitk::StateEvent(mitk::EIDSTSMALERNMINUS1, stateEvent->GetEvent());
				this->HandleEvent( newStateEvent );
				delete newStateEvent;
				ok = true;
			}
			else
				//after the addition of a point, the container will be full
			{
				mitk::StateEvent* newStateEvent = 
					new mitk::StateEvent(mitk::EIDSTLARGERNMINUS1, stateEvent->GetEvent());
				this->HandleEvent(newStateEvent );
				delete newStateEvent;
				ok = true;
			}//else
		}
	break;

  case mitk::AcADDPOINT:
    // Declare two operations: one for the selected state: deselect the last 
    // one selected and select the new one the other operation is the add 
    // operation: There the first empty place have to be found and the new
    // point inserted into that space
    {
      mitk::DisplayPositionEvent const *posEvent = 
        dynamic_cast < const mitk::DisplayPositionEvent * > 
          (stateEvent->GetEvent());

      // Check if it is a DisplayEvent thrown in a 3D window. Then the
      // z-information is missing. Returning false might end in the state
      // full, but the last point couldn't be added, so the set wouldn't be
      // full. So a extra Action that checks the operationtype has been added.
      if ( posEvent == NULL )
      {
        return false;
      }

	  // Reset scalar value
	  m_ScalarVector->SetValue( 0, 0 );

	  mitk::Point3D worldPoint;
	  mitk::Point3D selectedPoint;
	  bool bPointFound = GetPickedPoint( stateEvent, worldPoint );
	  bool bPointIsOk = GetPointPosition( stateEvent, selectedPoint );

	  m_bAddingPoint = true;

	  //---------------------------------------------------
	  // Found a point -> Add it to the point set
	  if ( bPointFound || bPointIsOk )
	  {
		// undo-supported deselect of all points in the DataList; if List is
		// empty, then nothing will be unselected
		this->UnselectAll( m_TimeStep, timeInMS );

		// find the position, the point is to be added to: first entry with
		// empty index. If the Set is empty, then start with 0. if not empty, 
		// then take the first index not occupied
		int lastPosition = 0;
		if ( !points->empty() && m_bIncrementalAdd )
		{
			mitk::PointSet::PointsIterator it, end;
			it = points->Begin();
			end = points->End();
			while( it != end )
			{
			if (!points->IndexExists(lastPosition))
				break;
			++it;
			++lastPosition;
			}
		}
		else
		{
			lastPosition = m_CurrentAddPosition;
		}

		if (bPointIsOk) worldPoint = selectedPoint;
		mitk::PointOperation* doOp = new mitk::PointOperation(
			mitk::OpINSERT, timeInMS, worldPoint, lastPosition);

		if (m_UndoEnabled)
		{
			// difference between OpDELETE and OpREMOVE is, that OpDELETE deletes 
			// a point at the end, and OpREMOVE deletes it from the given position
			// remove is better, cause we need the position to add or remove the 
			// point anyway. We can get the last position from size()
			mitk::PointOperation *undoOp = new mitk::PointOperation(
			mitk::OpREMOVE, timeInMS, worldPoint, lastPosition);
			mitk::OperationEvent *operationEvent = 
			new mitk::OperationEvent(pointSet, doOp, undoOp, "Add point");
			m_UndoController->SetOperationEvent(operationEvent);
		}

		// execute the Operation
		pointSet->ExecuteOperation(doOp);
		delete doOp;

		// Go to next state (move point)
		mitk::StateEvent* newStateEvent = 
			new mitk::StateEvent(mitk::EIDYES, stateEvent->GetEvent());
		this->HandleEvent( newStateEvent );
		delete newStateEvent;
		ok = true;

	  }

      ok = true;

	  m_bAddingPoint = false;

      // Update the display
      mitk::RenderingManager::GetInstance()->RequestUpdateAll();
      break;
    }
  case mitk::AcINCREASE:
	  {
		  int iValue = m_ScalarVector->GetValue( 0 );
		  m_ScalarVector->SetValue( 0, iValue + 1 );
		  m_ScalarVector->Modified();
	  }
	  break;
  case mitk::AcDECREASE:
	  {
		  int iValue = m_ScalarVector->GetValue( 0 );
		  m_ScalarVector->SetValue( 0, iValue - 1 );
		  m_ScalarVector->Modified();
	  }
	  break;
  default:
    return Superclass::ExecuteAction( action, stateEvent );
  }

  return ok;
}

bool blMitkPointSetInteractor::GetPickedPoint( 
	mitk::StateEvent const* stateEvent,
	mitk::Point3D &worldPoint )
{
	mitk::PositionEvent const *posEvent = 
		dynamic_cast < const mitk::PositionEvent * > 
		(stateEvent->GetEvent());

	if (posEvent == NULL) 
		return false;


	// Always get the point from the world position
	// because sometimes we simulate the events and fill this filed manually
	worldPoint = posEvent->GetWorldPosition( );


	//---------------------------------------------------
	// Get position of the closest point in the surface
	if ( m_bRestrictAddPointPosition && m_MeshPointSet.IsNotNull() && m_MeshPointSet->GetData() )
	{
		double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };

		int iPosition = -1;
		mitk::BaseData* data = m_MeshPointSet->GetData();
		mitk::Surface* surface = dynamic_cast<mitk::Surface*> (data);
		vtkPolyData* vtkSurface = NULL;
		if ( surface )
		{
			int timeStep = std::min( GetTimeStep(), surface->GetSizeOfPolyDataSeries( ) );
			vtkSurface = surface->GetVtkPolyData( timeStep );
			iPosition = vtkSurface->FindPoint( pPoint );
		}

		if ( iPosition>=0 && vtkSurface )
		{
			mitk::Point3D polyDataPoint;
			worldPoint[ 0 ] = vtkSurface->GetPoint(iPosition)[ 0 ];
			worldPoint[ 1 ] = vtkSurface->GetPoint(iPosition)[ 1 ];
			worldPoint[ 2 ] = vtkSurface->GetPoint(iPosition)[ 2 ];
		}
	}

	return true;
}

void blMitkPointSetInteractor::SetRestrictAddPointPosition( 
						bool bVal,
						mitk::DataTreeNode::Pointer meshPointSet )
{
	m_bRestrictAddPointPosition = bVal;
	m_MeshPointSet = meshPointSet;
}

bool blMitkPointSetInteractor::GetIncrementalAdd() const
{
	return m_bIncrementalAdd;
}

void blMitkPointSetInteractor::SetIncrementalAdd( bool val )
{
	m_bIncrementalAdd = val;
}

void blMitkPointSetInteractor::SetMaxPoints( int iMaxPoints )
{
	m_iMaxPoints = iMaxPoints;
}

void blMitkPointSetInteractor::GetInteraction2DSlicePosition(
						mitk::StateEvent const* stateEvent )
{
	m_LastClickedPlaneIsOk = false;
	if ( stateEvent->GetEvent() == NULL )
	{
		return;
	}

	mitk::BaseRenderer* renderer = stateEvent->GetEvent()->GetSender();
	if ( renderer == NULL ) 
	{
		return;
	}

	const mitk::PlaneGeometry* planeGeometry;
	planeGeometry = dynamic_cast<const mitk::PlaneGeometry*> (renderer->GetCurrentWorldGeometry2D() );
	if ( planeGeometry == NULL )
	{
		return;
	}

	m_LastClickedNormal = planeGeometry->GetNormal( );
	m_LastClickedOrigin = planeGeometry->GetOrigin();
	m_LastClickedPlaneIsOk = true;

}

bool blMitkPointSetInteractor::GetLastClickedNormal( mitk::Vector3D &lastClickedNormal ) const
{
	lastClickedNormal = m_LastClickedNormal;
	return m_LastClickedPlaneIsOk;
}

bool blMitkPointSetInteractor::GetLastClickedOrigin( mitk::Point3D &lastClickedOrigin ) const
{
	lastClickedOrigin = m_LastClickedOrigin;
	return m_LastClickedPlaneIsOk;
}

vtkIntArray * blMitkPointSetInteractor::GetScalarVector() const
{
	return m_ScalarVector;
}

void blMitkPointSetInteractor::SetMeasurementPointSet( bool val )
{
	if (val)
	{
		if (m_DataTreeNode == NULL) return;
		blMITKUtils::ApplyMeasurementProperties(m_DataTreeNode);
	}
}

bool blMitkPointSetInteractor::GetPointPosition( 
	mitk::StateEvent const* stateEvent, 
	mitk::Point3D &selectedPoint )
{

	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent * > 
		(stateEvent->GetEvent());


	mitk::Point3D worldPoint;
	worldPoint= posEvent->GetWorldPosition();
	
	//---------------------------------------------------
	bool bPointIsOk = false;
	if ( m_bRestrictAddPointPosition && m_MeshPointSet.IsNotNull() && m_MeshPointSet->GetData() )
	{
		// Get position of the closest point in the surface
		double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };

		mitk::BaseData* data = m_MeshPointSet->GetData();
		mitk::Surface* surface = dynamic_cast<mitk::Surface*> (data);
		vtkPolyData* vtkSurface = NULL;
		if ( surface )
		{
			int timeStep = std::min( GetTimeStep(), surface->GetSizeOfPolyDataSeries( ) );
			vtkSurface = surface->GetVtkPolyData( timeStep );
		}

		int iPosition = blMITKUtils::ComputeClosestPoint( stateEvent, 
															pPoint,
															vtkSurface);

		// If point found
		if ( iPosition > 0 && vtkSurface )
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
/*	if ( bPointIsOk && m_MeshPointSet != NULL )
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

	}*/

	return bPointIsOk;
}


void blMitkPointSetInteractor::SetPointSize(double ptSize)
{
	m_ptSize=ptSize;
}

double blMitkPointSetInteractor::GetPointSize()
{
	return m_ptSize;
}

void blMitkPointSetInteractor::SetContourColor(double color[3])
{
	m_color[0] = color[0];
	m_color[1] = color[1];
	m_color[2] = color[2];
}

//! Get line color
double *blMitkPointSetInteractor::GetContourColor()
{
	return m_color;
}
