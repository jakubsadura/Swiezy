/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSettingsIO_H
#define coreSettingsIO_H

#include "coreObject.h"
#include "coreConfigVars.h"
#include "coreXMLDOMHandler.h"
#include <string>


namespace Core
{
// forward declarations
namespace Runtime { class Settings; }

namespace IO
{

/** 
\brief Store and retrieve configuration data in cross platform way. 
It provides XML access to the settings file of the application.

\note Note that its methods are only accessible from the Settings manager. 
You shall not use directly this class.

\ingroup gmIO
\sa Core::Runtime::Settings
\author Juan Antonio Moya
\date 18 Apr 2007
*/
class GMIO_EXPORT SettingsIO : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro( SettingsIO, Core::SmartPointerObject);

	/**
	Check if the config file already exists, if don't, just create an 
	empty file for that user
	*/
	void TestCreateConfigFile(
		const std::string& projectHomePath, 
		const std::string& configFileFullPath);

	//! Read config file
	void ReadConfigFromFile(
		const std::string& configFileFullPath, 
		ConfigVars& configVars);

	//! Write config file
	void WriteConfigToFile(
		const std::string& configFileFullPath, 
		const std::string& dtdFileFullPath, 
		const std::string& projectName, 
		ConfigVars& configVars);

private:
	SettingsIO( );

	~SettingsIO( );

	coreDeclareNoCopyConstructors( SettingsIO );

	//!
	void ReadProfile( 
		XML::DOMHandler::Pointer xmlHandler, 
		ConfigVars& configVars );

	//!
	void WriteProfile( 
		xercesc::DOMDocument* document, 
		const ConfigVars& configVars );

	//!
	void ReadStringVector( 
		XML::DOMHandler::Pointer xmlHandler,
		const char* tagName,
		const char* subtagName,
		std::vector<std::string> &strVector );

	//!
	void WriteStringVector( 
		xercesc::DOMDocument* document, 
		const char* tagName,
		const char* subtagName,
		const std::vector<std::string> &strVector );

	//!
	void ReadPluginConfiguration( 
		XML::DOMHandler::Pointer xmlHandler, 
		ConfigVars& configVars );

	//!
	void WritePluginConfiguration(
		xercesc::DOMDocument* document, 
		ConfigVars& configVars );

	//!
	std::string ReadTextVariable( 
		XML::DOMHandler::Pointer xmlHandler,
		const char* tagName );

	//!
	void WriteTextVariable( 
		xercesc::DOMDocument* document, 
		xercesc::DOMElement* parent,
		const char* tagName,
		std::string strValue );

	//! Return "true" or "false"
	std::string Bool2String( bool bValue );

	//! "true" or "false" to bool
	bool String2Bool( std::string bValue );

	//!
	xercesc::DOMElement* CreateElement( 
		xercesc::DOMDocument* document, 
		const std::string &strName );

	//!
	xercesc::DOMText* CreateTextNode( 
		xercesc::DOMDocument* document,
		const std::string &strName );

	//!
	void SetNodeAttribute( 
		xercesc::DOMDocument* document,
		xercesc::DOMElement* elem,
		const std::string &strAttribute,
		const std::string &strValue );

private:

};

} // namespace Core
} // namespace IO


#endif
