/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREDATAENTITYHOLDER_H
#define COREDATAENTITYHOLDER_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntity.h"
#include "coreDataHolder.h"

namespace Core{

class DataEntityHolderConnection;

class GMDATAHANDLING_EXPORT DataEntityHolder : public DataHolder<DataEntity::Pointer>
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DataEntityHolder, DataHolder<DataEntity::Pointer>);

	//!
	void AddConnection( DataEntityHolderConnection* connection );

	//!
	void RemoveConnection( DataEntityHolderConnection* connection );

	//!
	DataEntityHolderConnection* FindConnection( DataEntityHolder* holder );

	//!
	void DeleteAllConnections( );

	//!
	virtual void SetSubject( const DataEntity::Pointer& data, bool bForceNotification = false );
	
private:
	//!
	DataEntityHolder( );

	virtual ~DataEntityHolder( );

private:
	//!
	std::list<DataEntityHolderConnection*> m_connectionList;

	//!
	bool m_NotifyingObservers;
};

//! Creates a new data holder in \a dataHolder and sets its subject to p
void GMDATAHANDLING_EXPORT Initialize( DataEntityHolder::Pointer& dataHolder, DataEntity::Pointer p );

} // Core

#include "coreDataEntityHolderConnection.h"

#endif //COREDATAENTITYHOLDER_H
