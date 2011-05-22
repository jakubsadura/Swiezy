/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenderingTree.h"
#include "coreAssert.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityInfoHelper.h"
#include "coreEnvironment.h"
#include "coreLogger.h"
#include "coreKernel.h"

#include <sstream>

using namespace Core;

//!
RenderingTree::RenderingTree(void)
{
}

//!
RenderingTree::~RenderingTree(void)
{
	// Clear all connection to OnDestroyed signal of the observed DataEntities
	for(OnDestroyDataEntityConnections::iterator it = this->m_OnDestroyDataEntityConnections.begin();
		it != this->m_OnDestroyDataEntityConnections.end();
		++it)
	{
		(*it).second.m_Connection.disconnect();
	}
	this->m_OnDestroyDataEntityConnections.clear();
}

/**
Removes all the rendering nodes mapped to the given data entity. If there were removed successfully, it returns
true and also casts a signal so observers could notice the event.  This function will kill the implied rendering 
nodes and all its subtrees.

\param DataEntity to be removed
\sa DataEntity
*/
bool RenderingTree::Remove(DataEntity* dataEntity, bool bInitializeViews /*= true*/)
{
	if ( dataEntity == NULL )
	{
		throw Core::Exceptions::Exception( "RenderingTree::Remove", 
			"The requested DataEntity cannot be NULL, so as to removing it from the RenderingTree" );
	}

	bool removedAny = false;
	try
	{
		removedAny = this->Remove(dataEntity->GetId(), bInitializeViews);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTree::Remove)
	return removedAny;
}

//!
void RenderingTree::OnDestroyDataEntity(Core::DataEntity* dataEntity)
{
	try
	{
		this->Remove(dataEntity);
	}
	coreCatchExceptionsReportAndNoThrowMacro(RenderingTree::OnDestroyDataEntity)
}

//!
void RenderingTree::ConnectToOnDestroySignal(Core::DataEntity::Pointer dataEntity)
{
	coreAssertMacro(dataEntity.IsNotNull() && "You may give a valid subject so as to observe it");
	unsigned int id = dataEntity->GetId();
	if(this->m_OnDestroyDataEntityConnections[id].refCount == 0)
		this->m_OnDestroyDataEntityConnections[id].m_Connection = 
		dataEntity->AddObserverOnDestroyDataEntity<RenderingTree>(this, &RenderingTree::OnDestroyDataEntity);
	coreAssertMacro(this->m_OnDestroyDataEntityConnections[id].m_Connection.connected() && 
		"the connection must be alive now");
	this->m_OnDestroyDataEntityConnections[id].refCount++;
}



//!
void RenderingTree::DisconnectOnDestroyedSignal(unsigned int dataEntityId)
{
	if(this->m_OnDestroyDataEntityConnections[dataEntityId].refCount > 0)
		this->m_OnDestroyDataEntityConnections[dataEntityId].refCount--;
	if(this->m_OnDestroyDataEntityConnections[dataEntityId].refCount == 0)
	{
		this->m_OnDestroyDataEntityConnections[dataEntityId].m_Connection.disconnect();
		coreAssertMacro(!this->m_OnDestroyDataEntityConnections[dataEntityId].m_Connection.connected() 
			&& "the connection must not be kept alive any more");
		this->m_OnDestroyDataEntityConnections.erase(dataEntityId);
	}
}

void Core::RenderingTree::ConnectToOnModifiedSignal( Core::DataEntity::Pointer dataEntity )
{
	dataEntity->AddObserver1<RenderingTree>( this, &RenderingTree::OnModifiedDataEntity );
}

void Core::RenderingTree::DisconnectOnModifiedSignal( Core::DataEntity* dataEntity )
{
	dataEntity->RemoveObserver1<RenderingTree>( this, &RenderingTree::OnModifiedDataEntity );
}

void Core::RenderingTree::ConnectToOnModifiedAnyNodeSignal( Core::DataEntity::Pointer dataEntity )
{
	
}

void Core::RenderingTree::DisconnectToOnModifiedAnyNodeSignal( Core::DataEntity::Pointer dataEntity )
{
	
}

void Core::RenderingTree::UpdateMetadata( Core::DataEntity::Pointer dataEntity )
{

}

void Core::RenderingTree::UpdateRenderingProperties( Core::DataEntity::Pointer dataEntity )
{

}

std::string Core::RenderingTree::GetName() const
{
	return m_Name;
}

void Core::RenderingTree::SetName( std::string val )
{
	m_Name = val;
}
