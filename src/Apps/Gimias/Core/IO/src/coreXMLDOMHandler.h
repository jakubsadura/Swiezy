/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDOMHandler_H
#define coreDOMHandler_H

#include "gmIOWin32Header.h"
#include "coreFile.h"
#include "coreXMLDOMErrorHandler.h"
#include "coreObject.h"
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>


namespace Core
{
namespace IO
{
namespace XML
{
/**
\brief The DOMHandler is a wrapper class of the standard DOM XML Parser

It allows to easy read and write an xml file for storing configurations 
or simple data.
It is responsible of initializing a DOMBuilder when reading from an xml 
file, and initializing a DOMWriter when serializing the DOMDocument to 
the file, so the user has not to take care of Document handling or IO 
and focus on the XML tree (DOMDocument).

The DOMHandler is prepared for auto-validating XML documents through 
their DTD. If you provide a DTD to the parsed xml document, it will try 
validate it and prompt any validation error found to the application.

\ingroup gmIO
\author Juan Antonio Moya
\date 11 Apr 2007
*/
class GMIO_EXPORT DOMHandler : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(DOMHandler, Core::SmartPointerObject);
			
	void SetFileNameFullPath(const std::string& fullPath);
	std::string GetFileNameFullPath(void);

	void Parse(void);
	void Serialize(void);

	void SetDOMDocument(xercesc::DOMDocument* document);
	xercesc::DOMDocument* GetDOMDocument(void);
	xercesc::DOMElement* GetRootElement(void);
	xercesc::DOMElement* GetElementById(const std::string& elementId);
	xercesc::DOMNodeList* GetElementsByTagName(const std::string& tagName);
	//!
	bool FindElementByName( const std::string &name );

	static xercesc::DOMNodeList* GetChildElementsByTagName(
		xercesc::DOMElement* parent, 
		const std::string& childTagName);
	static std::string GetTextOfANode(xercesc::DOMNode* parent);
	static xercesc::DOMDocument* CreateDocument(void);
	static xercesc::DOMDocument* CreateDocumentWithDTD(
		const std::string& fullPathToDTDFile, 
		const std::string& dtdQualifiedName, 
		const std::string& publicIdName);

protected:
	DOMHandler(void);
	~DOMHandler(void);

	Core::IO::File::Pointer xmlFile;
	xercesc::DOMLSParser* xmlDOMParser;
	xercesc::DOMLSSerializer* xmlDOMWriter; //xercesc/dom/DOMLSSerializer.hpp
	xercesc::DOMDocument* xmlDocument;
	Core::IO::XML::DOMErrorHandler::Pointer errorHandler;

private:
	coreDeclareNoCopyConstructors(DOMHandler);

};

}
}
}

#endif // coreDOMHandler_H
