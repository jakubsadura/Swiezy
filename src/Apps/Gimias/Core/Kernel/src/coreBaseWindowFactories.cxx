/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseWindowFactories.h"

Core::BaseWindowFactories::BaseWindowFactories()
{
	m_CreatorsMapHolder = Core::BaseWindowFactories::FactoriesMapHolderType::New();
}

Core::BaseWindow* 
Core::BaseWindowFactories::CreateWindow( 
	const std::string &factoryName,
	wxWindow* parent,
	const wxPoint&  pos /*= wxDefaultPosition*/,
	const wxSize&  size /*= wxDefaultSize*/ )
{
	BaseWindowFactory::Pointer factory = FindFactory( factoryName );
	if ( factory.IsNull() )
	{
		std::ostringstream strError;
		strError << factoryName << " is not registered" << std::endl;
		throw Core::Exceptions::Exception( 
			"BaseWindowFactories::CreateWindow",
			strError.str().c_str() );
	}

	Core::WindowConfig config = GetFactoriesMap()[ factoryName ].m_WindowConfig;
	
	factory->SetPosition( pos );
	factory->SetSize( size );
	factory->SetParent( parent );
	factory->SetWindowId( config.GetId( ) );
	factory->SetWindowName( config.GetCaption() );
	factory->SetBitmapFilename( config.GetBitmapFilename() );

	return factory->CreateWindow();
}

void Core::BaseWindowFactories::CleanRegisteredWidgets()
{
	GetFactoriesMap().clear();
}

bool Core::BaseWindowFactories::GetWindowConfig( 
	const std::string &factoryName,
	Core::WindowConfig &config )
{
	FactoriesMapType::iterator it = GetFactoriesMap().find( factoryName );
	if(it != GetFactoriesMap().end( ) )
	{
		config = it->second.m_WindowConfig;
		return true;
	}
	return false; 
}

std::list<std::string> Core::BaseWindowFactories::GetWindowsList(
	WIDGET_TYPE type /*= WIDGET_TYPE_MAX*/,
	const std::string &caption /*= ""*/,
	const std::string &category /*= ""*/,
	int id /*=-1*/,
	WIDGET_STATE state /*= WIDGET_STATE_MAX*/)
{
	std::list<std::string> windowsList;

	FactoriesListType::iterator it;
	for ( it = m_CreatorsList.begin( ) ; it != m_CreatorsList.end() ; it++ )
	{
		WindowConfig config;
		GetWindowConfig( *it, config );
		bool hasFlag = config.HasFlag( state ) || state == WIDGET_STATE_MAX;
		bool hasType = config.HasType( type ) || type == WIDGET_TYPE_MAX;
		bool matchesCategory = category == config.GetCategory() || category.empty();
		bool matchesCaption = caption == config.GetCaption() || caption.empty();
		bool matchesID = id == -1 || id == config.GetId();

		if ( hasFlag && hasType && matchesCategory && matchesCaption && matchesID )
		{
			windowsList.push_back( *it );
		}
	}

	return windowsList;
}

Core::BaseWindowFactories::FactoriesMapHolderType::Pointer 
Core::BaseWindowFactories::GetFactoriesHolder()
{
	return m_CreatorsMapHolder;
}

Core::BaseWindowFactories::FactoriesMapType & 
Core::BaseWindowFactories::GetFactoriesMap()
{
	return m_CreatorsMapHolder->GetSubject();
}

void Core::BaseWindowFactories::RegisterFactory( 
	BaseWindowFactory::Pointer factory,
	WindowConfig widgetConfig )
{

	std::string name = factory->GetName();
	if ( FindFactory( name ) )
	{
		std::ostringstream strError;
		strError << "A factory with the class name: " << name
			<< " is already registered." << std::endl;
		throw Core::Exceptions::Exception( 
			"BaseWindowFactories::RegisterFactory",
			strError.str().c_str() );
	}

	FactoryData data;
	data.m_Factory = factory;
	data.m_WindowConfig = widgetConfig;

	m_CreatorsList.push_back( name );
	GetFactoriesMap()[ name ] = data;
	GetFactoriesHolder( )->NotifyObservers( );

}

Core::BaseWindowFactory::Pointer 
Core::BaseWindowFactories::FindFactory( const std::string &factoryName )
{
	FactoriesMapType::iterator it = GetFactoriesMap().find( factoryName );
	if ( it == GetFactoriesMap().end() )
	{
		return NULL;
	}

	return it->second.m_Factory;
}

