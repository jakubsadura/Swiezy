/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataHolderBase.h"
#include "coreAssert.h"

Core::DataHolderBase::~DataHolderBase()
{
	this->m_DataHolderDestructSignal();
}

void Core::DataHolderBase::NotifyObservers() const
{
	this->Modified();
	this->m_DataModifiedSignalOrNewSubject( );
	this->m_DataModifiedSignal();
}

Core::DataHolderBase::SignalType* 
Core::DataHolderBase::GetSignal( int _eventType )
{
	SignalType* signal;
	switch( _eventType )
	{
	case DH_NEW_SUBJECT: signal = &m_NewDataSignal; break;
	case DH_SUBJECT_MODIFIED:signal = &m_DataModifiedSignal;break;
	case DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT: signal = &m_DataModifiedSignalOrNewSubject;break;
	case DH_DESTRUCTED: signal = &m_DataHolderDestructSignal; break;
	}

	return signal;
}

std::string Core::DataHolderBase::GetName() const
{
	return m_Name;
}

void Core::DataHolderBase::SetName( std::string val )
{
	m_Name = val;
}
