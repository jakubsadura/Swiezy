/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataHolder.h"
#include "coreAssert.h"



template< class T >
Core::DataHolder<T>::DataHolder( )
{
	m_initialized = false;
}

template< class T >
Core::DataHolder<T>::~DataHolder( )
{
	this->m_DataHolderDestructSignal1( m_Data );
}

template< class T >
T& Core::DataHolder<T>::GetSubject()
{
	return this->m_Data;
}


template< class T >
void Core::DataHolder<T>::SetSubject( const T& data, bool bForceNotification )
{
	doSetSubject( data, bForceNotification );
}

template< class T >
void Core::DataHolder<T>::doSetSubject( const T& data, bool bForceNotification )
{
	// If data has changed -> Update it and notify observers
	if ( !m_initialized || m_Data != data )
	{
		this->m_Data = data;
	    this->m_initialized = true;
		this->Modified();
		this->m_NewDataSignal();
		this->m_DataModifiedSignalOrNewSubject( );
		this->m_NewDataSignal1( this->m_Data );
		this->m_DataModifiedSignalOrNewSubject1( this->m_Data );
	}
	// If data has not changed and force notification -> Notify obervers on modified
	else if ( bForceNotification )
	{
		this->NotifyObservers( );
	}
}

template< class T >
typename Core::DataHolder<T>::SignalType1* 
Core::DataHolder<T>::GetSignal1( int _eventType )
{
	typename Core::DataHolder<T>::SignalType1* localSignal;
	switch( _eventType )
	{
	case DH_NEW_SUBJECT: localSignal = &m_NewDataSignal1; break;
	case DH_SUBJECT_MODIFIED:localSignal = &m_DataModifiedSignal1;break;
	case DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT: localSignal = &m_DataModifiedSignalOrNewSubject1;break;
	case DH_DESTRUCTED: localSignal = &m_DataHolderDestructSignal1; break;
	}

	return localSignal;
}

template< class T >
void Core::DataHolder<T>::NotifyObservers() const
{
	Superclass::NotifyObservers( );

	this->m_DataModifiedSignalOrNewSubject1( m_Data );
	this->m_DataModifiedSignal1( m_Data );
}

template< class T >
void Core::DataHolder<T>::OnModifiedSubject( )
{
	NotifyObservers( );
}


