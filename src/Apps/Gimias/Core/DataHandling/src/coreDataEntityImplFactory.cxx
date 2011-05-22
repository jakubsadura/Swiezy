/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityImplFactory.h"

using namespace Core;

DataEntityImplFactory::DataEntityImplFactoryListType DataEntityImplFactory::m_FactoryList;

/**
*/
Core::DataEntityImplFactory::DataEntityImplFactory( ) 
{
	m_Properties = blTagMap::New( );
}

DataEntityImplFactory::Pointer 
Core::DataEntityImplFactory::FindFactory( const std::type_info &type )
{
	blTagMap::Pointer tagMap = blTagMap::New( );
	tagMap->AddTag( type.name(), true );
	DataEntityImplFactory::Pointer factory = FindFactory( tagMap );
	return factory;
}

DataEntityImplFactory::Pointer 
Core::DataEntityImplFactory::FindFactory( blTagMap::Pointer tagMap )
{
	bool found = false;
	DataEntityImplFactoryListType::iterator it = m_FactoryList.begin( );
	while( it != m_FactoryList.end() )
	{
		if ( (*it)->CheckProperties( tagMap ) )
		{
			found = true;
			break;
		}
		it++;
	}

	if ( !found )
	{
		return NULL;
	}

	return (*it);
}

DataEntityImplFactory::Pointer 
Core::DataEntityImplFactory::FindFactory( const std::string &name )
{
	bool found = false;
	DataEntityImplFactoryListType::iterator it = m_FactoryList.begin( );
	while( it != m_FactoryList.end() )
	{
		std::string datatypename = (*it)->GetDataType( ).name( );
		if ( datatypename.find( name ) != std::string::npos )
		{
			found = true;
			break;
		}
		it++;
	}

	if ( !found )
	{
		return NULL;
	}

	return (*it);
}

void Core::DataEntityImplFactory::RegisterFactory( DataEntityImplFactory::Pointer factory )
{
	m_FactoryList.push_back( factory );
}

void Core::DataEntityImplFactory::UnRegisterAll()
{
	m_FactoryList.clear( );
}

bool Core::DataEntityImplFactory::CheckProperties( blTagMap::Pointer tagMap )
{
	bool success = false;

	blTagMap::Iterator it;
	for ( it = tagMap->GetIteratorBegin() ; it != tagMap->GetIteratorEnd() ; it++ )
	{
		blTag::Pointer tag = m_Properties->FindTagByName( it->second->GetName( ) );
		if ( tag.IsNotNull( ) )
		{
			success |= tag->Compare( it->second );
		}
		else
		{
			success = false;
		}
	}

	return success;
}

blTagMap::Pointer Core::DataEntityImplFactory::GetProperties() const
{
	return m_Properties;
}
