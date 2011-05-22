/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreFrontEndPluginManager_H
#define coreFrontEndPluginManager_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include <itksys/DynamicLoader.hxx>
#include "coreProfile.h"
#include "coreDataEntity.h"
#include "coreFrontEndPluginLoader.h"
#include "ModuleFactory.h"

namespace Core
{
namespace Runtime
{

/**
\brief Loads plugin objects dinamically

Plugin Manager searches for shared objects (or dll libraries under 
Windows) in the ./plugins/bin directory and tries to load them if they 
share the profile configuration with the current user. The Plugin Manager 
currently loads objects of type "WidgetPluginConstructor" and actually 
searches for a call named "newFrontEndPlugin".

\image html loading_plugins.png
\sa Creating a plugin
\sa Core::FrontEndPlugin::FrontEndPlugin
\note You should access and get the instance for this class through 
the runtime high level manager object: wxMitkGraphicalInterface
\ingroup gmKernel
\author Juan Antonio Moya
\date 11 Jan 2008
*/
class GMKERNEL_EXPORT FrontEndPluginManager : 
	public Core::SmartPointerObject
{
public:
	typedef std::list<Core::Runtime::FrontEndPluginLoader::Pointer> 
		PluginLoaderList;

	coreDeclareSmartPointerClassMacro(
		FrontEndPluginManager, 
		Core::SmartPointerObject);
	
	//! Return m_AvailableProfiles
	Core::Profile::Pointer GetAvailableProfiles() const;

	//! Holder to nofiy changes
	DataHolder<Core::Profile::Pointer>::Pointer GetAvailableProfilesHolder() const;

	//! Return m_CLPProfiles
	Core::Profile::Pointer GetCLPProfiles() const;

	//! Holder to nofiy changes
	DataHolder<Core::Profile::Pointer>::Pointer GetCLPProfilesHolder() const;

	//! Create BaseFrontEndPlugin instances and attach them to GUI
	void CreatePluginsMatchingProfile(Core::Profile::Pointer userProfile);
	//! Unload all plugins and detach them from GUI
	void UnLoadAllPlugins(bool alsoReleaseResources = false);
	/** 
	When calling plugin constructor, this is the current plugin name
	specified in the xml file
	*/
	std::string GetCurrentPluginName() const;

	void SetCurrentPluginName( const std::string &name );

	//!
	void LoadPlugins( std::list<std::string> pluginsList );

	//!
	ModuleFactory *GetModuleFactory();

	//!
	Core::Runtime::FrontEndPluginLoader::Pointer GetPluginLoader( 
		const std::string &pluginName); 

	//!
	void ScanPluginsFolders( );

	//!
	void LoadCommandLinePlugins( Core::Profile::Pointer userProfile );

	//! Load XML files for GIMIAS widgets descriptions
	void LoadXMLDescriptions( const std::string& path );

protected:
	//!
	FrontEndPluginManager(void);
	//!
	virtual ~FrontEndPluginManager(void);
	//! Set DICOM plugin at position 0
	void OrderPlugins( );
	//! Compute available profiles from loaded libraries
	Core::Profile::Pointer InferAvailableProfilesFromPlugins(void);
	//! Load libraries from folders
	void PreLoadPluginLibraries(void);
	//!
	void OrderPluginsDependencies( );
	//! Add dependent plugins of profile plugins to the userProfile
	void AddPluginsDependencies( Core::Profile::Pointer userProfile );

	//!
	Core::Profile::Pointer SearchCommandLinePlugins( );

	//!
	void ScanPluginFolder( const std::string &folder );

private:

	//! GIMIAS Plugins
	DataHolder<Core::Profile::Pointer>::Pointer m_AvailableProfilesHolder;
	//! Command Line Plugins
	DataHolder<Core::Profile::Pointer>::Pointer m_CLPProfilesHolder;
	//! GIMIAS Plugins
	PluginLoaderList m_LoadedPlugins;
	//! Used to classify the registered factories
	std::string m_CurrentPluginName;
	//! Command Line Plugins
	ModuleFactory *m_ModuleFactory;
	//!
	coreDeclareNoCopyConstructors(FrontEndPluginManager);
};

} // namespace Runtime
} // namespace Core

#endif

