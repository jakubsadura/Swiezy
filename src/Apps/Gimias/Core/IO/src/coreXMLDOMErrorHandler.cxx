/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreXMLDOMErrorHandler.h"
#include "coreXMLSubsystem.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNode.hpp>

using namespace Core::IO::XML;


DOMErrorHandler::DOMErrorHandler(void)
{
	
}

DOMErrorHandler::~DOMErrorHandler(void)
{

}

bool DOMErrorHandler::handleError(const xercesc::DOMError& domError)
{
	XMLSubsystem::Initialize();

	const XMLCh* str = domError.getMessage();
	std::string message = xercesc::XMLString::transcode(str);

	xercesc::DOMLocator* locator = domError.getLocation();
	int line = locator->getLineNumber();
	int col = locator->getColumnNumber();
	xercesc::DOMNode* node = locator->getRelatedNode();
	const XMLCh* nodeName; 
	if(node != NULL) nodeName = node->getNodeName();
	std::string nodeNameStr = xercesc::XMLString::transcode(nodeName);
	const XMLCh* uri = locator->getURI();
	std::string uriStr = xercesc::XMLString::transcode(uri);

	// Build the error string
	std::cerr << "Error found when parsing \"" << uriStr << "\" at line (" << line << "," << col << ")";
	if(node != NULL) std::cerr << " - In node \"" << nodeNameStr << "\"" << std::endl; 
	std::cerr << "  - " << message << std::endl;

	// Return true means continue parsing after the error
	return true;
}

