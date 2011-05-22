/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSettings_H
#define coreSettings_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
#include "coreConfigVars.h"
#include "coreSettingsIO.h"
#include <string>

namespace Core
{

namespace Runtime
{

	enum PERSPECTIVE_TYPE
	{
		PERSPECTIVE_PLUGIN,
		PERSPECTIVE_WORKFLOW
	};

/** 
\brief Store and retrieve configuration data in a platform-independent style. 

The settings also stores the user profile, so reading it determines all 
the actor types he pertains. Profile only will work when running in 
graphical mode

\sa Core::Profile
\ingroup gmKernel
\author Juan Antonio Moya
\date 20 May 2006
*/

class GMKERNEL_EXPORT Settings : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(Settings,Core::SmartPointerObject)
	coreClassNameMacro(Settings)

	/** Factory method for creating a new instance.
	 \param argv0 - The first argument of the GIMIAS main function 
	 (the filename of the GIMIAS executable). 
	 */
	static Pointer New(const std::string& argv0);
	
	std::string GetApplicationTitleAndVersion() const;
	std::string GetApplicationTitle() const;
	std::string GetYear() const;
	std::string GetCorpLogoFileName() const;
	std::string GetCompanyName() const;
	std::string GetProjectHomeFolderName() const;
	std::string GetApplicationPath() const;
	//! Returns a \a subpath from the application path.
	std::string GetApplicationPathSubPath(const std::string& subPath);
	std::string GetScriptVersion() const;
	std::string GetKernelVersion() const;
	std::string GetStudioName() const;
	std::string GetProjectName() const;
	std::string GetPluginsPath() const;
	std::string GetResourcePath() const;
	std::string GetModulesPath() const;
	std::string GetDataSourcePath() const;
	std::string GetProjectHomePath() const;
	std::string GetConfigFileFullPath() const;
	//! See m_ConfigDTDSourceFileFullPath
	std::string GetConfigDTDSourceFileFullPath() const;
	//! See m_ConfigDTDTargetFileFullPath
	std::string GetConfigDTDTargetFileFullPath() const;
	std::string GetLogFileFullPath() const;

	bool LoadSettings( const char* path = NULL );
	void SaveSettings(void);

	bool IsFirstTimeStart(void) const;

	std::string GetPluginResourceForFile(const 
		std::string& pluginName, 
		const std::string& filename) const;
	std::string GetCoreResourceForFile(const std::string& filename) const;
	Core::Profile::Pointer GetCurrentUserProfile(void) const;

	/** Calls TestCreateConfigFile on this->m_SettingsIO (MN: I have no 
	idea why this is needed, and would
	 love to work on refactoring this asap :-( )
	 */
	void TestCreateConfigFile();

	//!
	std::vector<std::string> GetMRUList( );

	//!
	void SetMRUList( std::vector<std::string> list );

	//! Get GetLastOpenedPath( ) if its not empty, else GetDataSourcePath( )
	std::string GetCurrentDataPath( );

	//! Get Called AE information
	std::string GetPacsCalledAE( );

	//! Get Calling AE information
	std::string GetPacsCallingAE( );

	//! Set Called AE information strCalledAE must be of the type AETitle:IPAdress:TCPPort
	void SetPacsCalledAE( std::string strCalledAE);

	//! Set Calling AE information strCallingAE must be of the type AETitle:IPAdress:TCPPort
	void SetPacsCallingAE( std::string strCallingAE);

	//! pathtype: 0 -> data file, 1 -> dicom dir file, 2-> dicom file 
	std::string GetLastOpenedPath( int pathType = 0);

	//! 
	std::string GetLastSavePath( );

	//! 
	void SetLastSavePath( const std::string path );

	//! pathtype: 0 -> data file, 1 -> dicom dir file, 2-> dicom file
	void SetLastOpenedPath( std::string strPath, int pathType = 0 );

	void SetMainAppName(const std::string& value);
	void SetFirstTimeStart(bool value);

	//! Layout configuration for a plugin
	void SetLayoutConfiguration( 
		const char *pluginName,
		const std::string &strLayoutConfig );

	//! Layout configuration for a plugin
	void GetLayoutConfiguration( 
		const char *pluginName,
		std::string &strLayoutConfig );

	//!
	PERSPECTIVE_TYPE GetPerspective( );
	void SetPerspective( PERSPECTIVE_TYPE type );

	//!
	std::string GetActiveWorkflow( );
	void SetActiveWorkflow( const std::string &name );

	//!
	void SetImportConfiguration( bool val );
	bool GetImportConfiguration( );

	//!
	std::vector<std::string> GetPluginsFolders( );
	void SetPluginsFolders( const std::vector<std::string> folders );

	//!
	bool GetShowRegistrationForm( );
	void SetShowRegistrationForm( bool val );

	//!
	bool GetUserRegistered( );
	void SetUserRegistered( bool val );

	//! Generic property for a plugin
	void SetPluginProperty( 
		const char *pluginName,
		const std::string &name,
		const std::string &value );

	//! Generic property for a plugin
	bool GetPluginProperty( 
		const char *pluginName,
		const std::string &name,
		std::string &value );

protected:
	/**
	\brief Initializer for the class Settings.
	 When creating also tests creating configuration file 
	*/
	Settings(const std::string& argv0);

	/**
	\brief Destructor for the class Settings.
	\todo Mutex XMLWriter or de-block it on destruction time. By now: do 
	not Save settings on application destruction, or XML system will not 
	be able to parse the file because it could be blocked by the XMLWriter.
	*/
	virtual ~Settings(void);

private:
	// Member constants
	std::string m_KernelVersion;
	std::string m_ScriptVersion;
	std::string m_StudioName;
	std::string m_ProjectName;
	std::string m_ApplicationTitle;
	std::string m_ApplicationTitleAndVersion;
	std::string m_Year;
	std::string m_CorpLogoFileName;
	std::string m_CompanyName;
	std::string m_DefaultLogFileName;
	std::string m_DefaultConfigFileName;
	std::string m_DefaultConfigDTDFileName;
	std::string m_DefaultCorpLogoFileName;
	std::string m_DefaultPluginsDirName;
	std::string m_DefaultResourceDirName;
	std::string m_DefaultModulesDirName;
	std::string m_DefaultDataDirName;

	// Computable values
	std::string m_ProjectHomeFolderName;
	std::string m_ApplicationPath;
	std::string m_PluginsPath;
	std::string m_ResourcePath;
	std::string m_ModulesPath;
	std::string m_DataSourcePath;
	std::string m_ProjectHomePath;
	std::string m_ConfigFileFullPath;
	//! This is the dtd file that is part of the GIMIAS resources.
	std::string m_ConfigDTDSourceFileFullPath;
	/** This is the dtd file that is copied to the users folder (and is 
	used to verify the user profile).
	*/
	std::string m_ConfigDTDTargetFileFullPath;
	std::string m_LogFileFullPath;

	//! Config vars got from the IO
	Core::IO::ConfigVars m_configVars;
	//! Read and save ConfigVars
	Core::IO::SettingsIO::Pointer m_SettingsIO;

	coreDeclareNoCopyConstructors(Settings);

};
}
}


#endif
