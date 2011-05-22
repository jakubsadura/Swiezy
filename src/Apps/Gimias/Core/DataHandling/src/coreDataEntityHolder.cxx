/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityHolder.h"
#include "coreDataEntityHolderConnection.h"

namespace Core{

void Initialize( DataEntityHolder::Pointer& dataHolder, DataEntity::Pointer p )
{
	dataHolder = DataEntityHolder::New();
	dataHolder->SetSubject( p );
}

}

Core::DataEntityHolder::~DataEntityHolder()
{
	// We need to send this signal before destroying this object
	this->m_DataHolderDestructSignal();

	if ( GetSubject().IsNotNull( ) )
	{
		GetSubject()->RemoveObserverOnModified< DataEntityHolder >( 
			this,
			&DataHolder<Core::DataEntity::Pointer>::OnModifiedSubject );
	}
}

void Core::DataEntityHolder::AddConnection( DataEntityHolderConnection* connection )
{
	m_connectionList.push_back( connection );
}

void Core::DataEntityHolder::RemoveConnection( DataEntityHolderConnection* connection )
{
	m_connectionList.remove( connection );
}

void Core::DataEntityHolder::DeleteAllConnections()
{
	while ( m_connectionList.size() )
	{
		DataEntityHolderConnection* connection;
		connection = m_connectionList.front();

		// The destructor will remove itself from the list
		delete connection;
	}
}

Core::DataEntityHolderConnection* 
Core::DataEntityHolder::FindConnection( DataEntityHolder* holder )
{
	std::list<DataEntityHolderConnection*>::iterator it;
	it = m_connectionList.begin();

	while ( it != m_connectionList.end( ) )
	{
		if (  ( (*it)->GetDataHolder1() == this && (*it)->GetDataHolder2() == holder )
			||( (*it)->GetDataHolder2() == this && (*it)->GetDataHolder1() == holder ) )
		{
			return *it;
		}
		it++;
	}

	return NULL;
}


void Core::DataEntityHolder::SetSubject( 
	const Core::DataEntity::Pointer& data, bool bForceNotification )
{
	m_NotifyingObservers = true;
	if ( GetSubject().IsNotNull( ) )
	{
		GetSubject()->RemoveObserverOnModified< DataEntityHolder >( 
			this,
			&DataHolder<Core::DataEntity::Pointer>::OnModifiedSubject );
	}

	doSetSubject( data, bForceNotification );

	if ( data.IsNotNull( ) )
	{
		data->AddObserverOnModified< DataEntityHolder >( 
			this,
			&DataHolder<Core::DataEntity::Pointer>::OnModifiedSubject );
	}
	m_NotifyingObservers = false;
}

Core::DataEntityHolder::DataEntityHolder()
{
	m_NotifyingObservers = false;
}

