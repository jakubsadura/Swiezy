/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityHolderConnection.h"
#include "coreAssert.h"

using namespace Core;

// #define _DEBUG_HOLDER_CONNECTION

Core::ConnectionConfiguration::ConnectionConfiguration( )
{
	m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ACTION_NONE;
	m_EventAction[ DH_SUBJECT_MODIFIED ] = ACTION_NONE;
	m_EventAction[ DH_NEW_SUBJECT ] = ACTION_NONE;
	m_EventAction[ DH_DESTRUCTED ] = ACTION_BREAK_CONNECTION;
}

void Core::DataEntityHolderConnection::Connect(
	Core::DataEntityHolder* _dataHolder1, 
	Core::DataEntityHolder* _dataHolder2,
	CONNECTION_CONFIGURATION config )
{
	// If the connection already exists, remove it
	DataEntityHolderConnection* connection;
	connection = _dataHolder1->FindConnection( _dataHolder2 );
	if ( connection )
	{
		delete connection;
	}

	Core::ConnectionConfiguration config1;
	Core::ConnectionConfiguration config2;

	switch ( config )
	{
	case CONNECTION_DATALIST_AND_PROCESSOR_OUT:
		config1.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_PROPAGATE;
		config1.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		config2.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		break;
	case CONNECTION_DATALIST_AND_PROCESSOR_IN:
		config1.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_PROPAGATE;
		config1.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		config2.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		break;
	case CONNECTION_PROCESSOR_IN_AND_PROCESSOR_IN:
		config1.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config1.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_PROPAGATE;
		config1.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_SUBJECT_MODIFIED ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_NEW_SUBJECT ] = ConnectionConfiguration::ACTION_NONE;
		config2.m_EventAction[ DH_DESTRUCTED ] = ConnectionConfiguration::ACTION_BREAK_CONNECTION;
		break;
	}

	new DataEntityHolderConnection(_dataHolder1, _dataHolder2, config1, config2);
}

Core::DataEntityHolderConnection::DataEntityHolderConnection( 
	DataEntityHolder* _dataHolder1, 
	DataEntityHolder* _dataHolder2,
	ConnectionConfiguration config1,
	ConnectionConfiguration config2 )
{
	coreAssertMacro(_dataHolder1);
	coreAssertMacro(_dataHolder2);

	m_DataHolder1 = _dataHolder1;
	m_DataHolder2 = _dataHolder2;

	_dataHolder1->AddConnection( this );
	_dataHolder2->AddConnection( this );

	AddObservers( _dataHolder1, _dataHolder2, config1, m_Connection1 );
	AddObservers( _dataHolder2, _dataHolder1, config2, m_Connection2 );
}

Core::DataEntityHolderConnection::~DataEntityHolderConnection()
{
	m_DataHolder1->RemoveConnection( this );
	m_DataHolder2->RemoveConnection( this );
}

void Core::DataEntityHolderConnection::AddObservers( 
	DataEntityHolder* _dataHolder1, 
	DataEntityHolder* _dataHolder2, 
	ConnectionConfiguration config,
	HolderConnection::Pointer connectionArray[ DH_MAX ] )
{
	// _dataHolder1
	for ( int i = 0 ; i < DH_MAX ; i++ )
	{
		DataHolderEventType eventType = DataHolderEventType( i );

		ConnectionConfiguration::ACTION_TYPE action;
		action = config.m_EventAction[ eventType ];

		HolderConnection::Pointer connection = HolderConnection::New( );
		connection->m_DataHolder1 = _dataHolder1;
		connection->m_DataHolder2 = _dataHolder2;

		switch ( action )
		{
		case ConnectionConfiguration::ACTION_NONE:
			break;
		case ConnectionConfiguration::ACTION_PROPAGATE: 
			{
				switch( eventType )
				{
				case DH_SUBJECT_MODIFIED:
					connection->m_Connection = _dataHolder1->AddObserver( 
						connection.GetPointer(), 
						&HolderConnection::PropagateModifiedSignal, 
						eventType );
					break;
				case DH_NEW_SUBJECT:
					connection->m_Connection = _dataHolder1->AddObserver( 
						connection.GetPointer(), 
						&HolderConnection::PropagateNewSubjectSignal, 
						eventType );
					break;
				}
			}
			break;
		case ConnectionConfiguration::ACTION_BREAK_CONNECTION: 
			connection->m_Connection = _dataHolder1->AddObserver( 
				this, 
				&DataEntityHolderConnection::BreakConnection, 
				eventType );
			break;
		}

		connectionArray[ i ] = connection;
	}
}

DataEntityHolder* Core::DataEntityHolderConnection::GetDataHolder1() const
{
	return m_DataHolder1;
}

DataEntityHolder* Core::DataEntityHolderConnection::GetDataHolder2() const
{
	return m_DataHolder2;
}

void Core::DataEntityHolderConnection::BreakConnection()
{
	delete this;
}

void Core::HolderConnection::PropagateModifiedSignal()
{
	Register();

	bool update = false;

	//Core::DataEntity::Pointer sourceDataEntity = m_DataHolder1->GetSubject( );
	//if ( sourceDataEntity.GetPointer( ) != NULL )
	//{
	//	update = sourceDataEntity->GetMTime() > m_DataHolder2->GetMTime();
	//}
	update = m_DataHolder1->GetMTime( ) > m_DataHolder2->GetMTime( );

	if ( update )
	{
		m_Connection.block();
		#ifdef _DEBUG_HOLDER_CONNECTION
		{
			std::cout 
				<< "PropagateModifiedSignal: " 
				<< m_DataHolder1->GetName() << " ---> "
				<< m_DataHolder2->GetName() << std::endl;
		}
		#endif
		m_DataHolder2->NotifyObservers( );
		m_Connection.unblock();
	}

	UnRegister();
}

void Core::HolderConnection::PropagateNewSubjectSignal()
{
	Register();

	bool update = false;

	//Core::DataEntity::Pointer sourceDataEntity = m_DataHolder1->GetSubject( );
	//if ( sourceDataEntity.GetPointer( ) == NULL &&
	//	m_DataHolder2->GetSubject( ).GetPointer( ) != NULL )
	//{
	//	update = true;
	//}
	//else if ( sourceDataEntity.GetPointer( ) != NULL )
	//{
	//	update = sourceDataEntity->GetMTime() > m_DataHolder2->GetMTime();
	//}
	update = m_DataHolder1->GetMTime( ) > m_DataHolder2->GetMTime( );

	if ( update )
	{
		m_Connection.block();
		#ifdef _DEBUG_HOLDER_CONNECTION
		{
			std::cout 
				<< "PropagateNewSubjectSignal: " 
				<< m_DataHolder1->GetName() << " ---> "
				<< m_DataHolder2->GetName() << std::endl;
		}
		#endif
		m_DataHolder2->SetSubject( m_DataHolder1->GetSubject( ) );
		m_Connection.unblock();
	}

	UnRegister();
}

Core::HolderConnection::HolderConnection()
{

}

Core::HolderConnection::~HolderConnection()
{
	m_Connection.disconnect( );
}
