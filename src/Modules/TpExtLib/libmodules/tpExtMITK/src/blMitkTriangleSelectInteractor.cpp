/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkTriangleSelectInteractor.h"
#include "vtkOBBTree.h"
#include "vtkMath.h"
#include "vtkCommand.h"
#include "vtkSmartPointer.h"
#include <vtkPolyData.h>

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

blMitkTriangleSelectInteractor::blMitkTriangleSelectInteractor(
	const char * type, 
	mitk::DataTreeNode* dataTreeNode,
	vtkPolyData	*meshPointSet,
	mitk::PointSet* points,
	double relativeSelectedPointSize,
	double relativeMarginAroundBoundingBox
)
: Interactor(type, dataTreeNode)
{
	pointSet = points;
	m_MeshPointSet = meshPointSet;
	if ( m_MeshPointSet )
	{
		m_MeshPointSet->Register( NULL );
	}
	m_Direction = 0;

	m_pointSize = 0.5;

	//// Find out how large the point set is, and set the mitk point size accordingly
	blMITKUtils::ComputePointSize(meshPointSet,m_pointSize ,relativeSelectedPointSize);

	meshPointSet->GetBounds(this->m_BoundingBox);
	for( int i = 0; i < 3; ++i )
	{
		double width = this->m_BoundingBox[2*i + 1] - this->m_BoundingBox[2*i];
		double delta = 0.5 * relativeMarginAroundBoundingBox * width;
		this->m_BoundingBox[2*i] -= delta;
		this->m_BoundingBox[2*i + 1] += delta;
	}

	this->m_Radius = 5*m_pointSize  ;
	
	this->m_selected = false;
	m_ImplicitSphere = vtkSphere::New();

}

blMitkTriangleSelectInteractor::~blMitkTriangleSelectInteractor()
{
	if ( m_MeshPointSet )
	{
		m_MeshPointSet->UnRegister( NULL );
	}
}

void blMitkTriangleSelectInteractor::SetBoundingBox(double bounds[ 6 ]) 
{ 
	m_BoundingBox[ 0 ] = bounds[ 0 ];
	m_BoundingBox[ 1 ] = bounds[ 1 ];
	m_BoundingBox[ 2 ] = bounds[ 2 ];
	m_BoundingBox[ 3 ] = bounds[ 3 ];
	m_BoundingBox[ 4 ] = bounds[ 4 ];
	m_BoundingBox[ 5 ] = bounds[ 5 ];
}

bool blMitkTriangleSelectInteractor::ExecuteAction( Action* action, mitk::StateEvent const* stateEvent )
{
	bool ok = false;//for return type bool

	//-----------------------------------------------------------------
	//checking corresponding Data; has to be a PointSet or a subclass
	//mitk::PointSet* pointSet = dynamic_cast<mitk::PointSet*>(m_DataTreeNode->GetData());
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
			// Remove last selected point
			if ( pointSet->GetSize() )
			{
				PointSet::PointType selectedPoint;
				selectedPoint = pointSet->GetPoint( 0 );

				mitk::Point3D itkPoint;
				itkPoint[0] = selectedPoint[0];
				itkPoint[1] = selectedPoint[1];
				itkPoint[2] = selectedPoint[2];

				PointOperation* doOp = new mitk::PointOperation(
					OpREMOVE, timeInMS, itkPoint, 0);

				if (m_UndoEnabled)  //write to UndoMechanism
				{
					PointOperation* undoOp = new mitk::PointOperation(OpINSERT,
						timeInMS, itkPoint, 0, true, PTUNDEFINED );
					OperationEvent *operationEvent = new OperationEvent(pointSet, 
						doOp, undoOp, "Remove point");
					m_UndoController->SetOperationEvent(operationEvent);
				}
				pointSet->ExecuteOperation(doOp);
			}


			//---------------------------------------------------
			mitk::Point3D worldPoint;
			//GetPickedPoint( stateEvent, worldPoint );
			worldPoint = posEvent->GetWorldPosition();


			//---------------------------------------------------
			// Get position of the closest point in the surface
			double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };
			int iPosition = m_MeshPointSet->FindPoint( pPoint );


			//---------------------------------------------------
			// Found a point -> Add it to the point set
			if (iPosition>=0)
			{
				mitk::Point3D polyDataPoint;
			/*	polyDataPoint[ 0 ] = m_MeshPointSet->GetPoint(iPosition)[ 0 ];
				polyDataPoint[ 1 ] = m_MeshPointSet->GetPoint(iPosition)[ 1 ];
				polyDataPoint[ 2 ] = m_MeshPointSet->GetPoint(iPosition)[ 2 ];*/
				polyDataPoint[ 0 ] = pPoint[ 0 ];
				polyDataPoint[ 1 ] = pPoint[ 1 ];
				polyDataPoint[ 2 ] = pPoint[ 2 ];


				m_ImplicitSphere->SetCenter(pPoint);
				m_ImplicitSphere->SetRadius(m_Radius);
				PointOperation* doOp = new mitk::PointOperation(
					OpINSERT, timeInMS, polyDataPoint, 0, true, PTUNDEFINED);
				
				//---------------------------------------------------
				//execute the Operation
				pointSet->ExecuteOperation(doOp);
			}

			ok = true;

			//---------------------------------------------------
			// Update the display
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();
			break;
		}

		//-----------------------------------------------------------------
		// Change the radius
	case AcUPDATEMESH:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			mitk::Point3D worldPoint;
			worldPoint = posEvent->GetWorldPosition();


			//---------------------------------------------------
			// Get position of the closest point in the surface
			double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };
			double qPoint[ 3 ] = {pointSet->GetPoint(0)[0],pointSet->GetPoint(0)[1],pointSet->GetPoint(0)[2]};

		/*	double distance;
			distance =sqrt( vtkMath::Distance2BetweenPoints(pPoint,qPoint));
			SetRadius(distance);*/
			m_ImplicitSphere->SetCenter(pPoint);
			m_ImplicitSphere->SetRadius(m_Radius);
			PointOperation* doOp = new mitk::PointOperation(
				OpINSERT, timeInMS, worldPoint, 0, true, PTUNDEFINED);

			//---------------------------------------------------
			//execute the Operation
			pointSet->ExecuteOperation(doOp);
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();

			break;
		}
	case AcNEWPOINT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

	//		m_ImplicitSphere->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, 0);
			m_Radius = 0;
			//---------------------------------------------------
			//execute the Operation
			//pointSet->ExecuteOperation(doOp);
		
			m_selected = true;
			ok = true;
			//---------------------------------------------------
			// Update the display
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();
			

			break;
		}
	case AcREMOVEPOINT:
		{
			mitk::DisplayPositionEvent const *posEvent = 
				dynamic_cast < const mitk::DisplayPositionEvent * > 
				(stateEvent->GetEvent());

			if (posEvent == NULL) 
				return false;

			//---------------------------------------------------
			mitk::Point3D worldPoint;
			//GetPickedPoint( stateEvent, worldPoint );
			worldPoint = posEvent->GetWorldPosition();


			//---------------------------------------------------
			// Get position of the closest point in the surface
			double pPoint[ 3 ] = { worldPoint[ 0 ], worldPoint[ 1 ], worldPoint[ 2 ] };
			int iPosition = m_MeshPointSet->FindPoint( pPoint );


			//---------------------------------------------------
			// Found a point -> Remove it from the point set
			if (iPosition>=0)
			{
				mitk::Point3D polyDataPoint;
				polyDataPoint[ 0 ] = pPoint[ 0 ];
				polyDataPoint[ 1 ] = pPoint[ 1 ];
				polyDataPoint[ 2 ] = pPoint[ 2 ];


				PointOperation* doOp = new mitk::PointOperation(
					OpREMOVE, timeInMS, polyDataPoint, 0, true, PTUNDEFINED);

			}
			m_selected = true;
			ok = true;
			//---------------------------------------------------
			// Update the display
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();


			break;
		}
	
		//-----------------------------------------------------------------
	default:
		return Superclass::ExecuteAction( action, stateEvent );
	}

	return ok;
}

blMitkTriangleSelectInteractor::Pointer blMitkTriangleSelectInteractor::New( 
	const char * type, 
	mitk::DataTreeNode* dataTreeNode, 
	vtkPolyData *meshPointSet,
	mitk::PointSet* points,
	double relativeSelectedPointSize, 
	double relativeMarginAroundBoundingBox
)
{
	Pointer smartPtr = new Self( type, dataTreeNode, meshPointSet,points,
		relativeSelectedPointSize, 
		relativeMarginAroundBoundingBox);
	smartPtr->UnRegister();
	return smartPtr;
}

bool blMitkTriangleSelectInteractor::GetPickedPoint( 
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
	mitk::VtkPropRenderer* propRenderer = dynamic_cast<mitk::VtkPropRenderer*>( renderer );
	mitk::VtkPropRenderer::PickingMode	oldPickingMode;
	oldPickingMode = propRenderer->GetPickingMode( );
	propRenderer->SetPickingMode( mitk::VtkPropRenderer::PointPicking );

	propRenderer->PickWorldPoint(posEvent->GetDisplayPosition(), worldPoint);

	propRenderer->SetPickingMode( oldPickingMode );

	return true;
}

void blMitkTriangleSelectInteractor::SetRadius(double radius)
{
	m_Radius = radius*m_pointSize ;
}

double blMitkTriangleSelectInteractor::GetRadius()
{
	return m_Radius;
}

vtkSphere* blMitkTriangleSelectInteractor::GetImplicitSphere()
{
	return m_ImplicitSphere;
}

