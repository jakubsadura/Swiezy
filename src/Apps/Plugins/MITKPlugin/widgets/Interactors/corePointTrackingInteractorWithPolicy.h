/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef POINTTRACKINGINTERACTORWITHPOLICY_H
#define POINTTRACKINGINTERACTORWITHPOLICY_H

#include "corePluginMacros.h"
#include "coreObject.h"
#include <CILabNamespaceMacros.h>
#include <CILabItkMacros.h>
#include <mitkCommon.h>
#include <mitkStateMachine.h>
#include <mitkPointSet.h>
#include <mitkPositionTracker.h>
#include <itkPoint.h>
#include "coreSmartPointerMacros.h"

namespace Core{

class Operation;
class OperationActor;

/**
\brief This class is used by PointTrackingInteractorWithPolicy. See for details.
Users of PointTrackingInteractorWithPolicy should create a subclass of 
this class and overload the OnNewPoint 
function.

DisplayPixelValuePolicy: This class uses the coreImageWidget as
it interface. An image (VolumeImage Holder) and a imageInformationHolder 
should be passed to the policy. 
An example of how to use the class follows:

\code
//create a new policy
this->intensityValueDisplayPolicy = DisplayPixelValuePolicy::New();
//Set the image to the policy 
this->intensityValueDisplayPolicy->SetImage(_inputImage->GetSubject());
//Set the image information holder
this->intensityValueDisplayPolicy->SetInformationHolder(_imageInformationHolder);

this->mouseTracker = PointTrackingInteractorWithPolicy::New(
	"PositionTracker", 
	static_cast<PointTrackingInteractorPolicy::Pointer>(intensityValueDisplayPolicy) );

//The interactor must be registered as a listener
mitk::GlobalInteraction::GetInstance()->AddListener( 
	static_cast<mitk::StateMachine::Pointer>(mouseTracker) );
\endcode

\ingroup gmInteractors
\author Maarten Nieber & Martin Bianculli
20 feb 2008
*/

class PLUGIN_EXPORT PointTrackingInteractorPolicy 
	: public Core::SmartPointerObject
{
public:
	//! Create itk typedefs and new function for PointTrackingInteractorPolicy
	coreDeclareSmartPointerTypesMacro(
		PointTrackingInteractorPolicy, 
		Core::SmartPointerObject);
	coreClassNameMacro(PointTrackingInteractorPolicy);

	//! Type of the coordinate that is received by this class.
	typedef mitk::PointSet::PointType PointType;

	//! Respond to the event that a new point has been selected in 
	//! PointTrackingInteractorWithPolicy.
	//! This function is pure virtual; it must be implemented in derived classes.
	virtual void OnNewPoint(const PointType& _worldCoordinate) = 0;

	//!
	PointTrackingInteractorPolicy(void);

private:
	coreDeclareNoCopyConstructors(PointTrackingInteractorPolicy);
};


/**
This interactor stores a pointer to a PointTrackingInteractorWithPolicy 
(member variable m_Policy).
Each time the interactor detects that a new point has been "selected" with 
the mouse, it calls
m_Policy->OnNewPoint(worldCoordinate), where worldCoordinate is the world 
coordinate of the "selected" point.
Users of PointTrackingInteractorWithPolicy should program their own 
PointTrackingInteractorPolicy to specify what happens
when a new point has been "selected".

This class copies the structure of mitkPositionTracker.

Example:

DisplayPixelValuePolicy::Pointer displayPixelValuePolicy = 
	DisplayPixelValuePolicy::New();
PointTrackingInteractorWithPolicy* pointTrackingInteractor = 
	ew PointTrackingInteractorWithPolicy(displayPixelValuePolicy);

\author Maarten Nieber
20 feb 2008
*/
 

class PLUGIN_EXPORT PointTrackingInteractorWithPolicy 
	: public mitk::StateMachine
{
public:
	coreDeclareSmartPointerClassMacro2Param
	(
		PointTrackingInteractorWithPolicy,
		mitk::StateMachine,
		const char*, 
		PointTrackingInteractorPolicy::Pointer
	);

protected:
	/**
	\param onNewPointPolicy - Call the OnNewPoint function of this object 
	each time there is a new selected point.
	\param type: string, that describes the StateMachine-Scheme to take 
	from all SM (see XML-File)
	*/
	PointTrackingInteractorWithPolicy
	(
		const char* type,
		PointTrackingInteractorPolicy::Pointer onNewPointPolicy
	);

	//! Overloaded
	virtual bool ExecuteAction(
		mitk::Action* action, 
		mitk::StateEvent const* stateEvent);

private:
	PointTrackingInteractorPolicy::Pointer m_Policy;

	coreDeclareNoCopyConstructors(PointTrackingInteractorWithPolicy);
};

} // Core

#endif //POINTTRACKINGINTERACTORWITHPOLICY_H
