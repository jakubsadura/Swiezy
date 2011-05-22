/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataHolderConnection.h"
#include "coreAssert.h"

Core::DataHolderConnection::DataHolderConnection( DataHolderBase* _dataHolder1, DataHolderBase* _dataHolder2 )
{
	coreAssertMacro(_dataHolder1);
	coreAssertMacro(_dataHolder2);

	this->m_DataHolderInfo1.dataHolder = _dataHolder1;
	this->m_DataHolderInfo1.connectionToDataModifiedSignal = 
		_dataHolder1->AddObserver(this, &Self::m_DataOfDataHolder1Modified, Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	this->m_DataHolderInfo1.connectionToNewDataSignal = 
		_dataHolder1->AddObserver(this, &Self::m_BreakConnection, Core::DH_NEW_SUBJECT);
	this->m_DataHolderInfo1.connectionToDestructedSignal = 
		_dataHolder1->AddObserver(this, &Self::m_BreakConnection, Core::DH_DESTRUCTED);

	this->m_DataHolderInfo2.dataHolder = _dataHolder2;
	this->m_DataHolderInfo2.connectionToDataModifiedSignal = 
		_dataHolder2->AddObserver(this, &Self::m_DataOfDataHolder2Modified, Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	this->m_DataHolderInfo2.connectionToNewDataSignal = 
		_dataHolder2->AddObserver(this, &Self::m_BreakConnection, Core::DH_NEW_SUBJECT);
	this->m_DataHolderInfo2.connectionToDestructedSignal = 
		_dataHolder2->AddObserver(this, &Self::m_BreakConnection, Core::DH_DESTRUCTED);
}

void Core::DataHolderConnection::m_BreakConnection()
{
	// Disconnect takes place in the destructor.
	delete this;
}

void Core::DataHolderConnection::m_DataOfDataHolder1Modified()
{
	this->m_DataHolderInfo2.connectionToDataModifiedSignal.block();
	this->m_DataHolderInfo2.dataHolder->m_DataModifiedSignal();
	this->m_DataHolderInfo2.connectionToDataModifiedSignal.unblock();
}

void Core::DataHolderConnection::m_DataOfDataHolder2Modified()
{
	this->m_DataHolderInfo1.connectionToDataModifiedSignal.block();
	this->m_DataHolderInfo1.dataHolder->m_DataModifiedSignal();
	this->m_DataHolderInfo1.connectionToDataModifiedSignal.unblock();
}

Core::DataHolderConnection::~DataHolderConnection()
{
	// connections are broken in destructor of DataHolderInfo.
}

Core::DataHolderConnection::DataHolderInfo::~DataHolderInfo()
{
	this->connectionToDataModifiedSignal.disconnect();
	this->connectionToNewDataSignal.disconnect();
	this->connectionToDestructedSignal.disconnect();
}

void Core::ConnectNotifications(Core::DataHolderBase* _dataHolder1, Core::DataHolderBase* _dataHolder2)
{
	new Core::DataHolderConnection(_dataHolder1, _dataHolder2);
}
