/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePointTrackingInteractorWithPolicy.h"
#include <mitkBaseRenderer.h>
#include <mitkDataTree.h>
#include <mitkDisplayPositionEvent.h>
#include <mitkAction.h>
#include <mitkStateEvent.h>
#include <mitkUndoController.h>
#include <mitkInteractionConst.h>
#include <mitkPointSet.h>
#include <mitkRenderingManager.h>
#include <mitkVector.h> // for PointDataType 

using namespace Core;

PointTrackingInteractorWithPolicy::PointTrackingInteractorWithPolicy
(
	const char * type,
	PointTrackingInteractorPolicy::Pointer onNewPointPolicy
)
: mitk::StateMachine(type)
{
	this->m_Policy = onNewPointPolicy;
}

bool PointTrackingInteractorWithPolicy::ExecuteAction
(
	mitk::Action* /*action*/,
	mitk::StateEvent const* stateEvent
)
{
	const mitk::DisplayPositionEvent* displayPositionEvent = 
		dynamic_cast<const mitk::DisplayPositionEvent*>(stateEvent->GetEvent());

	if(displayPositionEvent != NULL && this->m_Policy.IsNotNull() )
	{
		this->m_Policy->OnNewPoint(displayPositionEvent->GetWorldPosition());
	}
	return true;
}

Core::PointTrackingInteractorPolicy::PointTrackingInteractorPolicy( void )
{
}
