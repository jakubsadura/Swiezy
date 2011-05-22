/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataHolderBase_H
#define _coreDataHolderBase_H

#include "gmCommonObjectsWin32Header.h"
#include "coreObject.h"
#include "boost/bind.hpp"
#include "boost/signals.hpp"

namespace Core{

//!
class DataHolderConnection;

//! Types of events that cause a signal from the DataHolder to its observers.
enum DataHolderEventType {
	DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT, 
	DH_SUBJECT_MODIFIED, 
	DH_NEW_SUBJECT, 
	DH_DESTRUCTED,
	DH_MAX};

/**
\brief Base class for DataHolders. Contains the signals and observer registration part.

This class is an implementation of Subject-Observer pattern. 

It's implemented using boost.

\ingroup gmCommonObjects
\author Maarten Nieber
\date 10 Jan 2008
*/

class GMCOMMONOBJECTS_EXPORT DataHolderBase : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(DataHolderBase, Core::SmartPointerObject)

	typedef boost::signal0<void> SignalType;
	//!
	friend class DataHolderConnection;
	//!
	typedef boost::signals::connection ConnectionType;


public:
	//! First thing it does is fire m_DataHolderDestructSignal.
	virtual ~DataHolderBase(void);

	//! Notify the observers that the m_Data has been modified.
	virtual void NotifyObservers(void) const;

	/** Convenience function. Connects member function \a 
	_recipientMemberFunction of \a _recipient to _event.
	*/
	template< class T > 
		ConnectionType AddObserver(
			T* _recipient, 
			void (T::*_recipientMemberFunction)( void ), 
			int _eventType = DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT )
	{
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		return GetSignal(_eventType)->connect( 
			boost::bind(_recipientMemberFunction, _recipient) );
	}

	/** Disconnects member function \a _recipientMemberFunction 
	of \a _recipient to _event.
	*/
	template< class T > 
		void RemoveObserver(
		T* _recipient, 
		void (T::*_recipientMemberFunction)( void ), 
		int _eventType = DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT )
	{
		// Use boost::bind directly in this call, otherwise
		// trackable will not work
		GetSignal(_eventType)->disconnect( 
			boost::bind(_recipientMemberFunction, _recipient) );
	}

	//!
	SignalType* GetSignal( int _eventType );

	//!
	std::string GetName() const;
	void SetName(std::string val);

protected:

protected:
	//! Name of this holder. For debuggin purposes
	std::string m_Name;
	//! Called when a new m_Data has been set in the DataHolder.
	SignalType m_NewDataSignal;
	/** Called when someone has performed modifications on the m_Data in 
	the DataHolder (but there is not a new m_Data).
	*/
	SignalType m_DataModifiedSignal;
	//! Join m_NewDataSignal and m_DataModifiedSignal
	SignalType m_DataModifiedSignalOrNewSubject;
	//! Called by the DataHolder when it destructs.
	SignalType m_DataHolderDestructSignal;
};


} // Core

#endif //_coreDataHolderBase_H
