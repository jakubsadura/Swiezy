/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreFrontEndPluginManager.h"
#include "coreKernel.h"
#include "coreLogger.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreSettings.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreDirectory.h"
#include "coreFile.h"
#include "coreAssert.h"

#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

using namespace Core::Runtime;
using namespace Core::FrontEndPlugin;

bool CheckLibraryHandleIsNULL( 
	Core::Runtime::FrontEndPluginLoader::Pointer loader )
{
	return loader->GetLibraryHandle( ) == NULL;
}

/** Constructor for the class FrontEndPluginManager.
When the FrontEndPluginManager is initialized, it preloads all the plugin 
shared libraries and scans these libraries for any user profile descriptor. 
It sets all the found profile descriptors to the internal list 
of Available profiles of this plugin manager, that can be retrieved 
later by a call to GetAvailableProfiles().
\sa InferAvailableProfilesFromPlugins, GetAvailableProfiles
*/
FrontEndPluginManager::FrontEndPluginManager(void)
{
	m_ModuleFactory = new ModuleFactory( );
	m_AvailableProfilesHolder = DataHolder<Core::Profile::Pointer>::New( );
	m_CLPProfilesHolder = DataHolder<Core::Profile::Pointer>::New( );
	ScanPluginsFolders( );
}

/** Destructor for the class FrontEndPluginManager. */
FrontEndPluginManager::~FrontEndPluginManager(void)
{
	UnLoadAllPlugins(true);
	if ( m_ModuleFactory )
	{
		delete m_ModuleFactory;
		m_ModuleFactory = NULL;
	}
}

/**
It preloads the libraries found under "plugins" directory of the current 
working dir (the application's dir) Keeps a handle to those shared 
libraries and maintains them opened to handle future queries for functions.
This step is the previous one for either inferring the profiles from 
the plugins or to actually load the plugins
into the GUI, and MUST be called first.
*/
void FrontEndPluginManager::PreLoadPluginLibraries(void)
{
	try
	{
		// Get the required high level managers
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		coreAssertMacro(settings.IsNotNull() && "The settings manager is not initialized");

		std::vector<std::string> pluginsFolders = settings->GetPluginsFolders();
		std::vector<std::string>::iterator it;
		for ( it = pluginsFolders.begin() ; it != pluginsFolders.end() ; it++ )
		{
			blTextUtils::StrSub( *it, "$(GimiasPath)", settings->GetApplicationPath() );
		}

		for ( it = pluginsFolders.begin() ; it != pluginsFolders.end( ) ; it++ )
		{
			ScanPluginFolder( *it );
		}

		OrderPluginsDependencies( );

		// Load library handle
		PluginLoaderList::iterator itLib;
		for (itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
		{
			(*itLib)->LoadLibrary( );
		} // for each Plugin dir

		m_LoadedPlugins.remove_if( CheckLibraryHandleIsNULL );

	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::PreLoadPluginLibraries);
}

/**
When called it scans the plugin libraries so as to retrieve the Profile 
descriptors, and returns them.
\return The list of available profiles scanned for all the plugins
\sa GetAvailableProfiles
*/
Core::Profile::Pointer FrontEndPluginManager::InferAvailableProfilesFromPlugins(void)
{
	Core::Profile::Pointer profileFromPlugin;
	Core::Profile::Pointer scannedProfiles;

	try
	{
		// Clear the collected profile from the plugins and set it to the default
		scannedProfiles = Core::Profile::New();

		PluginLoaderList::iterator itLib;
		for (itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
		{
			try
			{  
				profileFromPlugin = (*itLib)->GetProfileDescriptor( );

				if(profileFromPlugin.IsNotNull())
					scannedProfiles->MergeProfiles(profileFromPlugin);
			}
			catch(Core::Exceptions::Exception& e)
			{
				Core::Runtime::Kernel::GetGraphicalInterface()->LogException(e);
				Core::Runtime::Kernel::GetLogManager()->LogException(e);
			}
		} // for each shared library


	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::InferAvailableProfilesFromPlugins);
	return scannedProfiles;
}

/**
Load the plugins from the available plugin libraries retrieved from a 
previous call to PreLoadPluginLibraries.
It attaches to the GUI all the plugins that match the given profile 
as parameter, and unloads any other plugin
not matching the profile.
\sa PreLoadPluginLibraries
*/
void FrontEndPluginManager::CreatePluginsMatchingProfile(
	Core::Profile::Pointer userProfile)
{
	try
	{
		UnLoadAllPlugins();

		// Add dependencies
		AddPluginsDependencies( userProfile );

		// for each loaded library
		PluginLoaderList::iterator itLib;
		for ( itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
		{
			try
			{  
				// Create the FrontEndPlugin object from the library
				Core::Profile::Pointer profileFromPlugin;
				profileFromPlugin = (*itLib)->GetProfileDescriptor( );

				// Grab the plugin's profile and check if matches with the user's profile
				bool matchesProfile;
				matchesProfile = userProfile->ProfileSharesSomeOfType(profileFromPlugin);

				if(matchesProfile )
				{
					 
					std::string message = "Loading plugin " + (*itLib)->GetName( );
					Core::Runtime::Kernel::GetGraphicalInterface()->LogMessage(message);
					std::cout << message << std::endl;

					m_CurrentPluginName = (*itLib)->GetName();

					(*itLib)->CreateBaseFrontEndPlugin( );

					LoadXMLDescriptions( (*itLib)->GetLibraryPath() + "/Filters/" );
				}  
			}
			catch(Core::Exceptions::Exception& e)
			{
				e.AddTrace("FrontEndPluginManager::LoadPlugins");
				Core::Runtime::Kernel::GetGraphicalInterface()->LogException(e);
				Core::Runtime::Kernel::GetLogManager()->LogException(e);
			}
			catch(...)
			{
				// If the symbol was not found, display error
				Core::Exceptions::CannotReadProfileException eBadProf("FrontEndPluginManager::LoadPlugins");
				Core::Runtime::Kernel::GetGraphicalInterface()->LogException(eBadProf);
				Core::Runtime::Kernel::GetLogManager()->LogException(eBadProf);
			}

		} // for each shared library

		LoadCommandLinePlugins( userProfile );

		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		LoadXMLDescriptions( settings->GetResourcePath() + "/Descriptions/" );

		m_CurrentPluginName = "";
	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::LoadPlugins);
}

void FrontEndPluginManager::AddPluginsDependencies( 
	Core::Profile::Pointer userProfile )
{
	try
	{

		PluginLoaderList::iterator itLib;
		for ( itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
		{
			// Create the FrontEndPlugin object from the library
			Core::Profile::Pointer profileFromPlugin;
			profileFromPlugin = (*itLib)->GetProfileDescriptor( );

			// Grab the plugin's profile and check if matches with the user's profile
			bool matchesProfile;
			matchesProfile = userProfile->ProfileSharesSomeOfType(profileFromPlugin);

			if(matchesProfile )
			{
				std::list<std::string> dependencies = (*itLib)->GetDependencies();
				std::list<std::string>::iterator itDepen;
				for ( itDepen = dependencies.begin();itDepen != dependencies.end() ; ++itDepen)				
				{
					Core::Runtime::FrontEndPluginLoader::Pointer loader;
					loader = GetPluginLoader( *itDepen );
					if ( loader.IsNotNull( ) )
					{
						userProfile->MergeProfiles( loader->GetProfileDescriptor( )  );
					}
				}
			}
		}

		// Add always MITKPlugin for backwards compatibility
		userProfile->AddToProfile( "MITKPlugin" );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::AddPluginsDependencies);
}

/**
Unloads all the front end plugins from the GUI of the application, by 
removing them from the main window
and calling UnRegister so as to release all its resources. 

\param When the parameter alsoReleaseResources is set to
true, it also closes all open library handles. So if you need to load 
plugins later, you have to make a call 
to PreLoadPluginLibraries() in order to open again the library handles.

\sa PreLoadPluginLibraries
*/
void FrontEndPluginManager::UnLoadAllPlugins(bool alsoReleaseResources)
{
	// Unload all plugins
	PluginLoaderList::iterator it;
	for ( it = m_LoadedPlugins.begin() ; it != m_LoadedPlugins.end() ; it++ )
	{
		try
		{
			// Force kill
			Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface; 
			graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
			if ( (*it)->GetBaseFrontEndPlugin( ).IsNotNull( ) )
			{
				(*it)->ResetBaseFrontEndPlugin( );
			}
		}
		catch(...) { ; }
	}

	if(alsoReleaseResources)
	{
		while ( m_LoadedPlugins.size() > 0 )
		{
			m_LoadedPlugins.remove( m_LoadedPlugins.back() );
		}
	}
}

Core::Profile::Pointer Core::Runtime::FrontEndPluginManager::GetAvailableProfiles() const
{
	return m_AvailableProfilesHolder->GetSubject( );
}

/**
Dicom plugin will be put as the first plugin
*/
void Core::Runtime::FrontEndPluginManager::OrderPlugins()
{

	// Put DICOMPlugin at the beginning for backwards compatibility
	PluginLoaderList::iterator itLib;
	for (itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
	{
		if ( (*itLib)->GetName( ) == "DICOMPlugin" )
		{
			FrontEndPluginLoader::Pointer loader = (*itLib);
			m_LoadedPlugins.erase( itLib );
			m_LoadedPlugins.push_front( loader );
			break;
		}
	}

	// Put MITKPlugin at the beginning for backwards compatibility
	for (itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
	{
		if ( (*itLib)->GetName( ) == "MITKPlugin" )
		{
			FrontEndPluginLoader::Pointer loader = (*itLib);
			m_LoadedPlugins.erase( itLib );
			m_LoadedPlugins.push_front( loader );
			break;
		}
	}


}

void Core::Runtime::FrontEndPluginManager::OrderPluginsDependencies()
{
	try
	{

		// Vertex are identified by ints
		// So we need to create associations between plugins and ints
		std::map<std::string, int> pluginIndexMap;
		std::vector<std::string> pluginNamesVector;

		// Fill structures 
		PluginLoaderList::iterator it;
		int pos = 0;
		for ( it = m_LoadedPlugins.begin();it != m_LoadedPlugins.end() ; ++it)
		{
			pluginIndexMap[ (*it)->GetName() ] = pos;
			pluginNamesVector.push_back( (*it)->GetName() );
			pos++;
		}

		// Fill plugin dependencies 
		// For each dependency, create an edge
		typedef std::pair<int, int> Edge;
		std::vector<Edge> used_by;
		for ( it = m_LoadedPlugins.begin();it != m_LoadedPlugins.end() ; ++it)
		{
			// Fill plugin dependencies
			std::list<std::string> dependencies = (*it)->GetDependencies();
			std::list<std::string>::iterator itDepen;
			for ( itDepen = dependencies.begin();itDepen != dependencies.end() ; ++itDepen)				
			{

				std::map<std::string, int>::iterator itMap;
				itMap = pluginIndexMap.find( *itDepen );
				if ( itMap != pluginIndexMap.end() )
				{
					Edge edge( 
						pluginIndexMap[ *itDepen ], 
						pluginIndexMap[ (*it)->GetName() ] );
					used_by.push_back( edge );
				}
			}
		}

		// Create graph
		typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> Graph;
		Graph g(used_by.begin(), used_by.end(), m_LoadedPlugins.size());
		typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

		// Sort graph
		typedef std::list<Vertex> MakeOrder;
		MakeOrder make_order;
		boost::topological_sort(g, std::front_inserter(make_order));

		// Reorder plugin list
		PluginLoaderList newList;
		for (MakeOrder::iterator i = make_order.begin();
			i != make_order.end(); ++i)
		{
			newList.push_back( GetPluginLoader( pluginNamesVector[ (*i )] ) );
		}
		m_LoadedPlugins = newList;

		OrderPlugins();

		std::cout << "Plugin list: ";
		PluginLoaderList::iterator itLib;
		for (itLib = m_LoadedPlugins.begin();itLib != m_LoadedPlugins.end();++itLib)				
		{
			std::cout << (*itLib)->GetName( ) << " ";
		}
		std::cout << std::endl;

	}
	coreCatchExceptionsLogAndNoThrowMacro(
		FrontEndPluginManager::OrderPluginsDependencies);

}

Core::Runtime::FrontEndPluginLoader::Pointer 
Core::Runtime::FrontEndPluginManager::GetPluginLoader( 
	const std::string &pluginName )
{
	PluginLoaderList::iterator it;
	for ( it = m_LoadedPlugins.begin();it != m_LoadedPlugins.end() ; ++it)
	{
		if ( (*it)->GetName() == pluginName )
		{
			return (*it);
		}
	}

	return NULL;
}

std::string Core::Runtime::FrontEndPluginManager::GetCurrentPluginName() const
{
	return m_CurrentPluginName;
}


void Core::Runtime::FrontEndPluginManager::LoadPlugins( std::list<std::string> pluginsList )
{
	try
	{
		Core::Profile::Pointer profile = Core::Profile::New();

		std::list<std::string>::iterator it;
		for (it = pluginsList.begin();it != pluginsList.end();++it)				
		{
			Core::Runtime::FrontEndPluginLoader::Pointer loader;
			loader = GetPluginLoader( (*it) );
			if ( loader.IsNotNull() )
			{
				profile->MergeProfiles( loader->GetProfileDescriptor( ) );
			}
			else // Command Line Plugin
			{
				profile->AddToProfile( (*it) );
			}
		}

		CreatePluginsMatchingProfile( profile );

	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::LoadPlugins);
}

ModuleFactory *Core::Runtime::FrontEndPluginManager::GetModuleFactory() 
{
	return m_ModuleFactory;
}

Core::Profile::Pointer Core::Runtime::FrontEndPluginManager::SearchCommandLinePlugins( )
{

	Core::Profile::Pointer scannedProfiles;

	try
	{
		// Clear the collected profile from the plugins and set it to the default
		scannedProfiles = Core::Profile::New();

		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
#ifdef _WIN32
		std::string delim(";");
#else
		std::string delim(":");
#endif

		std::string paths;
		std::vector<std::string> pluginsFolders = settings->GetPluginsFolders();
		std::vector<std::string>::iterator it;
		for ( it = pluginsFolders.begin() ; it != pluginsFolders.end( ) ; it++ )
		{
			blTextUtils::StrSub( *it, "$(GimiasPath)", settings->GetApplicationPath() );

			paths = paths + *it + delim;
			paths = paths + *it + "/debug" + delim;
			paths = paths + *it + "/release" + delim;
		}

		if ( m_ModuleFactory )
		{
			delete m_ModuleFactory;
			m_ModuleFactory = NULL;
		}
		m_ModuleFactory = new ModuleFactory( );
		m_ModuleFactory->SetSearchPaths( paths  );

		// Configure cache path
		Core::IO::Directory::Pointer cachePath = Core::IO::Directory::New( );
		cachePath->SetDirNameFullPath( settings->GetApplicationPath() + Core::IO::SlashChar + "CLPCache" );
		cachePath->Create( );

		m_ModuleFactory->SetCachePath( cachePath->GetFullPathDirName( ) );
		m_ModuleFactory->SetWorkingDirectory( settings->GetApplicationPath() );
		m_ModuleFactory->Scan();

		std::vector<std::string> moduleNames;
		moduleNames = m_ModuleFactory->GetModuleNames();
		for ( int i = 0 ; i < moduleNames.size() ; i++ )
		{
			scannedProfiles->AddToProfile( moduleNames[ i ] );
		}

	}
	coreCatchExceptionsAddTraceAndThrowMacro(FrontEndPluginManager::SearchCommandLinePlugins);
	return scannedProfiles;
}

void Core::Runtime::FrontEndPluginManager::SetCurrentPluginName( const std::string &name )
{
	m_CurrentPluginName = name;
}

Core::Profile::Pointer Core::Runtime::FrontEndPluginManager::GetCLPProfiles() const
{
	return m_CLPProfilesHolder->GetSubject( );
}

void Core::Runtime::FrontEndPluginManager::LoadCommandLinePlugins( Core::Profile::Pointer userProfile )
{

	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

	// For each XML of ModuleFactory -> Register a factory
	std::vector<std::string> moduleNames;
	moduleNames = m_ModuleFactory->GetModuleNames();
	for ( int i = 0 ; i < moduleNames.size() ; i++ )
	{
		ModuleDescription module;
		module = m_ModuleFactory->GetModuleDescription( moduleNames[ i ] );
		if ( userProfile->ProfileHasType( moduleNames[ i ] ) )
		{
			m_CurrentPluginName = moduleNames[ i ];
			graphicalIface->GetMainWindow()->RegisterModule( &module );
		}
	}
}

void Core::Runtime::FrontEndPluginManager::LoadXMLDescriptions( const std::string& path )
{
	// Read the XML filters
	ModuleFactory moduleFactory;
	moduleFactory.SetSearchPaths( path.c_str( ) );
	moduleFactory.Scan();

	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

	// For each XML of ModuleFactory -> Register a factory
	std::vector<std::string> moduleNames;
	moduleNames = moduleFactory.GetModuleNames();
	for ( int i = 0 ; i < moduleNames.size() ; i++ )
	{
		ModuleDescription module;
		module = moduleFactory.GetModuleDescription( moduleNames[ i ] );
		graphicalIface->GetMainWindow()->RegisterModule( &module );
	}
}

void Core::Runtime::FrontEndPluginManager::ScanPluginFolder( const std::string &folder )
{

	// Build the search dir for find plugin libraries (subdirs of ./plugins)
	std::string pluginsDirectories = folder;
	Core::IO::Directory::Pointer dir = Core::IO::Directory::New();
	dir->SetDirNameFullPath(pluginsDirectories);
	dir->GetFilter()->SetMode(Core::IO::DirEntryFilter::SubdirsOnly);
	Core::IO::FileNameList pluginList = dir->GetContents();

	// Report the number of available plugins
	char nPluginsStr[48];
	sprintf(nPluginsStr, "Found %d item(s) in plugins directory", (int)pluginList.size());
	std::string message(nPluginsStr);
	Core::Runtime::Kernel::GetGraphicalInterface()->LogMessage(message);

	// For each directory, try to load XML plugin
	for (Core::IO::FileNameList::iterator it = pluginList.begin(); it != pluginList.end(); ++it) 
	{
		Core::Runtime::FrontEndPluginLoader::Pointer loader;
		loader = Core::Runtime::FrontEndPluginLoader::New( );
		loader->SetPath( (*it) );
		loader->LoadXML( );
		if ( !loader->GetLibraryFilename().empty() )
		{
			// Push the library in the opened shared libraries list for later close
			m_LoadedPlugins.push_back( loader );
		}

	}
}

void Core::Runtime::FrontEndPluginManager::ScanPluginsFolders()
{
	Core::Runtime::Kernel::ApplicationRuntimePointer app;
	app = Core::Runtime::Kernel::GetApplicationRuntime( );

	try
	{
		app->SetAppState( Core::Runtime::APP_STATE_PROCESSING );

		PreLoadPluginLibraries();
		m_AvailableProfilesHolder->SetSubject( InferAvailableProfilesFromPlugins() );
		m_CLPProfilesHolder->SetSubject( SearchCommandLinePlugins( ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro(FrontEndPluginManager::FrontEndPluginManager)

	app->SetAppState( Core::Runtime::APP_STATE_IDLE );
}

Core::DataHolder<Core::Profile::Pointer>::Pointer 
Core::Runtime::FrontEndPluginManager::GetAvailableProfilesHolder() const
{
	return m_AvailableProfilesHolder;
}

Core::DataHolder<Core::Profile::Pointer>::Pointer
Core::Runtime::FrontEndPluginManager::GetCLPProfilesHolder() const
{
	return m_CLPProfilesHolder;
}