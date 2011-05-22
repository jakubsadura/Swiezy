/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityList_TXX
#define coreDataEntityList_TXX

#include "coreDataEntityList.h"
#include "coreDataEntity.h"
#include "coreReportExceptionMacros.h"
#include <boost/bind.hpp>

/**
Connects an observer to the DataEntityList by using boost signals. It has 
to be templated by observer class type Connects a slot thet responds to 
the OnAdded signal of DataEntityList, which will be notified (called) when a 
data entity has been added to the list.

\param observer is the class instance of the observer
\param slotFunction is the Slot member of the observer class, following 
the specified signature 
\return the boost connection, but you might not need it
*/
template <class T>
boost::signals::connection Core::DataEntityList::AddObserverOnAddDataEntity(
	T* observer, void (T::*slotFunction)(Core::DataEntity::Pointer))
{
	try
	{
		DataEntityAddedSlot _slot = boost::bind(slotFunction, observer, _1);
		return this->m_OnAddSignal.connect(_slot);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityList::AddObserverOnAddDataEntity)
}

/**
Connects an observer to the DataEntityList by using boost signals. It has 
to be templated by observer class type Connects a slot thet responds to 
the OnRemoved signal of DataEntityList, which will be notified (called) when a 
data entity has been removed from the list.

\param observer is the class instance of the observer
\param slotFunction is the Slot member of the observer class, following 
the specified signature 
\return the boost connection, but you might not need it
*/
template <class T>
boost::signals::connection Core::DataEntityList::AddObserverOnRemoveDataEntity(
	T* observer, void (T::*slotFunction)(Core::DataEntity::Pointer))
{
	try
	{	
		DataEntityRemovedSlot _slot = boost::bind(slotFunction, observer, _1);
		return this->m_OnRemoveSignal.connect(_slot);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityList::AddObserverOnAddDataEntity)
}

#endif



