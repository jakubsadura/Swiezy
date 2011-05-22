/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreObject_H
#define coreObject_H

#include "gmCommonObjectsWin32Header.h"
#include "coreCommonMacros.h"
#include "coreSmartPointerMacros.h"

#include "itkLightObject.h"
#include "itkTimeStamp.h"

#include "boost/signals.hpp"
#include "boost/bind.hpp"

namespace Core
{

/** 
\brief Abstract class that provides a common point for all the class 
hierarchy, because all classes in the Core inherit from it

Boost.Signals can automatically track the lifetime of objects involved 
in signal/slot connections, including automatic disconnection of slots 
when objects involved in the slot call are destroyed using trackable.

\ingroup gmCommonObjects
\author Juan Antonio Moya
\date 03 Jan 2008
*/
class GMCOMMONOBJECTS_EXPORT Object : public boost::signals::trackable
{
public:
	coreClassNameMacro(Object)

	//! Define virtual destructor to avoid warning
	virtual ~Object( );;
};


/** 
\brief Abstract class that provides base functionalities for handling 
objects with SmartPointers (see itk::LightObject).
It also provides a common point for all the class hierarchy, because all 
classes in the Core inherit from this Object class (or either Core::Object).

It also has Modified( ) function to notify observers on modified. 

\ingroup gmCommonObjects
\author Juan Antonio Moya
\date 03 Jan 2008
*/
class GMCOMMONOBJECTS_EXPORT SmartPointerObject : public itk::LightObject , public Object
{
public:
	coreDeclareSmartPointerClassMacro(SmartPointerObject, itk::LightObject);
	typedef boost::signal0<void> ModifiedSignal;
	typedef boost::signal1<void,SmartPointerObject*> SignalType1;

	/** Return this objects modified time.  */
	virtual unsigned long GetMTime() const;

	/** Update the modification time for this object. Many filters rely on the
	* modification time to determine if they need to recompute their data.  */
	virtual void Modified() const;

	//!
	template <class T> 
	boost::signals::connection 
	AddObserverOnModified(T* observer, void (T::*slotFunction)(void))
	{
		return m_OnModified.connect( boost::bind(slotFunction, observer) );
	}

	//!
	template< class T > 
	void RemoveObserverOnModified( T* _recipient, void (T::*slotFunction)( void ) )
	{
		m_OnModified.disconnect( boost::bind(slotFunction, _recipient) );
	}

	/** Convenience function. Connects member function \a 
	_recipientMemberFunction of \a _recipient to _event
	and calls the \a _recipientMemberFunction
	*/
	template< class Observer > 
		boost::signals::connection AddObserver1(
		Observer* _recipient, 
		void (Observer::*_recipientMemberFunction)( SmartPointerObject* ) )
	{
		// Call callback funtion
		(_recipient->*_recipientMemberFunction)( this );
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		return m_DataModifiedSignal1.connect( 
			boost::bind(_recipientMemberFunction, _recipient, _1) );
	}

	/** Disconnects member function \a _recipientMemberFunction 
	of \a _recipient to _event and calls the \a _recipientMemberFunction 
	with NULL parameter
	*/
	template< class Observer > 
		void RemoveObserver1(
		Observer* _recipient, 
		void (Observer::*_recipientMemberFunction)( SmartPointerObject* ) )
	{
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		m_DataModifiedSignal1.disconnect( 
			boost::bind(_recipientMemberFunction, _recipient, _1) );

		// Set it to NULL
		(_recipient->*_recipientMemberFunction)( NULL );
	}


protected:
	SmartPointerObject(void);
	virtual ~SmartPointerObject(void);;

private:
	coreDeclareNoCopyConstructors(SmartPointerObject);

	/** Keep track of modification time. */
	mutable itk::TimeStamp m_MTime;

	// SignalTypeId handling
	ModifiedSignal m_OnModified;

	//! Modified signal with this as parameter
	SignalType1 m_DataModifiedSignal1;
};

}

#endif
