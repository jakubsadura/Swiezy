/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreAssert.h"
#include "coreSettings.h"
#include "coreDirectory.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "boost/filesystem.hpp"

using namespace Core::Runtime;
using namespace Core::IO;

/** 
*/
Settings::Settings(const std::string& argv0)
{
	try
	{
		// Assign default style and functional values
		m_KernelVersion = "v1.3.0";
		m_StudioName = "CISTIB";
		m_CompanyName = "Group for Computational Image and Simulation Technologies in Biomedicine";
		m_ProjectName = "GIMIAS";
#if defined(WIN32) || defined(_WINDOWS_)
		m_ProjectHomeFolderName = "gimias";
#else
		m_ProjectHomeFolderName = ".gimias";
#endif
		m_Year = "2010";
		m_ApplicationTitle = "GIMIAS";
		m_DefaultCorpLogoFileName = "logoCilab.png";
		m_DefaultLogFileName = "logFile.txt";
		m_DefaultConfigFileName = "config.xml";
		m_DefaultConfigDTDFileName = "config.dtd";
		m_DefaultPluginsDirName = "plugins";
		m_DefaultResourceDirName = "resource";
		m_DefaultModulesDirName = "modules";
		m_DefaultDataDirName = "data";

		// Compound variables and computed fields
		m_ApplicationPath = boost::filesystem::path(argv0).branch_path().string();
		// If using command line in Windows and the same directory, 
		// we need to add "." or it takes the root directory for the rest of variables
		if ( m_ApplicationPath.empty( ) )
		{
			m_ApplicationPath = ".";
		}
		m_ApplicationTitleAndVersion = m_ApplicationTitle + " " + m_KernelVersion;
		m_ScriptVersion = m_ProjectName + "_" + m_KernelVersion;
		m_PluginsPath = m_ApplicationPath + SlashChar + m_DefaultPluginsDirName;
		m_ResourcePath = m_ApplicationPath + SlashChar + m_DefaultResourceDirName;
		m_ModulesPath = m_ApplicationPath + SlashChar + m_DefaultModulesDirName;
		m_DataSourcePath = m_ApplicationPath + SlashChar + m_DefaultDataDirName;
		m_CorpLogoFileName = GetResourcePath() + SlashChar + m_DefaultCorpLogoFileName;
		m_ProjectHomePath = DirectoryHelper::GetApplicationData() + SlashChar + m_ProjectHomeFolderName + SlashChar + m_KernelVersion;
		m_ConfigFileFullPath = GetProjectHomePath() + SlashChar + m_DefaultConfigFileName;
		m_ConfigDTDSourceFileFullPath = GetResourcePath() + SlashChar + m_DefaultConfigDTDFileName;
		m_ConfigDTDTargetFileFullPath = GetProjectHomePath() + SlashChar + m_DefaultConfigDTDFileName;
		m_LogFileFullPath = GetProjectHomePath() + SlashChar + m_DefaultLogFileName;

		std::string path = "$(GimiasPath)";
		path += SlashChar + m_DefaultPluginsDirName;
		blTextUtils::StrSub( path, "release", "debug" );
		m_configVars.m_PluginsFoldersDebug.push_back( path );
		path = "$(GimiasPath)/commandLinePlugins";
		blTextUtils::StrSub( path, "release", "debug" );
		m_configVars.m_PluginsFoldersDebug.push_back( path );

		path = "$(GimiasPath)";
		path += SlashChar + m_DefaultPluginsDirName;
		blTextUtils::StrSub( path, "debug", "release" );
		m_configVars.m_PluginsFoldersRelease.push_back( path );
		path = "$(GimiasPath)/commandLinePlugins";
		blTextUtils::StrSub( path, "debug", "release" );
		m_configVars.m_PluginsFoldersRelease.push_back( path );

		// Start the IO interface for load / save settings
		m_SettingsIO = Core::IO::SettingsIO::New( );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Settings::Settings);
}

/** 
*/
Settings::~Settings(void)
{
	SaveSettings();
}

/** Read all configuration values from the file */
bool Settings::LoadSettings( const char* path )
{
	try
	{
		std::string configFileFullPath;
		if ( path == NULL )
		{
			if( boost::filesystem::exists( GetConfigDTDTargetFileFullPath() ) )
			{
				boost::filesystem::remove( GetConfigDTDTargetFileFullPath() );
			}

			// std::cout << "copy file from  "<<GetConfigDTDSourceFileFullPath() << " to " << GetConfigDTDTargetFileFullPath() << std::endl;
			boost::filesystem::copy_file(
				GetConfigDTDSourceFileFullPath(),
				GetConfigDTDTargetFileFullPath());

			configFileFullPath = GetConfigFileFullPath();
		}
		else
		{
			configFileFullPath = path + SlashChar + m_DefaultConfigFileName;
		}

		m_SettingsIO->ReadConfigFromFile(
			configFileFullPath, 
			m_configVars);

		return true;
	}
	coreCatchExceptionsReportAndNoThrowMacro(Settings::LoadSettings);

	return false;
}


/** Saves all configuration values to the file */
void Settings::SaveSettings(void)
{
	try
	{
		m_SettingsIO->WriteConfigToFile(
			GetConfigFileFullPath(), 
			GetConfigDTDTargetFileFullPath(), 
			m_ProjectName,
			m_configVars
			);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Settings::SaveSettings);
}


/** Returns true if the application was started for first time */
bool Settings::IsFirstTimeStart() const
{ 
	return !m_configVars.m_startedOnce;
}

/** Sets the application was started for first time */
void Settings::SetFirstTimeStart(bool value)
{
	m_configVars.m_startedOnce = !value;
}


/** Returns a pointer to the current profile */
Core::Profile::Pointer Settings::GetCurrentUserProfile(void) const
{
	return m_configVars.m_profile;
}


/** 
Returns the absolute path to the resource dir of the given plugin, and appends the given filename.
This method is really useful for finding and loading resources like sounds or images required and 
provided with the plugin distribution.
\param pluginName Name of the plugin dir under plugins/ directory
\param filename target resource file
*/
std::string Settings::GetPluginResourceForFile(const std::string& pluginName, const std::string& filename) const
{
	std::string dir = GetPluginsPath() + SlashChar + pluginName + \
		SlashChar + m_DefaultResourceDirName + SlashChar + filename;
	return dir;
}


/** 
Returns the absolute path to the resource dir of the Core, and appends the given filename.
This method is really useful for finding and loading resources like sounds or images required and 
provided with the Core distribution.
\param filename target resource file
*/
std::string Settings::GetCoreResourceForFile(const std::string& filename) const
{
	std::string dir = GetResourcePath() + SlashChar + filename;
	return dir;
}


std::string Core::Runtime::Settings::GetApplicationTitleAndVersion() const
{
	return m_ApplicationTitleAndVersion;
}

std::string Core::Runtime::Settings::GetApplicationTitle() const
{
	return m_ApplicationTitle;
}

std::string Core::Runtime::Settings::GetYear() const
{
	return m_Year;
}

std::string Core::Runtime::Settings::GetCorpLogoFileName() const
{
	return m_CorpLogoFileName;
}

std::string Core::Runtime::Settings::GetCompanyName() const
{
	return m_CompanyName;
}

std::string Core::Runtime::Settings::GetProjectHomeFolderName() const
{
	return m_ProjectHomeFolderName;
}

std::string Core::Runtime::Settings::GetApplicationPath() const
{
	return m_ApplicationPath;
}

std::string Core::Runtime::Settings::GetScriptVersion() const
{
	return m_ScriptVersion;
}

std::string Core::Runtime::Settings::GetKernelVersion() const
{
	return m_KernelVersion;
}

std::string Core::Runtime::Settings::GetStudioName() const
{
	return m_StudioName;
}

std::string Core::Runtime::Settings::GetProjectName() const
{
	return m_ProjectName;
}

std::string Core::Runtime::Settings::GetPluginsPath() const
{
	std::string path = m_PluginsPath;
	blTextUtils::StrSub( path, "$(GimiasPath)", m_ApplicationPath );
	return path;
}

std::string Core::Runtime::Settings::GetResourcePath() const
{
	return m_ResourcePath;
}

std::string Core::Runtime::Settings::GetModulesPath() const
{
	return m_ModulesPath;
}

std::string Core::Runtime::Settings::GetDataSourcePath() const
{
	return m_DataSourcePath;
}

std::string Core::Runtime::Settings::GetProjectHomePath() const
{
	return m_ProjectHomePath;
}

std::string Core::Runtime::Settings::GetConfigFileFullPath() const
{
	return m_ConfigFileFullPath;
}

std::string Core::Runtime::Settings::GetConfigDTDSourceFileFullPath() const
{
	return m_ConfigDTDSourceFileFullPath;
}

std::string Core::Runtime::Settings::GetConfigDTDTargetFileFullPath() const
{
	return m_ConfigDTDTargetFileFullPath;
}

std::string Core::Runtime::Settings::GetLogFileFullPath() const
{
	return m_LogFileFullPath;
}

Core::Runtime::Settings::Pointer Core::Runtime::Settings::New( const std::string& argv0 )
{
	Pointer smartPtr;
	Settings *rawPtr = new Settings(argv0);
	smartPtr = rawPtr;
	rawPtr->UnRegister();
	return smartPtr;
}

std::string Core::Runtime::Settings::GetApplicationPathSubPath( const std::string& subPath )
{
	return (boost::filesystem::path(GetApplicationPath()) / subPath).string();
}

std::vector<std::string> Core::Runtime::Settings::GetMRUList()
{
	return m_configVars.m_MRUList;
}

void Core::Runtime::Settings::SetMRUList( std::vector<std::string> list )
{
	m_configVars.m_MRUList = list;
}

std::string Core::Runtime::Settings::GetLastOpenedPath(int pathType /*= 0*/)
{
	switch(pathType)
	{
		case 0:
			return m_configVars.m_lastOpenedPath; break;
		case 1:
			return m_configVars.m_lastOpenedDicomDirPath; break;
		case 2:
			return m_configVars.m_lastOpenedDicomFilePath; break;			
		default:
			return m_configVars.m_lastOpenedPath; break;
	}

}

std::string Core::Runtime::Settings::GetLastSavePath( )
{
	if ( m_configVars.m_lastSavePath.empty() )
	{
		return GetDataSourcePath();
	}

	return m_configVars.m_lastSavePath;
}

void Core::Runtime::Settings::SetLastSavePath( const std::string path )
{
	m_configVars.m_lastSavePath = path;
}

void Core::Runtime::Settings::SetLastOpenedPath( std::string strPath, int pathType /*= 0*/ )
{
	switch(pathType)
	{
		case 0:
			m_configVars.m_lastOpenedPath = strPath; break;
		case 1:
			m_configVars.m_lastOpenedDicomDirPath = strPath; break;
		case 2:
			m_configVars.m_lastOpenedDicomFilePath = strPath; break;			
		default:
			m_configVars.m_lastOpenedPath = strPath; break;
	}
}

std::string Core::Runtime::Settings::GetPacsCalledAE()
{
	return m_configVars.m_pacsCalledAE;
}

void Core::Runtime::Settings::SetPacsCalledAE( std::string strCalledAE)
{
	m_configVars.m_pacsCalledAE=strCalledAE;
}

std::string Core::Runtime::Settings::GetPacsCallingAE()
{
	return m_configVars.m_pacsCallingAE;
}

void Core::Runtime::Settings::SetPacsCallingAE( std::string strCallingAE)
{
	m_configVars.m_pacsCallingAE=strCallingAE;
}


std::string Core::Runtime::Settings::GetCurrentDataPath()
{
	std::string dataPath;
	dataPath = GetLastOpenedPath( );
	if ( dataPath.empty() )
	{
		dataPath = GetDataSourcePath();
	}
	return dataPath;
}


void Core::Runtime::Settings::TestCreateConfigFile()
{
	m_SettingsIO->TestCreateConfigFile(GetProjectHomePath(), GetConfigFileFullPath());
}

void Core::Runtime::Settings::SetLayoutConfiguration( 
	const char *pluginName, 
	const std::string &strLayoutConfig )
{
	m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties[ "layout" ] = strLayoutConfig;
}

void Core::Runtime::Settings::GetLayoutConfiguration( 
	const char *pluginName, 
	std::string &strLayoutConfig )
{
	strLayoutConfig = 
		m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties[ "layout" ];	
}

PERSPECTIVE_TYPE Core::Runtime::Settings::GetPerspective()
{
	PERSPECTIVE_TYPE type;
	if ( m_configVars.m_Perspective == "Plugin" || m_configVars.m_Perspective == "" )
	{
		type = PERSPECTIVE_PLUGIN;
	}
	else if ( m_configVars.m_Perspective == "Workflow" )
	{
		type = PERSPECTIVE_WORKFLOW;
	}
	return type;
}

void Core::Runtime::Settings::SetPerspective( PERSPECTIVE_TYPE type )
{
	switch ( type )
	{
	case PERSPECTIVE_PLUGIN: m_configVars.m_Perspective = "Plugin"; break;
	case PERSPECTIVE_WORKFLOW: m_configVars.m_Perspective = "Workflow"; break;
	}
}

std::string Core::Runtime::Settings::GetActiveWorkflow()
{
	return m_configVars.m_ActiveWorkflow;
}

void Core::Runtime::Settings::SetActiveWorkflow( const std::string &name )
{
	m_configVars.m_ActiveWorkflow = name;
}

void Core::Runtime::Settings::SetImportConfiguration( bool val )
{
	m_configVars.m_ImportConfiguration = val;
}

bool Core::Runtime::Settings::GetImportConfiguration()
{
	return m_configVars.m_ImportConfiguration;
}

std::vector<std::string> Core::Runtime::Settings::GetPluginsFolders()
{
#ifdef _DEBUG
	return m_configVars.m_PluginsFoldersDebug;
#else
	return m_configVars.m_PluginsFoldersRelease;
#endif
}

void Core::Runtime::Settings::SetPluginsFolders( const std::vector<std::string> folders )
{
#ifdef _DEBUG
	m_configVars.m_PluginsFoldersDebug = folders;
#else
	m_configVars.m_PluginsFoldersRelease = folders;
#endif
}

bool Core::Runtime::Settings::GetShowRegistrationForm()
{
	return m_configVars.m_ShowRegistrationForm;
}

void Core::Runtime::Settings::SetShowRegistrationForm( bool val )
{
	m_configVars.m_ShowRegistrationForm = val;
}

bool Core::Runtime::Settings::GetUserRegistered()
{
	return m_configVars.m_UserRegistered;
}

void Core::Runtime::Settings::SetUserRegistered( bool val )
{
	m_configVars.m_UserRegistered = val;
}

void Core::Runtime::Settings::SetPluginProperty( 
	const char *pluginName, const std::string &name, const std::string &val )
{
	m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties[ name ] = val;
}

bool Core::Runtime::Settings::GetPluginProperty( 
	const char *pluginName, const std::string &name, std::string &value )
{
	std::map<std::string, std::string>::iterator it;
	it = m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties.find( name );
	if ( it == m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties.end() )
	{
		return false;
	}

	value = m_configVars.m_MapPluginConfiguration[ pluginName ].m_Properties[ name ];

	return true;
}
