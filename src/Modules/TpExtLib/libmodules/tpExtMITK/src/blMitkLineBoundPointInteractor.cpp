/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkLineBoundPointInteractor.h"
#include "mitkDisplayPositionEvent.h"
#include <mitkInteractionConst.h>
#include <mitkDataTreeNode.h>
#include <mitkPointSet.h>
#include "mitkStateEvent.h"
#include "mitkPointOperation.h"
#include "mitkBaseRenderer.h"
#include "mitkVtkPropRenderer.h"

#include "blMITKUtils.h"

#include "vtkLine.h"

blMitkLineBoundPointInteractor::blMitkLineBoundPointInteractor( 
	mitk::DataTreeNode* node,
	const PointType& offset,
	const PointType& direction
)
: mitk::Interactor("LineBoundPointInteractor", node)
{
	m_Offset = offset;
	m_Direction = direction;
	m_Precision = 5;
}

bool blMitkLineBoundPointInteractor::ExecuteAction(mitk::Action* action, mitk::StateEvent const* stateEvent)
{
	bool ok = false;

	//-----------------------------------------------------------------
	//checking corresponding Data; has to be a PointSet or a subclass
	mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if (pointSet == NULL)
		return false;

	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent* >( stateEvent->GetEvent() );

	//get the timestep to also support 3D+T
	const mitk::Event *theEvent = stateEvent->GetEvent();
	int timeStep = 0;
	mitk::ScalarType timeInMS = 0.0;

	if ( theEvent )
	{
		if (theEvent->GetSender() != NULL)
		{
			timeStep = theEvent->GetSender()->GetTimeStep(pointSet);
			timeInMS = theEvent->GetSender()->GetTime();
		}
	}

	switch( action->GetActionId() )
	{
	//! Check that the mouse click is close to the point
	case mitk::AcCHECKELEMENT :
		{
			if (posEvent == NULL) 
				return false;

			mitk::Point3D worldPoint;
			int position = GetPickedPoint( stateEvent, worldPoint );

			// Find closest point to mouse click
			bool lineBoundPointIsSelected = ( position == 0 );

			if( lineBoundPointIsSelected )
			{
				// Select the point
				mitk::Point3D noPoint;
				noPoint.Fill( 0 );
				mitk::PointOperation *doOp = new mitk::PointOperation(
					mitk::OpSELECTPOINT, timeInMS, noPoint, position);
				pointSet->ExecuteOperation(doOp);
				delete doOp;

				// Update the views with the selected colors
				mitk::RenderingManager::GetInstance()->RequestUpdateAll();
			}


			// Go to next state
			mitk::StateEvent* newStateEvent = NULL;
			if( lineBoundPointIsSelected )
			{
				newStateEvent = new mitk::StateEvent(mitk::EIDYES, posEvent);
			}
			else
			{
				newStateEvent = new mitk::StateEvent(mitk::EIDNO, posEvent);
			}

			HandleEvent(newStateEvent );
			ok = true;
		}
		break;

	case mitk::AcMOVEPOINT:
		{
			mitk::Point3D worldPoint;
			GetPickedPoint( stateEvent, worldPoint );
			RepositionPoint( worldPoint, timeInMS );
		}
		break;

	case mitk::AcDESELECT:
		{
			int pointID = pointSet->SearchSelectedPoint( );
			if ( pointID == -1 )
			{
				return -1;
			}

			mitk::Point3D noPoint;
			noPoint.Fill( 0 );
			mitk::PointOperation *doOp = new mitk::PointOperation(
				mitk::OpDESELECTPOINT, timeInMS, noPoint, pointID);

			pointSet->ExecuteOperation(doOp);

			// Update the views with the selected colors
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		}
		break;

	}

	return ok;
}


//##Documentation
//## overwritten cause this class can handle it better!
float blMitkLineBoundPointInteractor
::CalculateJurisdiction(mitk::StateEvent const* stateEvent) const
{
	float returnValue = 0.0;
	//if it is a key event that can be handled in the current state, then return 0.5
	mitk::DisplayPositionEvent const  *disPosEvent =
		dynamic_cast <const mitk::DisplayPositionEvent *> (stateEvent->GetEvent());

	if ( disPosEvent == NULL )
	{
		return 0.5;
	}

	//if the event can be understood and if there is a transition waiting for that event
	if (GetCurrentState( GetTimeStep() )->GetTransition(stateEvent->GetId())!=NULL)
	{
		returnValue = 0.5;
	}

	// If the event can be understood and there's a point close -> return 1
	// This allows to use several interactors and check all of them
	mitk::PointSet *pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if ( returnValue > 0 && disPosEvent->GetSender() && pointSet != NULL )
	{
		int timeStep = disPosEvent->GetSender()->GetTimeStep();

		//if we have one point or more, then check if the have been picked
		if ( pointSet->GetSize( timeStep ) > 0 )
		{

			mitk::Point3D worldPoint;
			int position = GetPickedPoint( stateEvent, worldPoint );

			if ( position > -1 )
			{
				returnValue = 1.0;
			}

		}
	}

	return returnValue;
}

void blMitkLineBoundPointInteractor::RepositionPoint( 
	mitk::Point3D &newPosition,
	mitk::ScalarType timeInMS /*= 0.0*/ )
{
	mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if (pointSet == NULL)
		return;

	// Get selected point
	int pointID = pointSet->SearchSelectedPoint( );
	if ( pointID == -1 )
	{
		return;
	}

	// Modify position and update observers
	m_Offset = newPosition;
	mitk::PointOperation* doOp = new mitk::PointOperation(mitk::OpMOVE, timeInMS, 
		newPosition, pointID);
	pointSet->ExecuteOperation( doOp );
	delete doOp;

	mitk::Point3D noPoint;
	doOp = new mitk::PointOperation(mitk::OpSELECTPOINT, timeInMS, noPoint, pointID);
	pointSet->ExecuteOperation( doOp );
	delete doOp;

	// Update the views with the selected colors
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

blMitkLineBoundPointInteractor::PointType blMitkLineBoundPointInteractor::GetOffset() const
{
	return m_Offset;
}

void blMitkLineBoundPointInteractor::SetOffset( PointType val )
{
	if( m_Offset == val )
		return;
	m_Offset = val;

	// Update the node
	mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if (pointSet == NULL)
		return;
	// Don't update the observers
	pointSet->SetPoint( 0, m_Offset );
}

blMitkLineBoundPointInteractor::PointType blMitkLineBoundPointInteractor::GetDirection() const
{
	return m_Direction;
}

void blMitkLineBoundPointInteractor::SetDirection( PointType val )
{
	if( m_Direction == val )
		return;
	m_Direction = val;
}

float blMitkLineBoundPointInteractor::GetPrecision() const
{
	return m_Precision;
}

void blMitkLineBoundPointInteractor::SetPrecision( float val )
{
	m_Precision = val;
}

int blMitkLineBoundPointInteractor::GetPickedPoint( 
	mitk::StateEvent const* stateEvent, 
	mitk::Point3D &worldPoint,
	mitk::ScalarType timeInMS /*= 0.0*/ ) const
{
	int position = -1;

	mitk::DisplayPositionEvent const *posEvent = 
		dynamic_cast < const mitk::DisplayPositionEvent* >( stateEvent->GetEvent() );
	if (posEvent == NULL) 
		return -1;

	mitk::BaseRenderer* renderer = stateEvent->GetEvent()->GetSender();
	if ( !renderer )
	{
		return -1;
	}

	mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
	if (pointSet == NULL)
		return -1;

	mitk::VtkPropRenderer* propRenderer = dynamic_cast<mitk::VtkPropRenderer*>( renderer );

	if ( renderer->GetMapperID() == 1 )
	{
		worldPoint[ 0 ] = posEvent->GetWorldPosition( )[ 0 ];
		worldPoint[ 1 ] = posEvent->GetWorldPosition( )[ 1 ];
		worldPoint[ 2 ] = posEvent->GetWorldPosition( )[ 2 ];

		// Search closest point
		position = pointSet->SearchPoint( worldPoint, m_Precision, timeInMS );
	}
	else
	{
		// Get intersection between mouse pointer line and point movement line
		double selectionX;
		double selectionY;
		double selectionZ;
		selectionX = posEvent->GetDisplayPosition()[ 0 ];
		selectionY = posEvent->GetDisplayPosition()[ 1 ];
		selectionZ = 0;
		double p1World[4];
		double p2World[4];
		blMITKUtils::ComputeRay( 
			selectionX, 
			selectionY, 
			selectionZ, 
			propRenderer->GetVtkRenderer(), 
			p1World, p2World );


		// Get the projection of the clicked point to the movement 
		// restriction line of the interactor
		double point1[ 3 ] = {m_Offset[ 0 ], m_Offset[ 1 ], m_Offset[ 2 ]};
		double point2[ 3 ] = {
			m_Offset[ 0 ] + m_Direction[ 0 ], 
			m_Offset[ 1 ] + m_Direction[ 1 ], 
			m_Offset[ 2 ] + m_Direction[ 2 ]};

		double closestPt1[3];
		double closestPt2[3];
		double t1;
		double t2;
		vtkLine::DistanceBetweenLines( p1World, p2World, point1, point2, closestPt1, closestPt2, t1, t2 );

		worldPoint[ 0 ] = closestPt2[ 0 ];
		worldPoint[ 1 ] = closestPt2[ 1 ];
		worldPoint[ 2 ] = closestPt2[ 2 ];

		// Check the closest point
		double currentPoint[ 3 ] = {m_Offset[ 0 ], m_Offset[ 1 ], m_Offset[ 2 ]};
		double distance = vtkLine::DistanceToLine( currentPoint, p1World, p2World );
		if ( distance < m_Precision * m_Precision )
		{
			position = 0;
		}
	}

	return position;
}

blMitkLineBoundPointInteractor::~blMitkLineBoundPointInteractor()
{
}

void blMitkLineBoundPointInteractor::OnNodeDeleted()
{
}
