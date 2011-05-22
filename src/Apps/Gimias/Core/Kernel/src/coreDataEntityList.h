/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityList_H
#define coreDataEntityList_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreDataEntityHolder.h"
#include "coreException.h"
#include "CILabExceptionMacros.h"
#include "coreCreateExceptionMacros.h"
#include <boost/signals.hpp>
#include <map>

class CoreDataEntityListTest;

/** 
\brief This exception is thrown when you try to use a data entity id that 
does not exist in the data entity list.
\ingroup gmKernel

\author Maarten Nieber
\date 23 may 2008
*/
namespace Core 
{ 
namespace Exceptions 
{ 
class DataEntityIdNotFound : public RuntimeException	
{ 
public: coreClassNameMacro(DataEntityIdNotFound); 
		DataEntityIdNotFound (const char* callingFunction) 
			: RuntimeException(callingFunction, "Data entity id not found") 
		{ this->m_className = GetNameOfClass(); }; 
};	
}
}

namespace Core
{
/**
\brief The DataEntityList holds all the DataEntities available for 
processing, so the user can select one of them
so as to perform tasks, rendering it or browsing them.

This is the central piece for holding the list of available inputs 
and outputs of computational algorithms, and
is is also the central piece for inter-plugin communication of input data.
Some classes will be observing it on add and remove, like the 
DataEntityListBrowser, that displays the contents
of the DataEntityList, and keeps in sync with it.

\sa Core::DataEntity, Core::Widgets::DataEntityListBrowser

\ingroup gmKernel
\author Juan Antonio Moya
\date 25 Jan 2008
*/

class GMKERNEL_EXPORT DataEntityList : public Core::SmartPointerObject
{

protected:
	friend class ::CoreDataEntityListTest;

	//! Types for handling the internal list
	typedef std::map<unsigned int, DataEntityHolder::Pointer> InternalListType;
	//! Types for handling the internal list
	typedef std::pair<unsigned int, DataEntityHolder::Pointer> InternalListElement;
	//! Types for handling the internal list
	typedef std::pair<InternalListType::iterator, bool> InternalResultType;

public:
	coreDeclareSmartPointerClassMacro(DataEntityList, Core::SmartPointerObject);

	//! Types for handling the internal list
	typedef InternalListType::iterator iterator;

	// Types for signals and slots
	typedef boost::signal1<void, Core::DataEntity::Pointer> DataEntityAddedSignal;
	// Types for signals and slots
	typedef DataEntityAddedSignal::slot_function_type DataEntityAddedSlot;
	// Types for signals and slots
	typedef boost::signal1<void, Core::DataEntity::Pointer> DataEntityRemovedSignal;
	// Types for signals and slots
	typedef DataEntityRemovedSignal::slot_function_type DataEntityRemovedSlot;


public:

	//! Returns number of data entities
	int GetCount() const;

	//!
	Core::DataEntityHolder::Pointer GetSelectedDataEntityHolder() const;

	//!
	Core::DataEntity::Pointer GetSelectedDataEntity() const;

	/**
	* Add a data entity to the list
	\return If it already exists in the list, the function returns false. 
	Otherwise, it returns true.
	*/
	bool Add(DataEntity::Pointer dataEntity);
	
	/** Removes a data entity for the list all all the children
	If it was removed successfully, calls a procedure that broadcasts to all
	the plugins and other class interested, that a data entity has been removed 
	from the list.
	*/
	void Remove(DataEntity::Pointer dataEntity);

	//!
	void Remove(unsigned int dataEntityId);

	//!
	void RemoveAll();

	//!
	void RemoveSelectedDataEntity( );

	//! Get the dataEntity which has dataEntityId
	DataEntity::Pointer GetDataEntity(unsigned int dataEntityId);

	//! Returns true if dataEntityId belongs to the list
	bool IsInTheList(unsigned int dataEntityId);

	//! Return the position of the data entity in the list. 
	int GetPositionInList(unsigned int dataEntityId);
	
	/** Return the previous data entity or the next if is the first or 	
	NULL if its the only one
	*/
	DataEntity::Pointer GetPreviousDataEntity( DataEntity::Pointer data );

	//! Get the holder for this data entity
	DataEntityHolder::Pointer GetDataEntityHolder(unsigned int dataEntityId);

	/**
	Returns an iterator to the begining of the internal list of data 
	entities, in a stl approach
	*/
	Self::iterator Begin(void);
	/**
	Returns an iterator to the end of the internal list of data entities, 
	in a stl approach
	*/
	Self::iterator End(void);

	/**
	Returns an iterator to the next element of the internal list of data 
	entities, in a stl approach
	Its the same than pre-incrementing the iterator
	*/
	void Next(Self::iterator& it);

	/**
	Returns the data entity id of the DataEntity held by list element, 
	that the iterator is pointing to
	*/
	unsigned int GetId(Self::iterator it);

	/**
	Returns the DataEntity held by list element, that the iterator is 
	pointing to
	*/
	DataEntity::Pointer Get(Self::iterator it);

	//! Find iterator to Data entity
	Self::iterator Find(unsigned int dataEntityId);

	/** Finds a child of a data entity. Return the first match of the 
	child data entity, NULL otherwise. The child should be in the data list
	*/
	Core::DataEntity::Pointer FindChild(
		DataEntity::Pointer fatherDataEntity,
		Core::DataEntityType type,
		Core::DataEntityMetadata::Pointer metadata = NULL );

	/** Templated subject-observer methods (using boost signals). 
	Include coreDataEntityList.txx
	*/
	template <class T> boost::signals::connection AddObserverOnAddDataEntity(
		T* observer, 
		void (T::*slotFunction)(Core::DataEntity::Pointer));
	template <class T> boost::signals::connection AddObserverOnRemoveDataEntity(
		T* observer, 
		void (T::*slotFunction)(Core::DataEntity::Pointer));

	//!
	void ConnectInputHolder( Core::DataEntityHolder::Pointer processorHolder );

	//!
	void ConnectOutputHolder( Core::DataEntityHolder::Pointer processorHolder );

protected:
	//!
	virtual void NotifyObserversOnAdd(DataEntity::Pointer dataEntity) const;
	//!
	virtual void NotifyObserversOnRemove(DataEntity::Pointer dataEntity) const;

	//!
	DataEntityList(void);

	virtual ~DataEntityList(void);

private:
	//!
	InternalListType m_InternalList;

	//!
	DataEntityAddedSignal m_OnAddSignal;

	//!
	DataEntityRemovedSignal m_OnRemoveSignal;

	//!
	Core::DataEntityHolder::Pointer m_SelectedDataEntityHolder;

	coreDeclareNoCopyConstructors(DataEntityList);
};

} // namespace Core

#endif

