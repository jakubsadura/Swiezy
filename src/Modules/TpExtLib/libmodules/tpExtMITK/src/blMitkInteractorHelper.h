/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKINTERACTORHELPER_H
#define BLMITKINTERACTORHELPER_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkPointSet.h"

namespace mitk{ class BaseRenderer; }

/**
\brief Create the proper events to simulate the user interaction with the 
interactor. This will set the interactor state machine in the correct state.

If the data of the node in the rendering tree is modified directly,
the state machine of the interactor will be in a state that will not be
the correct one.

We need the mitk::BaseRenderer to get the current TimeStep. The interactor
uses this information to work with the correct time step.

\note The event can conflict with other interactors or listeners, like the
blMitkSlicesRotator. For this reason the left mouse button events will
always be with SHIFT pressed.

\ingroup blUtilitiesMITK
\date 30 09 08
\author Xavi Planes
*/
class TPEXTLIBMITK_EXPORT blMitkInteractorHelper 
{
public:

	/**
	\brief Send the event SHIFT + left mouse button at the position p
	*/
	static void AddPoint( mitk::BaseRenderer* render, double p[3] );

	//! Simulate mitk::Key_Delete key press event
	static void RemoveSelectedPoint( mitk::BaseRenderer* render );

	/**
	\brief Send the event SHIFT + left mouse button at the position of
	the point iPoint of the pointSet
	\note if the point iPoint doesn't exist, don't do anything
	*/
	static void SelectPoint( 
						mitk::BaseRenderer* render,
						mitk::PointSet::Pointer pointSet, 
						int iPoint );

private:
	/**
	\brief Send the event SHIFT + left mouse button press at the 
	position point and event SHIFT + left mouse button release
	*/
	static void SendEventLeftMouseButton( 
						mitk::BaseRenderer* render, 
						mitk::Point3D point );

};

#endif // BLMITKINTERACTORHELPER_H
