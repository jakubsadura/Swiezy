/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREDATAHOLDERCONNECTION_H
#define COREDATAHOLDERCONNECTION_H

#include "gmCommonObjectsWin32Header.h"
#include "coreDataHolder.h"

namespace Core{

/**
\brief This class connects the 'm_DataModified' signals of two data holders.
If either data holder sends the 'm_DataModified' signal, then the signal of 
the other data holder will fire as well.
If either DataHolder obtains a new m_Data, the connection will be broken 
and the DataHolderConnection will destruct itself.

\ingroup gmCommonObjects
\author Maarten Nieber
\date 10 Jan 2008
*/

class GMCOMMONOBJECTS_EXPORT DataHolderConnection : public Core::Object
{
public:
	typedef DataHolderConnection Self;

	coreClassNameMacro(DataHolderConnection)

	/**
	Connects 'm_DataModified' signals of _dataHolder1 and _dataHolder2.
	\param _dataHolder1 - First data holder, may not be NULL.
	\param _dataHolder2 - Second data holder, may not be NULL.

	Note: the DataHolderConnection should not keep the data holders alive. 
	Therefore, a raw pointer is used instead of
	a smart pointer. When the DataHolder destructs, it will notify the 
	DataHolderConnection.
	*/
	DataHolderConnection(
		DataHolderBase* _dataHolder1, 
		DataHolderBase* _dataHolder2);

	//! Breaks the connection between the DataHolders
	~DataHolderConnection();

private:
	//! Called when the m_Data of data holder 1 has modified.
	void m_BreakConnection();

	//! Called when m_Data of first DataHolder was modified
	void m_DataOfDataHolder1Modified();

	//! Called when m_Data of second DataHolder was modified
	void m_DataOfDataHolder2Modified();

private:
	/** Helper structure. Used to store the connections between member 
	functions of this DataHolderConnection and a DataHolder.
	*/
	struct GMCOMMONOBJECTS_EXPORT DataHolderInfo
	{
		//! Disconnects all connections
		~DataHolderInfo();

		//! Data holder with whom this DataHolderConnection is connected.
		Core::DataHolderBase* dataHolder;
		//! Connection between this class and a signal of this->dataHolder.
		boost::signals::connection connectionToDataModifiedSignal;
		//! Connection between this class and a signal of this->dataHolder.
		boost::signals::connection connectionToNewDataSignal;
		//! Connection between this class and a signal of this->dataHolder.
		boost::signals::connection connectionToDestructedSignal;
	};

	/** The signals of m_DataHolderInfo1->dataHolder are connected to the 
	signals of m_DataHolderInfo2->dataHolder.
	*/
	DataHolderInfo m_DataHolderInfo1;
	/** The signals of m_DataHolderInfo1->dataHolder are connected to 
	the signals of m_DataHolderInfo2->dataHolder.
	*/
	DataHolderInfo m_DataHolderInfo2;
};

//! Convenience function: create a new DataHolderConnection instance.
void GMCOMMONOBJECTS_EXPORT ConnectNotifications(
	DataHolderBase* _dataHolder1, 
	DataHolderBase* _dataHolder2);

} // Core


#endif //COREDATAHOLDERCONNECTION_H
