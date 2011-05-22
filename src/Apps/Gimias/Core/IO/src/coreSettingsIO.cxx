/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreAssert.h"
#include "coreProfile.h"
#include "coreException.h"
#include "coreSettingsIO.h"
#include "coreXMLDOMHandler.h"
#include "coreXMLSubsystem.h"
#include "coreDirectory.h"
#include "coreFile.h"
#include <stdio.h>

using namespace Core::IO;

const char* TAG_STARTED_ONCE = "startedOnce";
const char* TAG_PROFILE = "profile";
const char* TAG_PROFILE_ITEM = "profileItem";
const char* TAG_LAST_OPENED_PATH = "lastOpenedPath";
const char* TAG_LAST_SAVE_PATH = "lastSavePath";
const char* TAG_LAST_OPENED_DICOMDIR_PATH = "lastOpenedDicomDirPath";
const char* TAG_LAST_OPENED_DICOMFILE_PATH = "lastOpenedDicomFilePath";
const char* TAG_MRU_LIST = "MRUList";
const char* TAG_MRU_LIST_ITEM = "MRUListItem";
const char* TAG_PLUGIN_CONFIG = "PluginConfiguration";
const char* TAG_PLUGIN = "Plugin";
const char* TAG_PLUGIN_NAME = "PluginName";
const char* TAG_PLUGIN_LAYOUT = "PluginLayout";
const char* TAG_PACS_CALLEDAE = "Pacs Called AE";
const char* TAG_PACS_CALLINGAE = "Pacs Calling AE";
const char* TAG_PERSPECTIVE = "Perspective";
const char* TAG_ACTIVE_WORKFLOW = "Active Workflow";
const char* TAG_STARTED_IMPORT_CONFIGURATION = "importConfigurationWizard";
const char* TAG_PLUGINS_SCAN_FOLDERS_DEBUG = "PluginsScanFoldersDebug";
const char* TAG_PLUGINS_SCAN_FOLDERS_RELEASE = "PluginsScanFoldersRelease";
const char* TAG_PLUGINS_SCAN_FOLDERS_ITEM = "ScanFolder";
const char* TAG_SHOW_REGISTRATION_FORM = "ShowRegistrationForm";
const char* TAG_USER_REGISTERED = "UserRegistered";

const char* TAG_TRUE = "true";
const char* TAG_FALSE = "false";
const char* TAG_NAME = "name";
const char* TAG_VARIABLE = "variable";

/** 
*/
void SettingsIO::TestCreateConfigFile(const std::string& projectHomePath, const std::string& configFileFullPath)
{
	Directory::Pointer homeDir;
	File::Pointer file;
	std::string defaultProfile;
	std::string defaultInput;

	// Test if exists folder + create it if don't
	homeDir = Directory::New();
	homeDir->SetDirNameFullPath(projectHomePath);
	homeDir->Create();

	// Test if config file exists + create if don't
	file = File::New();
	file->SetFileNameFullPath(configFileFullPath);
	if(!file->Exists())
	{
		// Set the default contents for the config file
		defaultInput = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		defaultInput += "<!DOCTYPE GIMIAS_Configuration PUBLIC \"GIMIAS_Config_file\" \"config.dtd\">\n";
		defaultInput += "<GIMIAS_Configuration>\n";
		defaultInput += "  <variable name=\"startedOnce\">false</variable>\n";
		defaultInput += "  <profile>\n";
		defaultInput += "    <profileItem>" + defaultProfile + "</profileItem>\n";
		defaultInput += "  </profile>\n";
		defaultInput += "</GIMIAS_Configuration>";
		file->Create();
		file->Append(defaultInput);
	}

}

//!
void SettingsIO::ReadConfigFromFile(const std::string& configFileFullPath, ConfigVars& configVars)
{
	XML::DOMHandler::Pointer xmlHandler = XML::DOMHandler::New();
	xmlHandler->SetFileNameFullPath(configFileFullPath);
	xmlHandler->Parse();

	configVars.m_startedOnce = String2Bool( ReadTextVariable( xmlHandler, TAG_STARTED_ONCE ) );
	configVars.m_ImportConfiguration = String2Bool( ReadTextVariable( xmlHandler, TAG_STARTED_IMPORT_CONFIGURATION ) );
	ReadProfile( xmlHandler, configVars );
	configVars.m_lastOpenedPath = ReadTextVariable( xmlHandler, TAG_LAST_OPENED_PATH );
	configVars.m_lastOpenedDicomDirPath = ReadTextVariable( xmlHandler, TAG_LAST_OPENED_DICOMDIR_PATH );
	configVars.m_lastOpenedDicomFilePath = ReadTextVariable( xmlHandler, TAG_LAST_OPENED_DICOMFILE_PATH );
	configVars.m_lastSavePath = ReadTextVariable( xmlHandler, TAG_LAST_SAVE_PATH );
	configVars.m_pacsCalledAE =  ReadTextVariable( xmlHandler, TAG_PACS_CALLEDAE ) ;
	configVars.m_pacsCallingAE =  ReadTextVariable( xmlHandler, TAG_PACS_CALLINGAE ) ;
	configVars.m_Perspective =  ReadTextVariable( xmlHandler, TAG_PERSPECTIVE ) ;
	configVars.m_ActiveWorkflow =  ReadTextVariable( xmlHandler, TAG_ACTIVE_WORKFLOW ) ;
	ReadStringVector( xmlHandler, TAG_MRU_LIST, TAG_MRU_LIST_ITEM, configVars.m_MRUList );
	ReadStringVector( xmlHandler, TAG_PLUGINS_SCAN_FOLDERS_DEBUG, TAG_PLUGINS_SCAN_FOLDERS_ITEM, configVars.m_PluginsFoldersDebug );
	ReadStringVector( xmlHandler, TAG_PLUGINS_SCAN_FOLDERS_RELEASE, TAG_PLUGINS_SCAN_FOLDERS_ITEM, configVars.m_PluginsFoldersRelease );
	ReadPluginConfiguration( xmlHandler, configVars );
	std::string val = ReadTextVariable( xmlHandler, TAG_SHOW_REGISTRATION_FORM );
	if ( !val.empty() )
	{
		configVars.m_ShowRegistrationForm = String2Bool( val );
	}
	val = ReadTextVariable( xmlHandler, TAG_USER_REGISTERED );
	if ( !val.empty() )
	{
		configVars.m_UserRegistered = String2Bool( val );
	}
}

void SettingsIO::WriteConfigToFile(
						const std::string& configFileFullPath, 
						const std::string& dtdFileFullPath, 
						const std::string& projectName, 
						ConfigVars& configVars)
{
	xercesc::DOMDocument* document;
	XML::DOMHandler::Pointer xmlHandler;
	
	std::string qualifiedName = projectName + "_Configuration";
	std::string publicName = projectName + "_Config_file";
	document = XML::DOMHandler::CreateDocumentWithDTD(dtdFileFullPath, qualifiedName, publicName);

	if(document != NULL)
	{
		xmlHandler = XML::DOMHandler::New();
		xmlHandler->SetFileNameFullPath(configFileFullPath);

		WriteTextVariable( document, NULL, TAG_STARTED_ONCE, Bool2String( configVars.m_startedOnce ) );
		WriteTextVariable( document, NULL, TAG_STARTED_IMPORT_CONFIGURATION, Bool2String( configVars.m_ImportConfiguration ) );
		WriteTextVariable( document, NULL, TAG_LAST_OPENED_PATH, configVars.m_lastOpenedPath );
		WriteTextVariable( document, NULL, TAG_LAST_OPENED_DICOMDIR_PATH, configVars.m_lastOpenedDicomDirPath );
		WriteTextVariable( document, NULL, TAG_LAST_OPENED_DICOMFILE_PATH, configVars.m_lastOpenedDicomFilePath );
		WriteTextVariable( document, NULL, TAG_LAST_SAVE_PATH, configVars.m_lastSavePath );
		WriteTextVariable( document, NULL, TAG_PACS_CALLEDAE, configVars.m_pacsCalledAE );
		WriteTextVariable( document, NULL, TAG_PACS_CALLINGAE, configVars.m_pacsCallingAE );
		WriteTextVariable( document, NULL, TAG_PERSPECTIVE, configVars.m_Perspective );
		WriteTextVariable( document, NULL, TAG_ACTIVE_WORKFLOW, configVars.m_ActiveWorkflow );
		WriteProfile( document, configVars );
		WriteStringVector( document, TAG_MRU_LIST, TAG_MRU_LIST_ITEM, configVars.m_MRUList );
		WriteStringVector( document, TAG_PLUGINS_SCAN_FOLDERS_DEBUG, TAG_PLUGINS_SCAN_FOLDERS_ITEM, configVars.m_PluginsFoldersDebug );
		WriteStringVector( document, TAG_PLUGINS_SCAN_FOLDERS_RELEASE, TAG_PLUGINS_SCAN_FOLDERS_ITEM, configVars.m_PluginsFoldersRelease );
		WritePluginConfiguration( document, configVars );
		WriteTextVariable( document, NULL, TAG_SHOW_REGISTRATION_FORM, Bool2String( configVars.m_ShowRegistrationForm ) );
		WriteTextVariable( document, NULL, TAG_USER_REGISTERED, Bool2String( configVars.m_UserRegistered ) );

		// Serialize the document to the file
		xmlHandler->SetDOMDocument(document);
		xmlHandler->Serialize();
		
		// close document and release handles
		document->release();
	}
}


std::string Core::IO::SettingsIO::ReadTextVariable( 
						XML::DOMHandler::Pointer xmlHandler,
						const char* tagName )
{
	std::string nodeStr; 
	xercesc::DOMElement* elem = NULL;
	std::string text;

	// lastOpenedPath
	elem = xmlHandler->GetElementById( tagName );
	if(elem != NULL)
	{
		text = XML::XMLSubsystem::XMLChToString(elem->getTextContent());
	}

	return text;
}


void Core::IO::SettingsIO::WriteTextVariable( 
						xercesc::DOMDocument* document, 
						xercesc::DOMElement* parent,
						const char* strVariableName,
						std::string strValue )
{
	xercesc::DOMElement* elem = NULL;

	// Create variable element "variable"
	elem = CreateElement( document, TAG_VARIABLE );

	// Add attribute "name"
	SetNodeAttribute( document, elem, TAG_NAME, strVariableName );

	// Append child text node to elem
	elem->appendChild( CreateTextNode( document, strValue ) );

	// add it to the DOM tree
	if ( parent == NULL )
	{
		parent = document->getDocumentElement();
	}
	parent->appendChild(elem);
}

void SettingsIO::ReadProfile( 
			XML::DOMHandler::Pointer xmlHandler, 
			ConfigVars& configVars )
{
	// Clear the profile
	if(configVars.m_profile.IsNotNull())
	{
		configVars.m_profile->ClearProfile();
	}
	else
	{
		configVars.m_profile = Core::Profile::New();
	}

	std::vector<std::string> strVector;
	ReadStringVector( xmlHandler, TAG_PROFILE, TAG_PROFILE_ITEM, strVector );
	for ( unsigned i = 0 ; i < strVector.size() ; i++ )
	{
		configVars.m_profile->AddToProfile( strVector[ i ] );
	}
}

void Core::IO::SettingsIO::WriteProfile( 
							xercesc::DOMDocument* document, 
							const ConfigVars& configVars )
{
	Core::Profile::ProfileSet profiles;
	Core::Profile::ProfileSet::iterator it;
	std::vector<std::string> strVector;

	// profile element
	if(configVars.m_profile.IsNotNull())
	{
		profiles = configVars.m_profile->GetProfileAsStringSet();
		for(it = profiles.begin(); it != profiles.end(); ++it)
		{
			strVector.push_back( *it );
		}
	}

	WriteStringVector( document, TAG_PROFILE, TAG_PROFILE_ITEM, strVector );
}

void SettingsIO::ReadStringVector( 
			XML::DOMHandler::Pointer xmlHandler,
			const char* tagName,
			const char* subtagName,
			std::vector<std::string> &strVector )
{
	xercesc::DOMElement* elem = NULL;
	xercesc::DOMNodeList* elemList = NULL;
	xercesc::DOMNode* child = NULL;
	std::string text;

	// Check if element is present
	if ( !xmlHandler->FindElementByName( tagName ) )
	{
		return;
	}

	elemList = xmlHandler->GetElementsByTagName( tagName );
	if(elemList != NULL) 
	{
		// Get the first Profile element found
		child = elemList->item(0);
		strVector.clear();
	}
	if(child != NULL && child->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
	{
		// Get the  child profileItem elements
		elem = static_cast<xercesc::DOMElement*>(child);
		elemList = XML::DOMHandler::GetChildElementsByTagName(elem, subtagName);
		for(unsigned i = 0; i < elemList->getLength(); ++i)
		{
			text = XML::DOMHandler::GetTextOfANode(elemList->item(i));
			strVector.push_back( text );
		}
	}
}


void Core::IO::SettingsIO::WriteStringVector( 
						xercesc::DOMDocument* document, 
						const char* tagName,
						const char* subtagName,
						const std::vector<std::string> &strVector )
{
	xercesc::DOMElement* elem = NULL;
	xercesc::DOMElement* child = NULL;

	// Root node element
	elem = CreateElement( document, tagName );

	std::vector<std::string>::const_iterator it;
	for(it = strVector.begin(); it != strVector.end(); ++it)
	{
		// Element of the text node
		child = CreateElement( document, subtagName );
		
		// assign the text node to the element
		child->appendChild( CreateTextNode( document, (*it).c_str() ) );

		// append the element to the parent
		elem->appendChild(child);
	}

	// add the list of profileItems to the DOM tree
	document->getDocumentElement()->appendChild(elem);

}

Core::IO::SettingsIO::SettingsIO()
{
}

Core::IO::SettingsIO::~SettingsIO()
{
}


std::string Core::IO::SettingsIO::Bool2String( bool bValue )
{
	
	std::string strValue;
	if ( bValue )
	{
		strValue = TAG_TRUE;
	}
	else
	{
		strValue = TAG_FALSE;
	}
	
	return strValue;
}

bool Core::IO::SettingsIO::String2Bool( std::string bValue )
{
	if ( bValue.compare( TAG_TRUE ) == 0 )
	{
		return true;
	}
	else if ( bValue.compare( TAG_FALSE ) == 0 )
	{
		return false;
	}

	return false;
}

void Core::IO::SettingsIO::ReadPluginConfiguration( 
	XML::DOMHandler::Pointer xmlHandler, 
	ConfigVars& configVars )
{
	xercesc::DOMNodeList* elemListPluginConfiguration = NULL;
	xercesc::DOMNodeList* elemListPlugins = NULL;
	xercesc::DOMNodeList* elemListPluginProperties = NULL;
	xercesc::DOMNode* child = NULL;
	xercesc::DOMElement* pluginElement = NULL;
	xercesc::DOMElement* pluginConfigurationElement = NULL;
	std::string pluginName;


	// Start the read of the profile
	elemListPluginConfiguration = xmlHandler->GetElementsByTagName( TAG_PLUGIN_CONFIG );
	if(elemListPluginConfiguration != NULL) 
	{
		child = elemListPluginConfiguration->item( 0 );
	}

	if(child != NULL && child->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
	{
		pluginConfigurationElement = static_cast<xercesc::DOMElement*>(child);
		elemListPlugins = XML::DOMHandler::GetChildElementsByTagName(
			pluginConfigurationElement, TAG_PLUGIN );

		unsigned iNumPlugins = elemListPlugins->getLength();
		for(unsigned i = 0; i < iNumPlugins; ++i)
		{
			child = elemListPlugins->item(i);
			if ( child->getNodeType() == xercesc::DOMNode::ELEMENT_NODE )
			{
				ConfigVarsPlugin pluginConfig;

				pluginElement = static_cast<xercesc::DOMElement*>(child);

				elemListPluginProperties = 
					XML::DOMHandler::GetChildElementsByTagName(pluginElement, TAG_PLUGIN_NAME);
				pluginName = XML::DOMHandler::GetTextOfANode( elemListPluginProperties->item(0) );

				// Backwards compatibility
				elemListPluginProperties = 
					XML::DOMHandler::GetChildElementsByTagName(pluginElement, TAG_PLUGIN_LAYOUT);
				if ( elemListPluginProperties->getLength( ) )
				{
					std::string layoutConfiguration 
						= XML::DOMHandler::GetTextOfANode(elemListPluginProperties->item(0));
					pluginConfig.m_Properties[ "layout" ] = layoutConfiguration;
				}

				// Iterate over all child variables
				elemListPluginProperties = 
					XML::DOMHandler::GetChildElementsByTagName(pluginElement, TAG_VARIABLE);
				for(unsigned i = 0; i < elemListPluginProperties->getLength(); ++i)
				{
					xercesc::DOMNode* variableNode = elemListPluginProperties->item(i);

					xercesc::DOMNamedNodeMap* attribs = variableNode->getAttributes( );
					std::string name = XML::DOMHandler::GetTextOfANode( attribs->item( 0 ) );
					std::string value = XML::DOMHandler::GetTextOfANode( variableNode );
					pluginConfig.m_Properties[ name ] = value;
				}

				// Add the config
				Core::IO::ConfigVars::PluginMapPair pair( pluginName, pluginConfig );
				configVars.m_MapPluginConfiguration.insert( pair );
			}
		}

	}

}

void Core::IO::SettingsIO::WritePluginConfiguration( 
	xercesc::DOMDocument* document, 
	ConfigVars& configVars )
{
	xercesc::DOMElement* elem = NULL;
	xercesc::DOMElement* childPlugin = NULL;
	xercesc::DOMElement* childProperty = NULL;

	// Root node element
	elem = CreateElement( document, TAG_PLUGIN_CONFIG );

	Core::IO::ConfigVars::PluginMapType::iterator it;
	it = configVars.m_MapPluginConfiguration.begin();
	while ( it != configVars.m_MapPluginConfiguration.end() )
	{
		// Element of the plugin
		childPlugin = CreateElement( document, TAG_PLUGIN );

		// append the element to the parent
		elem->appendChild(childPlugin);

		// Element of the text TAG_PLUGIN_NAME
		childProperty = CreateElement( document, TAG_PLUGIN_NAME );

		// assign the text node to the element
		childProperty->appendChild( CreateTextNode( document, it->first ) );

		// append the element to the parent
		childPlugin->appendChild(childProperty);
		
		std::map<std::string,std::string>::iterator itProp;
		for ( itProp = it->second.m_Properties.begin() ; itProp != it->second.m_Properties.end() ; itProp++ )
		{
			// Element variable
			WriteTextVariable( document, childPlugin, itProp->first.c_str(), itProp->second.c_str() );
		}

		it++;
	}

	document->getDocumentElement()->appendChild(elem);
}

xercesc::DOMElement* Core::IO::SettingsIO::CreateElement( 
	xercesc::DOMDocument* document,
	const std::string &strName )
{
	XMLCh* xmlStr;
	xercesc::DOMElement* elem = NULL;

	xmlStr = xercesc::XMLString::transcode( strName.c_str() );
	elem = document->createElement(xmlStr);
	xercesc::XMLString::release(&xmlStr);

	return elem;
}

xercesc::DOMText* Core::IO::SettingsIO::CreateTextNode( 
	xercesc::DOMDocument* document,
	const std::string &strName )
{
	XMLCh* xmlStr;
	xercesc::DOMText* text = NULL;

	// set the text node
	xmlStr = xercesc::XMLString::transcode( strName.c_str() );
	text = document->createTextNode( xmlStr );
	xercesc::XMLString::release( &xmlStr );
	
	return text;
}

void Core::IO::SettingsIO::SetNodeAttribute( 
	xercesc::DOMDocument* document, 
	xercesc::DOMElement* elem, 
	const std::string &strAttribute,
	const std::string &strValue )
{
	XMLCh* xmlStrAttribute;
	XMLCh* xmlStrValue;

	xmlStrAttribute = xercesc::XMLString::transcode( strAttribute.c_str() );
	xmlStrValue = xercesc::XMLString::transcode( strValue.c_str() );

	elem->setAttribute( xmlStrAttribute, xmlStrValue);

	xercesc::XMLString::release( &xmlStrValue );
	xercesc::XMLString::release( &xmlStrAttribute );
}

