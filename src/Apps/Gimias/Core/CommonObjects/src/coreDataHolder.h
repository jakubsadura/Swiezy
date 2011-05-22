/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _COREDATAHOLDER_H
#define _COREDATAHOLDER_H

#include "gmCommonObjectsWin32Header.h"
#include "coreDataHolderBase.h"
#include "boost/bind.hpp"
#include "boost/signals.hpp"

namespace Core{

/**
\brief This class extends the DataHolderBase class with functions GetSubject
and SetSubject. It is templated over the m_Data type.

The main advantage of this implementation is templated and is possible 
to encapsulate any kind of object. 

\section Examples Examples

To create a DataHolder and add an observer function when the subject is modified:
\code

class ShapeScaleProcessor
{
public:
	//!
	typedef Core::DataHolder< float > ParametersHolder;

public:
	//!
	Example( );

	//!
	OnModified( );

	//!
	ParametersHolder::Pointer m_ParametersHolder;
}

ShapeScaleProcessor::ShapeScaleProcessor( )
{
	m_ParametersHolder = Core::DataHolder<float>::New();
	m_ParametersHolder->SetSubject( 0.5 );
	m_ParametersHolder->AddObserver( 
		this, 
		&ShapeScaleProcessor::OnModified,
		DH_SUBJECT_MODIFIED );

}

\endcode

\ingroup gmCommonObjects
\author Maarten Nieber
\date 31/1/2008
*/

template< class T >
class DataHolder : public DataHolderBase
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DataHolder, DataHolderBase);

	//!
	typedef boost::signal1<void,T> SignalType1;

	//! Get the m_Data.
	T& GetSubject();

	//! Set the m_Data. If bForceNotification then always call NotifyObservers()
	void SetSubject( const T& data, bool bForceNotification = false );

	//! Notify the observers that the m_Data has been modified.
	virtual void NotifyObservers(void) const;

	//!
	SignalType1* GetSignal1( int _eventType );

	/** Convenience function. Connects member function \a 
	_recipientMemberFunction of \a _recipient to _event
	and calls the \a _recipientMemberFunction
	*/
	template< class Observer > 
		ConnectionType AddObserver1(
		Observer* _recipient, 
		void (Observer::*_recipientMemberFunction)( T ), 
		int _eventType = DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT )
	{
		// Call callback funtion
		(_recipient->*_recipientMemberFunction)( GetSubject( ) );
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		return GetSignal1(_eventType)->connect( 
			boost::bind(_recipientMemberFunction, _recipient, _1) );
	}

	/** Disconnects member function \a _recipientMemberFunction 
	of \a _recipient to _event and calls the \a _recipientMemberFunction 
	with NULL parameter
	*/
	template< class Observer > 
		void RemoveObserver1(
		Observer* _recipient, 
		void (Observer::*_recipientMemberFunction)( T ), 
		int _eventType = DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT )
	{
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		GetSignal1(_eventType)->disconnect( 
			boost::bind(_recipientMemberFunction, _recipient, _1) );

		// Set it to NULL
		(_recipient->*_recipientMemberFunction)( NULL );
	}

	//!
	void OnModifiedSubject( );

protected:
    DataHolder( );
    
	virtual ~DataHolder( );

	//!
	void doSetSubject( const T& data, bool bForceNotification = false );

private:
	//!
	T m_Data;
	
	bool m_initialized;

	//! Called when a new m_Data has been set in the DataHolder.
	SignalType1 m_NewDataSignal1;
	/** Called when someone has performed modifications on the m_Data in 
	the DataHolder (but there is not a new m_Data).
	*/
	SignalType1 m_DataModifiedSignal1;
	//! Join m_NewDataSignal and m_DataModifiedSignal
	SignalType1 m_DataModifiedSignalOrNewSubject1;
	//! Called by the DataHolder when it destructs.
	SignalType1 m_DataHolderDestructSignal1;

};

//! Creates a new data holder in \a dataHolder and sets its subject to p
template <class T>
void Initialize( typename Core::DataHolder<T>::Pointer& dataHolder, T p )
{
	dataHolder = Core::DataHolder<T>::New();
	dataHolder->SetSubject( p );
};

/** Explicit specialization for Core::Object
When the Core::SmartPointerObject is modified, the holder is also modified
and notifies all observers
*/
template<>
void DataHolder<Core::SmartPointerObject::Pointer>::SetSubject( const Core::SmartPointerObject::Pointer& data, bool bForceNotification );



} // Core

#include "coreDataHolder.txx"

#endif //_COREDATAHOLDER_H

