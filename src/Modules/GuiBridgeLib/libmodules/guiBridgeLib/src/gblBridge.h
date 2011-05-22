/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLBRIDGE_H
#define GBLBRIDGE_H

/**
This module contains the basic classes for working with the GUI/Controller Bridge pattern.
The idea behind this module is that GUI's do not directly call methods in the controller.
Instead, they use c.f. bridge->CallEvent("DoIt"). 
The controller does not link itself directly to a GUI. Instead, it registers its member functions with the bridge,
c.f. SetBridgeEventSlot(bridge, "DoIt", controllerPtr, &Controller::DoIt).
This de-couples the events in the GUI from the functions in the controller.

\author Maarten Nieber
\date 1 Sep 2007
*/

#include "gblValueConverter.h"
#include "gblException.h"
#include "gblValueProxy.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include <string>
#include <map>
#include "GuiBridgeLibWin32Header.h"
#include "CILabNamespaceMacros.h"

CILAB_BEGIN_NAMESPACE(gbl)

/**
Holds a pointer to a function in a controller.
I would prefer to use boost::signal<void ()>::slot_function_type here, but this creates a problem: Qt and boost signals/slot
do not like each other because of the macros in Qt for 'signals' and 'slot'. Therefore, the boost signals and slots library
is hidden completely in the cpp file.
*/
typedef boost::function<void ()> SlotType;

/**
This class is used in Bridge when you try to call SetValueProxy with an identifier that is already used.
\author Maarten Nieber
\date 1 Sep 007
*/
gblDeclareExceptionMacro(BridgeException, std::bad_cast)

//! Forward declaration
class Bridge;

//! Shared pointer to a bridge.
typedef boost::shared_ptr<Bridge> BridgePtr;

/**
A bridge is used to communicate between a GUI and a Controller.
This is explained in 
\author Maarten Nieber
\date 1 Sep 007
*/

class GUIBRIDGELIB_EXPORT Bridge
{
public:
	//!
	friend bool InitBridge(Bridge& _leftHandSide, Bridge _rightHandSide);

	//!
	Bridge();

	//!
	virtual ~Bridge();

	/**
	Returns a new bridge object that shares its internals with this bridge object.
	\param _morePrefix - The new bridge object automatically prefixes any value or event id with _prefix + ".". 
	For example, calling this->GetText("Hello.Bye") is the same as any of the following: 
	- (*this)["Hello.Bye"]->CallEvent("Hola"), 
	- (*this)["Hello"]->CallEvent("Bye.Hola")
	- (*this)["Hello"]["Bye"]->CallEvent("Hola")
	*/
	Bridge operator[](const std::string& _prefix) const;

	//! Returns the string that is automatically prefix to value and event identifiers (see also operator[]).
	const std::string& GetAutoPrefix() const;

	//! Sets the string that is automatically prefix to value and event identifiers (see also operator[]).
	void SetAutoPrefix(const std::string& _prefix);

	//! Make a shallow copy from \a _rsh (which is purposely not a const argument, for this class will share its imp with _rhs). 
	void ShallowCopyFrom(gbl::Bridge _rhs);

	//! Sets the slot to be performed for the event with \a id.
	//! Throws BridgeException if this already has an slot.
	//! \see also CallEvent.
	void SetEventSlot(const std::string& id, SlotType slot);

	//! Signal to the bridge that event with \id must be executed.
	void CallEvent(const std::string& id);
	
	bool HasEvent(const std::string& _id) const;
	//bool HasValue(const std::string& _id) const;

	//! Performs a shallow copy! Prefer to use the ShallowCopy, which states explicitly what it is doing.
	Bridge(const Bridge& _rhs);

	//! Performs a shallow copy! Prefer to use the ShallowCopy, which states explicitly what it is doing.
	const Bridge& operator=(const Bridge& _rhs);

private:
	/**
	This is a pointer to the object that implements the behaviour of the bridge object. The pointer may be shared with other
	bridge instances (i.e. shallow copies).
	*/
	struct Impl;
	boost::shared_ptr<Impl> impl;
	/**
	This string is automatically prefixed to any value of event id. While the impl pointer may be shared with other
	bridge objects, the autoPrefix is unique to the bridge instance.
	*/
	std::string m_autoPrefix;
};

/**
Creates slot to be called by the bridge in case of an event with \a id.

\author Maarten Nieber
\date 1 Sep 007
*/
template< class T >
void SetBridgeEventSlot(Bridge _bridge, const std::string& _id, T* _recipient, void (T::*_recipientMemberFunction)( void ) )
{
	boost::function<void ()> slot;
	slot = boost::bind(_recipientMemberFunction, _recipient);
	_bridge.SetEventSlot(_id, slot);
}

/**
Helper function that sets _leftHandSide = _rightHandSide.
Use this function in GUI and controllers to initialise the bridge with a bridge pointer from outside,
with the following behaviour:

- if _leftHandSide is not NULL, throws gbl::Exception.
- else if _rightHandSide is NULL, throws gbl::Exception.
- else if _leftHandSide == _rightHandSide, does nothing and returns false.
- else, sets _leftHandSide = _rightHandSide and returns true.
*/
bool InitBridge(Bridge& _leftHandSide, Bridge _rightHandSide);

CILAB_END_NAMESPACE(gbl)

#endif //GBLBRIDGE_H
