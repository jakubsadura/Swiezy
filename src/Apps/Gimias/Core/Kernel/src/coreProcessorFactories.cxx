/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreProcessorFactories.h"

Core::ProcessorFactories::ProcessorFactories()
{
	m_CreatorsMapHolder = Core::ProcessorFactories::FactoriesMapHolderType::New();
}

Core::BaseProcessor::Pointer
Core::ProcessorFactories::Create( const std::string &classname )
{
	FactoriesMapType::iterator it = GetFactoriesMap().begin();
	bool found = false;
	while (!found && it != GetFactoriesMap().end( ) )
	{
		std::string processorClassName;
		processorClassName = it->second->GetProcessorClassname();
		if ( classname.find( processorClassName ) != std::string::npos )
		{
			found = true;
			return it->second->Create();
		}
		it++;
	}

	return NULL;
}

void Core::ProcessorFactories::CleanRegisteredFactories()
{
	GetFactoriesMap().clear();
}

Core::ProcessorFactories::FactoriesMapHolderType::Pointer 
Core::ProcessorFactories::GetFactoriesHolder()
{
	return m_CreatorsMapHolder;
}

Core::ProcessorFactories::FactoriesMapType & 
Core::ProcessorFactories::GetFactoriesMap()
{
	return m_CreatorsMapHolder->GetSubject();
}

void Core::ProcessorFactories::RegisterFactory( 
	ProcessorFactory::Pointer factory )
{
	GetFactoriesMap()[ factory->GetProcessorClassname() ] = factory;
	GetFactoriesHolder( )->NotifyObservers( );
}

