/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityList.h"
#include "coreAssert.h"
#include "coreReportExceptionMacros.h"
#include "CILabBoostMacros.h"

using namespace Core;

const bool ENABLE_DATAENTITYLIST_DEBUG = false;

/**
*/
DataEntityList::DataEntityList(void)
{
	m_SelectedDataEntityHolder = Core::DataEntityHolder::New();
	m_SelectedDataEntityHolder->SetName( "DataEntityList: Selected Holder" );
}

/**
*/
DataEntityList::~DataEntityList(void)
{
}

/**
*/
bool DataEntityList::Add(DataEntity::Pointer dataEntity)
{
	coreAssertMacro(dataEntity.IsNotNull() && 
		"The requested DataEntity cannot be NULL, if you plan to add it to the DataEntityList");
	DataEntityList::InternalListElement elem;
	DataEntityList::InternalResultType result;
	elem.first = dataEntity->GetId();
	elem.second = Core::DataEntityHolder::New( );
	elem.second->SetSubject( dataEntity );
	elem.second->SetName( "DataEntityList: In/Out holder" );
	result = m_InternalList.insert(elem);
	// Notify Observers
	if ( result.second == true )
	{
		NotifyObserversOnAdd(dataEntity);
	}
	return result.second;
}

/**
*/
void DataEntityList::Remove(DataEntity::Pointer dataEntity)
{
	try
	{
		if ( dataEntity.IsNull() )
		{
			throw Core::Exceptions::Exception( 
				"DataEntityList::Remove", 
				"The requested DataEntity cannot be NULL, for removing" \
				"it from the DataEntityList" );
		}


		if ( ENABLE_DATAENTITYLIST_DEBUG )
		{
			std::cout 
				<< "Remove DataEntity: " << dataEntity->GetId()
				<< " Children: " << dataEntity->GetChildrenList().size() 
				<< std::endl;
		}

		// Remove Children first
		if( !dataEntity->GetChildrenList().empty() )
		{
			// Create a temporal list because it will change during the iteration
			Core::DataEntity::ChildrenListType childrenList;
			childrenList = dataEntity->GetChildrenList();

			Core::DataEntity::ChildrenListType::iterator it;
			it = childrenList.begin();
			while ( it != childrenList.end() )
			{
				int dataEntityID = (*it)->GetId();

				if ( ENABLE_DATAENTITYLIST_DEBUG )
				{
					std::cout << "  Removing children: " << dataEntityID << std::endl;
				}

				// When the data entity is removed from the list
				// reference count could be different of 0 because there is
				// some reference somewhere
				Remove( dataEntityID );

				it++;
			}
		}

		// Get the previous data entity to select it if needed
		Core::DataEntity::Pointer previousDataEntity;
		if ( dataEntity == GetSelectedDataEntity( ) )
		{
			previousDataEntity = GetPreviousDataEntity( dataEntity );
		}

		// Set the holder to NULL -> Notify connected holders
		if ( GetDataEntity( dataEntity->GetId() ).IsNotNull() )
		{
			GetDataEntityHolder( dataEntity->GetId() )->SetSubject( NULL );
		}

		// Unselect the current data entity
		if ( dataEntity == GetSelectedDataEntity( ) )
		{
			m_SelectedDataEntityHolder->SetSubject( NULL );
		}

		// Remove data entity from the list
		int numElementErased = m_InternalList.erase( dataEntity->GetId() );

		// Notify Observers
		if ( numElementErased > 0 )
		{
			NotifyObserversOnRemove(dataEntity);
		}

		// Here everything should be clean
		// Select the new data entity if it was selected
		if ( previousDataEntity.IsNotNull() )
		{
			m_SelectedDataEntityHolder->SetSubject( previousDataEntity );
		}

	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityList::Remove)
}

/**
*/
void DataEntityList::Remove(unsigned int dataEntityId)
{
	try
	{
		DataEntity::Pointer dataEntity = GetDataEntity(dataEntityId);
		if(dataEntity.IsNotNull())
			Remove(dataEntity);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityList::Remove)
}

/**
*/
void DataEntityList::RemoveAll()
{
	try
	{
		while ( GetCount( ) )
		{
			Remove( GetId( m_InternalList.begin( ) ) );
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityList::RemoveAll)
}

/**
*/
DataEntity::Pointer DataEntityList::GetDataEntity(unsigned int dataEntityId)
{
	return GetDataEntityHolder( dataEntityId )->GetSubject( );
}


/**
*/
DataEntityHolder::Pointer Core::DataEntityList::GetDataEntityHolder( unsigned int dataEntityId )
{
	InternalListType::const_iterator it = m_InternalList.find(dataEntityId);
	if( it == m_InternalList.end() )
	{
		Core::Exceptions::DataEntityIdNotFound e("DataEntityList::GetDataEntityHolder");
		cilabBoostFormatMacro(" Unknown id %d", dataEntityId, message);
		e.Append(message.c_str());
		throw e;
	}
	return it->second;
}

void DataEntityList::NotifyObserversOnAdd(DataEntity::Pointer dataEntity) const
{
	// Calling the function call operator may invoke undefined behavior if no slots are connected
	// to the signal, depending on the combiner used, so check it was connected to a slot beforehand
	if(!m_OnAddSignal.empty())
		m_OnAddSignal(dataEntity);
}

//!
void DataEntityList::NotifyObserversOnRemove(DataEntity::Pointer dataEntity) const
{
	// Calling the function call operator may invoke undefined behavior if no slots are connected
	// to the signal, depending on the combiner used, so check it was connected to a slot beforehand
	if(!m_OnRemoveSignal.empty())
		m_OnRemoveSignal(dataEntity);
}


/**
*/
DataEntityList::iterator DataEntityList::Begin(void)
{ 
	return m_InternalList.begin(); 
}

/**
*/
DataEntityList::iterator DataEntityList::End(void) 
{ 
	return m_InternalList.end(); 
}

/**
*/
void DataEntityList::Next(Self::iterator& it) 
{
	++it; 
}

/**
*/
unsigned int DataEntityList::GetId(Self::iterator it) 
{
	return (*it).first; 
}

/**
*/
DataEntity::Pointer DataEntityList::Get(Self::iterator it)
{
	return (*it).second->GetSubject( ); 
}


int Core::DataEntityList::GetCount() const
{
	return m_InternalList.size();
}

bool Core::DataEntityList::IsInTheList( unsigned int dataEntityId )
{
	bool foundInInternalList;
	
	InternalListType::iterator it = m_InternalList.find( dataEntityId );
	foundInInternalList = it != m_InternalList.end();

	// Check that the holder is correctly configured
	if ( foundInInternalList )
	{
		Core::DataEntityHolder::Pointer  holder;
		holder = GetDataEntityHolder( dataEntityId );
		foundInInternalList = holder.IsNotNull() && holder->GetSubject().IsNotNull( ) ;
	}

	return foundInInternalList;
}


int  Core::DataEntityList::GetPositionInList(unsigned int dataEntityId)
{
	if(  m_InternalList.find( dataEntityId ) == m_InternalList.end() )
		return -1;
	else
	{
		Self::iterator it = m_InternalList.begin();
		//! the position to be returned. As Items are pushed from top,
		// the first item is the one with the highest id
		int position = m_InternalList.size()-1;

		while ( it != m_InternalList.end() )
		{	
			if( GetId(it) == dataEntityId )
				return position;
			else
			{
				it++;
				position--;
			}
		}
	}

	return -1;
}

Core::DataEntityList::iterator Core::DataEntityList::Find( 
	unsigned int dataEntityId )
{
	return m_InternalList.find( dataEntityId );
}

DataEntity::Pointer Core::DataEntityList::GetPreviousDataEntity( 
	DataEntity::Pointer dataEntity )
{
	if ( GetCount( ) > 1 )
	{
		Core::DataEntityList::iterator it;
		Core::DataEntityList::iterator begin = Begin( );
		it = Find( dataEntity->GetId( ) );

		// Select the previous one
		if ( it != begin )
		{
			it--;
			return Get( it );
		}
		else 
		{
			it++;
			return Get( it );
		}
	}

	return NULL;
}

Core::DataEntityHolder::Pointer 
Core::DataEntityList::GetSelectedDataEntityHolder() const
{
	return m_SelectedDataEntityHolder;
}

Core::DataEntity::Pointer Core::DataEntityList::GetSelectedDataEntity() const
{
	return m_SelectedDataEntityHolder->GetSubject();
}

void Core::DataEntityList::RemoveSelectedDataEntity()
{
	Remove( GetSelectedDataEntity() );
}

Core::DataEntity::Pointer Core::DataEntityList::FindChild( 
	Core::DataEntity::Pointer fatherDataEntity, 
	Core::DataEntityType type,
	Core::DataEntityMetadata::Pointer metadata )
{
	Core::DataEntity::Pointer childDataEntity = NULL;
	if(fatherDataEntity.IsNull())
	{
		return NULL;
	}

	Core::DataEntity::ChildrenListType list = fatherDataEntity->GetChildrenList();
	Core::DataEntity::ChildrenListType::iterator it = list.begin();
	while ( it != list.end() && childDataEntity.IsNull() )
	{
		Core::DataEntity::Pointer child = (*it);
		bool checkType = child->GetType() & type;
		bool checkIsInList = IsInTheList( child->GetId( ) );
		bool checkMetadata = true;
		if ( metadata.IsNotNull( ) )
		{
			Core::DataEntityMetadata::Pointer childMetadata;
			childMetadata = child->GetMetadata();
			blTagMap::Iterator it;
			for ( it = metadata->GetIteratorBegin( ); 
				  it != metadata->GetIteratorEnd( ); 
				  it++ )
			{
				blTag::Pointer childTag = childMetadata->GetTag( it->first );
				checkMetadata &= childTag->GetValueAsString() == it->second->GetValueAsString();
			}
		}
		if ( checkType && checkIsInList && checkMetadata )
		{
			childDataEntity = (*it);
		}
		it++;
	}

	return childDataEntity;
}

void Core::DataEntityList::ConnectInputHolder( 
	Core::DataEntityHolder::Pointer processorHolder )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = processorHolder->GetSubject( );
	if ( dataEntity.IsNull() )
	{
		return;
	}

	Core::DataEntityHolder::Pointer listHolder;
	listHolder = GetDataEntityHolder( dataEntity->GetId( ) );

	DataEntityHolderConnection::Connect( 
		listHolder, 
		processorHolder,
		CONNECTION_DATALIST_AND_PROCESSOR_IN );
}

void Core::DataEntityList::ConnectOutputHolder( 
	Core::DataEntityHolder::Pointer processorHolder )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = processorHolder->GetSubject( );
	if ( dataEntity.IsNull() )
	{
		return;
	}

	Core::DataEntityHolder::Pointer listHolder;
	listHolder = GetDataEntityHolder( dataEntity->GetId( ) );

	DataEntityHolderConnection::Connect( 
		listHolder, 
		processorHolder,
		CONNECTION_DATALIST_AND_PROCESSOR_OUT );
}
