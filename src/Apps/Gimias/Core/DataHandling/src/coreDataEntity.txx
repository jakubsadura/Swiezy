/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
	
#ifndef coreDataEntity_TXX
#define coreDataEntity_TXX

#include "coreDataEntity.h"
#include "coreAssert.h"
#include <boost/bind.hpp>
#include <vector>

/**
Returns the processing object that was set to be held as processing representation.
This function is templated over the object representation type. 
\return true if the cast has been successful. On failure returns false.
*/
template <typename T> 
bool Core::DataEntity::GetProcessingData(
	T& processingDataByRef, 
	TimeStepIndex timeStep,
	bool switchImplementation /*= false*/ ) 
{ 

	bool validTimeStepIndex = timeStep < this->m_DataEntityImpl->GetSize() && timeStep >= 0;
	if ( !validTimeStepIndex )
	{
		return false;
	}

	if ( switchImplementation )
	{
		SwitchImplementation( typeid( processingDataByRef ) );
	}

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( typeid( processingDataByRef ) );
	if ( factory.IsNull() )
	{
		return false;
	}

	boost::any anyProcessingData;
	if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		anyProcessingData = m_DataEntityImpl->GetAt( timeStep )->GetDataPtr( );
	}
	else if ( factory->GetProperties()->FindTagByName( "multiple" ) )
	{
		anyProcessingData = m_DataEntityImpl->GetDataPtr( );
	}
	bool typeIsOk = Core::CastAnyProcessingData( anyProcessingData, processingDataByRef);

	return typeIsOk;
}

template <typename T> 
bool Core::DataEntity::AddTimeSteps( std::vector<T> &processingData)
{
	bool success = true;

	ResetTimeSteps( );

	typename std::vector<T>::iterator it;
	for ( it = processingData.begin() ; it != processingData.end() ; it++ )
	{
		success &= AddTimeStep( *it );
	}

	return success;
}


/**
Connects an observer to the DataEntity by using boost signals. It has to be templated by observer class type
Connects a slot that responds to the OnDestroyed signal of DataEntity, which will be notified (called) when a 
data entity has been uregistered and is about to be destroyed.

\param observer is the class instance of the observer
\param slotFunction is the Slot member of the observer class, following the specified signature 
\return the boost connection, but you might not need it
*/
template <class T> 
boost::signals::connection 
Core::DataEntity::AddObserverOnDestroyDataEntity(T* observer, void (T::*slotFunction)(Core::DataEntity*))
{
	return this->m_OnDestroyedSignal.connect(boost::bind(slotFunction, observer, _1));
}

template< class T >
bool Core::CastAnyProcessingData(const Core::AnyProcessingData& data, T& result)
{
	try
	{
		result = boost::any_cast<T>(data);
	}
	catch(boost::bad_any_cast&)
	{
		return false;
	}
	return true; 
}


#endif // coreDataEntity_TXX
