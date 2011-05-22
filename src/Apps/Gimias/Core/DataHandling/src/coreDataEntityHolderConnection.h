/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataEntityHolderConnection_H
#define _coreDataEntityHolderConnection_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityHolder.h"

namespace Core{

enum CONNECTION_CONFIGURATION{
	//! DataEntity list and processor input
	CONNECTION_DATALIST_AND_PROCESSOR_IN,
	//! DataEntity list and processor output
	CONNECTION_DATALIST_AND_PROCESSOR_OUT,
	//! Processor input and processor input
	CONNECTION_PROCESSOR_IN_AND_PROCESSOR_IN
};


/** Configuration of connection
\ingroup gmCommonObjects
\author Xavi Planes
\date 2 Feb 2010
*/
class GMDATAHANDLING_EXPORT ConnectionConfiguration
{
public:
	enum ACTION_TYPE{
		ACTION_NONE,
		//! Propagate input holder notification to an output holder
		ACTION_PROPAGATE,
		//! Break connection between holders
		ACTION_BREAK_CONNECTION
	};

	//! Define an action for each event
	ACTION_TYPE m_EventAction[ DH_MAX ];

	//! Default configuration
	ConnectionConfiguration( );
};

class HolderConnection : public Core::SmartPointerObject
{
public:

	coreDeclareSmartPointerClassMacro( HolderConnection, Core::SmartPointerObject );

	//! Propagate DH_SUBJECT_MODIFIED signal to destination holder
	void PropagateModifiedSignal(  );

	//! Call holder2->SetSubject( holder1->GetSubject( ), true );
	void PropagateNewSubjectSignal( );

private:
	//!
	HolderConnection( );;

	//!
	~HolderConnection( );;

public:
	//!
	boost::signals::connection m_Connection;

	//! Source holder
	DataEntityHolder* m_DataHolder1;

	//! Destination holder
	DataEntityHolder* m_DataHolder2;
};

/**
\brief This class propagates the signals from holder1 to holder2.

Signals:
- DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT: Not used
- DH_SUBJECT_MODIFIED: Propagate signal
- DH_NEW_SUBJECT: Propagate subject
- DH_DESTRUCTED: Always break connection

\note Use the function Core::ConnectNotifications( ) to create an instance 
of this class because the destructor will call "delete this" and this
is only valid when using new operator at constructor.

\note This class should not keep the data holders alive. 
Therefore, a raw pointer is used instead of
a smart pointer. When the DataHolder destructs, it will notify the 
DataEntityHolderConnection.

\ingroup gmCommonObjects
\author Maarten Nieber
\date 10 Jan 2008
*/

class GMDATAHANDLING_EXPORT DataEntityHolderConnection : public Core::Object
{
public:
	typedef DataEntityHolderConnection Self;

	coreClassNameMacro(DataEntityHolderConnection)

	//!
	static void Connect(
		DataEntityHolder* _dataHolder1, 
		DataEntityHolder* _dataHolder2,
		CONNECTION_CONFIGURATION config );

	/**
	\param _dataHolder1 First data holder, may not be NULL.
	\param _dataHolder2 Second data holder, may not be NULL.
	\param config1 Parameters
	*/
	DataEntityHolderConnection(
		DataEntityHolder* _dataHolder1, 
		DataEntityHolder* _dataHolder2,
		ConnectionConfiguration config1,
		ConnectionConfiguration config2);

	//! Disconnect all m_Connection
	~DataEntityHolderConnection();

	//!
	DataEntityHolder* GetDataHolder1() const;

	//!
	DataEntityHolder* GetDataHolder2() const;

private:

	//! destroy this
	void BreakConnection();

	//!
	void AddObservers(
		DataEntityHolder* _dataHolder1, 
		DataEntityHolder* _dataHolder2,
		ConnectionConfiguration config,
		HolderConnection::Pointer connection[ DH_MAX ] );

private:

	//! Observer connection to dataholder1 signals
	HolderConnection::Pointer m_Connection1[ DH_MAX ];

	//! Observer connection to dataholder2 signals
	HolderConnection::Pointer m_Connection2[ DH_MAX ];

	//! Source holder
	DataEntityHolder* m_DataHolder1;

	//! Destination holder
	DataEntityHolder* m_DataHolder2;
};

} // Core


#endif //_coreDataEntityHolderConnection_H
