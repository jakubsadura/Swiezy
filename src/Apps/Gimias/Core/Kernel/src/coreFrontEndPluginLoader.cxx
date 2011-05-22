/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreFrontEndPluginLoader.h"
#include "coreSettings.h"
#include "coreKernel.h"
#include "dynLib.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreLogger.h"

#include "tinyxml.h"

using namespace Core::Runtime;
int FrontEndPluginLoader::m_PluginNameIndex = 0;

FrontEndPluginLoader::FrontEndPluginLoader(void)
{
	m_LibraryHandle = NULL;
	// Create a default unique name
	char name[128];
	sprintf( name, "Plugin%d", m_PluginNameIndex++ );
	m_Name = name;
}

FrontEndPluginLoader::~FrontEndPluginLoader(void)
{
	CloseLibrary();
}

Core::FrontEndPlugin::BaseFrontEndPlugin::Pointer 
Core::Runtime::FrontEndPluginLoader::GetBaseFrontEndPlugin() 
{
	return m_BaseFrontEndPlugin;
}

void Core::Runtime::FrontEndPluginLoader::SetPath( const std::string path )
{
	m_Path = path;
}

itksys::DynamicLoader::LibraryHandle 
Core::Runtime::FrontEndPluginLoader::GetLibraryHandle() 
{
	return m_LibraryHandle;
}

Core::Profile::Pointer Core::Runtime::FrontEndPluginLoader::GetProfileDescriptor()
{

	if ( m_Profile.IsNull( ) )
	{
		GetProfileFromPluginFunc profileDescriptor;
		itksys::DynamicLoader::SymbolPointer symbol;

		// Assign handles to NULL (not required but makes easier to track errors)
		symbol = NULL;
		profileDescriptor = NULL;

		// Resolve the procedure "getProfileDescriptor" of the current plugin library							
		symbol = itksys::DynamicLoader::GetSymbolAddress( m_LibraryHandle, "getProfileDescriptor");
		if (symbol == NULL)
		{
			// If the symbol was not found, display error
			Core::Exceptions::OnLoadPluginProcedureNotFoundException eNotFound("FrontEndPluginManager::LoadPlugins");
			throw eNotFound;
		}


		try
		{  
			// Create the FrontEndPlugin object from the library
			profileDescriptor = reinterpret_cast<GetProfileFromPluginFunc>(symbol);
			m_Profile = profileDescriptor();
		}
		catch(Core::Exceptions::Exception& e)
		{
			e.AddTrace("FrontEndPluginManager::LoadPlugins");
			throw e;
		}
		catch(...)
		{
			// If the symbol was not found, display error
			Core::Exceptions::CannotReadProfileException eBadProf("FrontEndPluginManager::LoadPlugins");
			throw eBadProf;
		}

	}

	return m_Profile;
}

void Core::Runtime::FrontEndPluginLoader::ResetBaseFrontEndPlugin()
{
	m_BaseFrontEndPlugin = NULL;
}

void Core::Runtime::FrontEndPluginLoader::LoadLibrary()
{
	try
	{
		LoadLibraryHandle( m_LibraryFilename );
	}
	catch (Core::Exceptions::Exception& e)
	{
		Core::Runtime::Kernel::GetGraphicalInterface()->LogException(e);
		Core::Runtime::Kernel::GetLogManager()->LogException(e);
	}

}

void Core::Runtime::FrontEndPluginLoader::LoadLibraryHandle( 
	const std::string &filePath )
{
	if ( m_LibraryHandle == NULL )
	{
		if( !(boost::ends_with(filePath, ".dll") || boost::ends_with(filePath, ".so"))
			|| boost::contains(filePath, "dephelp"))
			return ;

		// Init the Library resource
		m_LibraryHandle = itksys::DynamicLoader::OpenLibrary( filePath.c_str() );

		if (m_LibraryHandle == NULL)
		{
			// Check if the problem is related to missing dependencies
			std::string dllArrayNames;
#ifdef _WIN32
			Core::Runtime::Settings::Pointer settings;
			settings = Core::Runtime::Kernel::GetApplicationSettings();
			dynLib lib;
			lib.CheckMissingDependencies( m_LibraryFilename.c_str(), settings->GetApplicationPath().c_str(), dllArrayNames );
#endif // _WIN32

			if ( !dllArrayNames.empty() )
			{
				Core::Exceptions::OnLoadPluginException eError("FrontEndPluginManager::LoadPlugins");
				eError.Append("Plug-in was: ");
				eError.Append(filePath.c_str());
				eError.Append(" Missing DLLs: ");
				eError.Append( dllArrayNames.c_str() );
				throw eError;
			}
			else
			{
				// If the library was locked already, display an error
				Core::Exceptions::OnLoadPluginObjectAlreadyLockedException eLocked("FrontEndPluginManager::LoadPlugins");
				eLocked.Append("Plug-in was: ");
				eLocked.Append(filePath.c_str());
				throw eLocked;
			}

		}

	}
}

void Core::Runtime::FrontEndPluginLoader::CreateBaseFrontEndPlugin()
{
	itksys::DynamicLoader::SymbolPointer symbol;
	FrontEndPluginConstructorFunc pluginConstructor;

	if ( m_BaseFrontEndPlugin.IsNull( ) )
	{

		symbol = itksys::DynamicLoader::GetSymbolAddress(m_LibraryHandle, "newFrontEndPlugin");
		if (symbol != NULL)
		{
			try
			{  
				// Get the Profile object from the library, describing the profile of the plugin
				pluginConstructor = reinterpret_cast<FrontEndPluginConstructorFunc>(symbol);
				m_BaseFrontEndPlugin = pluginConstructor();

				if(m_BaseFrontEndPlugin.IsNull())
				{
					// If the plugin could not be loaded, display error
					Core::Exceptions::OnLoadPluginBadObjectException eBadObj("FrontEndPluginManager::LoadPlugins");
					throw eBadObj;
				}
			}
			catch(...)
			{
				// If the plugin could not be loaded, display error
				Core::Exceptions::OnLoadPluginException eErr("FrontEndPluginManager::LoadPlugins");
				throw eErr;
			}
		}
		else
		{
			// If the symbol was not found, display error
			Core::Exceptions::OnLoadPluginProcedureNotFoundException eNotFound("FrontEndPluginManager::LoadPlugins");
			throw eNotFound;
		}
	}
}

void Core::Runtime::FrontEndPluginLoader::ReadXML()
{
	TiXmlDocument doc( m_LibraryPath + "/plugin.xml" );
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem=hDoc.FirstChildElement().Element();
	if (!pElem) return;
	m_Name = pElem->Attribute("name");
	hRoot=TiXmlHandle(pElem);

	pElem=hRoot.FirstChild( "depends" ).FirstChild().Element();
	for( pElem; pElem; pElem=pElem->NextSiblingElement())
	{
		const char *pName=pElem->Attribute("name");
		m_Dependencies.push_back( pName );
	}

}

void Core::Runtime::FrontEndPluginLoader::WriteXML( )
{
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * plugin = new TiXmlElement( "plugin" );
	plugin->SetAttribute("name", "CardiacQuantificationPlugin");
	doc.LinkEndChild( plugin );

	TiXmlElement * pluginDependencies = new TiXmlElement( "depends" );
	plugin->LinkEndChild( pluginDependencies );

	TiXmlElement * pluginName;
	pluginName = new TiXmlElement( "plugin" );
	pluginName->SetAttribute("name", "SignalViewerPlugin");
	pluginDependencies->LinkEndChild( pluginName );

	pluginName = new TiXmlElement( "plugin" );
	pluginName->SetAttribute("name", "DICOMPlugin");
	pluginDependencies->LinkEndChild( pluginName );

	doc.SaveFile( m_LibraryPath + "/plugin.xml" );
}

void Core::Runtime::FrontEndPluginLoader::FindLibraryPath()
{
	// If was a directory, build the <pluginName>/lib path to access the shared library
	if(m_Path.compare(".") == 0 && m_Path.compare("..") == 0)
	{
		return;
	}

	std::vector<std::string> modes;
	modes.push_back("Debug");
	modes.push_back("Release");
	// also look in the current folder, for backward compatibility with previous code.
	modes.push_back("."); 

	std::vector<std::string>::const_iterator itMode;
	for( itMode = modes.begin(); itMode != modes.end(); ++itMode )
	{
		std::string pluginSharedLibraryDir;
		pluginSharedLibraryDir = m_Path;
		pluginSharedLibraryDir.append("/lib/" + *itMode );

		// Prepare the dir for grabbing files
		Core::IO::Directory::Pointer dir = Core::IO::Directory::New();
		// Prepare the directory for grabbing files
		dir->GetFilter()->SetMode(Core::IO::DirEntryFilter::FilesOnly);
		dir->GetFilter()->ResetGlobbingExpressions();
		dir->GetFilter()->AddGlobbingExpression("*.dll");
		dir->GetFilter()->AddGlobbingExpression("*.so");
		dir->SetDirNameFullPath(pluginSharedLibraryDir);

		Core::IO::FileNameList libraryList;
		libraryList = dir->GetContents();

		// Load all the libraries in the plugins/<pluginName>/lib
		Core::IO::FileNameList::iterator itLib;
		for ( itLib = libraryList.begin(); itLib != libraryList.end(); ++itLib)				
		{
			if( (boost::ends_with(*itLib, ".dll") || boost::ends_with(*itLib, ".so"))
				&& !boost::contains(*itLib, "dephelp"))
			{
				m_LibraryPath = pluginSharedLibraryDir;
				m_LibraryFilename = *itLib;
			}

		} // for each shared library

	} // for each mode
}

void Core::Runtime::FrontEndPluginLoader::LoadXML()
{
	FindLibraryPath( );

	ReadXML();
}

std::string Core::Runtime::FrontEndPluginLoader::GetLibraryFilename() const
{
	return m_LibraryFilename;
}

std::list<std::string> Core::Runtime::FrontEndPluginLoader::GetDependencies() const
{
	return m_Dependencies;
}

std::string Core::Runtime::FrontEndPluginLoader::GetName() const
{
	return m_Name;
}

std::string Core::Runtime::FrontEndPluginLoader::GetLibraryPath() const
{
	return m_LibraryPath;
}

void Core::Runtime::FrontEndPluginLoader::CloseLibrary()
{
	ResetBaseFrontEndPlugin( );

	// Close shared library handle
	try
	{
		itksys::DynamicLoader::CloseLibrary( m_LibraryHandle );
		m_LibraryHandle = NULL;
	}
	catch(...) { ; }
}
