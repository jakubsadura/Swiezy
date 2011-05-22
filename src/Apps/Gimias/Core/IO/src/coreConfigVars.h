/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreConfigVars_H
#define coreConfigVars_H

#include "gmIOWin32Header.h"
#include "coreProfile.h"
#include <vector>
#include <map>

namespace Core
{
namespace IO
{

/**
\brief Generic configuration parameters for a plugin
\ingroup gmIO
\date 02 Dec 2008
\author Xavi Planes
*/
class GMIO_EXPORT ConfigVarsPlugin : public Core::Object
{
public:
	ConfigVarsPlugin( );

	std::map<std::string,std::string> m_Properties;
};


/**
\brief GIMIAS configuration parameters that will be stored on disk
\ingroup gmIO
 */
class GMIO_EXPORT ConfigVars : public Core::Object
{
public:
	typedef std::map<std::string,ConfigVarsPlugin> PluginMapType;
	typedef std::pair<std::string,ConfigVarsPlugin> PluginMapPair;

public:
	ConfigVars( );

public:
	//! Application has started once or is the first time
	bool m_startedOnce;
	//! Import Configuration wizard is active
	bool m_ImportConfiguration;
	//! Profile Manager class
	Core::Profile::Pointer m_profile;  
	//! List of MRU
	std::vector<std::string> m_MRUList;
	//! Path of the last opened file
	std::string m_lastOpenedPath;
	//! Path of the last opened Dicom file
	std::string m_lastOpenedDicomFilePath;
	//! Path of the last opened DicomDIR file
	std::string m_lastOpenedDicomDirPath;
	//! Path of the last opened file
	std::string m_lastSavePath;
	//! PACS Called AE info
	std::string m_pacsCalledAE;
	//! PACS Calling AE info
	std::string m_pacsCallingAE;

	//! 
	PluginMapType m_MapPluginConfiguration;

	//! Perspective: Plugin or Workflow
	std::string m_Perspective;

	//! Active Workflow
	std::string m_ActiveWorkflow;

	//! Plugins folders
	std::vector<std::string> m_PluginsFoldersDebug;

	//! Plugins folders
	std::vector<std::string> m_PluginsFoldersRelease;

	//! 
	bool m_ShowRegistrationForm;

	//! 
	bool m_UserRegistered;
};

} // namespace IO
} //namespace Core

#endif // coreConfigVars_H
