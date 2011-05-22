/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreXMLDOMHandler.h"
#include "coreException.h"
#include "coreXMLSubsystem.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

using namespace Core::IO::XML;


DOMHandler::DOMHandler(void) :
//xmlDOMParser(NULL),
xmlDOMWriter(NULL),
xmlDocument(NULL)
{
	XMLCh* str;
	xercesc::DOMImplementationLS* domImpl;

	XMLSubsystem::Initialize();
	
	this->xmlDocument = NULL;

	// Instantiate a factory object for the XML DOM Implementation
	str = xercesc::XMLString::transcode("LS");
	domImpl = static_cast<xercesc::DOMImplementationLS*>(xercesc::DOMImplementationRegistry::getDOMImplementation(str));
	xercesc::XMLString::release(&str);
	
	// Create the parser for reading as a DOMBuilder
	this->xmlDOMParser = domImpl->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	//this->xmlDOMParser = ((xercesc::DOMImplementationLS*)domImpl)->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	// Set the parser features
	if(this->xmlDOMParser->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMValidate, true))
	{   // Report all validation errors.
		this->xmlDOMParser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMValidate, true);
	}
	if(this->xmlDOMParser->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMNamespaces, true))
	{	// Perform Namespace processing
		this->xmlDOMParser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMNamespaces, true);
	}
	if(this->xmlDOMParser->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true))
	{	// Let the validation process do its datatype normalization that is defined in the used schema language.
		this->xmlDOMParser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
	}

	// Create the parser for writing as a DOMWriter
	this->xmlDOMWriter = domImpl->createLSSerializer();

	// Set the writer features
	if(this->xmlDOMWriter->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
	{   // Set to format the output by adding whitespace to produce a pretty-printed, indented, human-readable form
		this->xmlDOMWriter->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}


	this->errorHandler = Core::IO::XML::DOMErrorHandler::New();
	//this->xmlDOMParser->setErrorHandler(this->errorHandler.GetPointer());
	//this->xmlDOMWriter->setErrorHandler(this->errorHandler.GetPointer());

	this->xmlFile = Core::IO::File::New();
}

DOMHandler::~DOMHandler(void)
{
    if(this->xmlDOMWriter != NULL)
    {
        this->xmlDOMWriter->release();
    }
    this->xmlDOMParser->release();
}


xercesc::DOMDocument* DOMHandler::CreateDocument(void)
{
	XMLCh* str;
	xercesc::DOMImplementation* domImpl;
	xercesc::DOMDocument* document = NULL;
	XMLSubsystem::Initialize();

	// Instantiate a factory object for the XML DOM Implementation
	str = xercesc::XMLString::transcode("Range");
	domImpl = xercesc::DOMImplementationRegistry::getDOMImplementation(str);
	xercesc::XMLString::release(&str);

	// Create the document entity
	str = xercesc::XMLString::transcode("root");
	document = domImpl->createDocument(NULL, str, NULL);
	xercesc::XMLString::release(&str);

	/*str = xercesc::XMLString::transcode("UTF-8");
	document->setEncoding(str);
	xercesc::XMLString::release(&str);*/

	str = xercesc::XMLString::transcode("1.0");
	document->setXmlVersion(str);
	xercesc::XMLString::release(&str);

	document->setXmlStandalone(false);

	return document;
}


xercesc::DOMDocument* DOMHandler::CreateDocumentWithDTD(const std::string& fullPathToDTDFile, const std::string& dtdQualifiedName, const std::string& publicIdName)
{
	
	XMLCh* str;
	XMLCh* qualifiedName;
	XMLCh* publicId;
	XMLCh* systemId;
	xercesc::DOMImplementation* domImpl;
	xercesc::DOMDocument* document = NULL;
	xercesc::DOMDocumentType* docType = NULL;
	XMLSubsystem::Initialize();

	// Instantiate a factory object for the XML DOM Implementation
	str = xercesc::XMLString::transcode("Range");
	domImpl = xercesc::DOMImplementationRegistry::getDOMImplementation(str);
	xercesc::XMLString::release(&str);

	// Create the DocumentType (DTD)
	qualifiedName = xercesc::XMLString::transcode(dtdQualifiedName.c_str());
	publicId = xercesc::XMLString::transcode(publicIdName.c_str());
	systemId = xercesc::XMLString::transcode(fullPathToDTDFile.c_str());
	docType = domImpl->createDocumentType(qualifiedName, publicId, systemId);
	
	// Create the document entity
	document = domImpl->createDocument(NULL, qualifiedName, docType);
	xercesc::XMLString::release(&str);

	// set the document properties
	/*str = xercesc::XMLString::transcode("UTF-8");
	document->setEncoding(str);
	xercesc::XMLString::release(&str);*/

	str = xercesc::XMLString::transcode("1.0");
	document->setXmlVersion(str);
	xercesc::XMLString::release(&str);

	xercesc::XMLString::release(&qualifiedName);
	xercesc::XMLString::release(&publicId);
	xercesc::XMLString::release(&systemId);

	return document;
}


void DOMHandler::Parse(void)
{
	XMLSubsystem::Initialize();

	if(this->xmlFile->Exists() && this->xmlDOMParser != NULL)
	{
		// Release any previous loaded document
		if(this->xmlDocument != NULL)
		{
			this->xmlDocument->release();
			this->xmlDocument = NULL;
		}
		
		try
		{
			this->xmlDocument = this->xmlDOMParser->parseURI(this->xmlFile->GetFileNameFullPath().c_str());
		}
		catch(const xercesc::XMLException& e)
		{
			std::string message = XMLSubsystem::XMLChToString(e.getMessage());
			Core::Exceptions::ParsingXMLException err("DOMHandler::Parse");
			err.Append("\nDescription: ");
			err.Append(message.c_str());
			throw err;
		}
		catch(const xercesc::DOMException& e)
		{
			std::string message = XMLSubsystem::XMLChToString(e.getMessage());
			Core::Exceptions::ProcessingDOMException err("DOMHandler::Parse");
			err.Append("\nDescription ");
			err.Append(message.c_str());
			throw err;
		}
	}
	else
	{
		Core::Exceptions::FileNotFoundException e("DOMHandler::Parse");
		e.Append("\nFile was: ");
		e.Append(this->xmlFile->GetFileNameFullPath().c_str());
		throw e;
	}
}


void DOMHandler::Serialize(void)
{
	XMLSubsystem::Initialize();
	XMLCh* txtDocument;
	std::string strDocument;

	if(this->xmlDOMWriter!= NULL && this->xmlDocument != NULL)
	{
		try
		{
			// Trick: We are not using writeToFile() because it leaves the file locked
			//txtDocument = this->xmlDOMWriter->writeToString(*this->xmlDocument);
			txtDocument = this->xmlDOMWriter->writeToString(xmlDocument);
			// Transform it into UTF-8
			xercesc::LocalFileFormatTarget outFile(this->xmlFile->GetFileNameFullPath().c_str());
			xercesc::XMLFormatter xmlFormat("UTF-16", "1.0", &outFile);
			xmlFormat << txtDocument;

			xercesc::XMLString::release(&txtDocument);
			this->xmlDOMWriter->release();
			this->xmlDOMWriter = NULL;
		}
		catch(const xercesc::XMLException& e)
		{
			std::string message = XMLSubsystem::XMLChToString(e.getMessage());
			Core::Exceptions::ParsingXMLException err("DOMHandler::Serialize");
			err.Append("\nDescription: ");
			err.Append(message.c_str());
			throw err;
		}
		catch(const xercesc::DOMException& e)
		{
			std::string message = XMLSubsystem::XMLChToString(e.getMessage());
			Core::Exceptions::ProcessingDOMException err("DOMHandler::Serialize");
			err.Append("\nDescription: ");
			err.Append(message.c_str());
			throw err;
		}
	}
	else
	{
		Core::Exceptions::FileNotFoundException e("DOMHandler::Parse");
		e.Append("\nFile was: ");
		e.Append(this->xmlFile->GetFileNameFullPath().c_str());
		throw e;
	}
}


void DOMHandler::SetFileNameFullPath(const std::string& fullPath)
{
	this->xmlFile->SetFileNameFullPath(fullPath);
}

std::string DOMHandler::GetFileNameFullPath(void)
{
	return this->xmlFile->GetFileNameFullPath();
}


xercesc::DOMElement* DOMHandler::GetElementById(const std::string& elementId)
{
	XMLSubsystem::Initialize();
	XMLCh* id;
	xercesc::DOMElement* element = NULL;
	if(this->xmlDocument != NULL)
	{
		id = xercesc::XMLString::transcode(elementId.c_str());
		element = this->xmlDocument->getElementById(id);
		xercesc::XMLString::release(&id);
	}
	return element;
}


xercesc::DOMElement* DOMHandler::GetRootElement(void)
{
	xercesc::DOMElement* element = NULL;
	if(this->xmlDocument != NULL)
	{
		element = this->xmlDocument->getDocumentElement();
	}
	return element;
}


xercesc::DOMNodeList* DOMHandler::GetElementsByTagName(const std::string& tagName)
{
	XMLCh* tagStr;
	xercesc::DOMNodeList* list = NULL;
	XMLSubsystem::Initialize();
	if(this->xmlDocument != NULL)
	{
		tagStr = xercesc::XMLString::transcode(tagName.c_str());
		list = this->xmlDocument->getElementsByTagName(tagStr);
		xercesc::XMLString::release(&tagStr);
	}
	return list;
}

xercesc::DOMDocument* DOMHandler::GetDOMDocument(void)
{
	return this->xmlDocument;
}

xercesc::DOMNodeList* DOMHandler::GetChildElementsByTagName(xercesc::DOMElement* parent, const std::string& childTagName)
{
	XMLCh* tagStr;
	xercesc::DOMNodeList* list = NULL;
	XMLSubsystem::Initialize();
	if(parent != NULL)
	{
		tagStr = xercesc::XMLString::transcode(childTagName.c_str());
		list = parent->getElementsByTagName(tagStr);
		xercesc::XMLString::release(&tagStr);
	}
	return list;
}

std::string DOMHandler::GetTextOfANode(xercesc::DOMNode* parent)
{
	std::string text = "";
	xercesc::DOMNode* textNode;
	XMLSubsystem::Initialize();
	if(parent != NULL)
	{
		textNode = parent->getFirstChild();
		if(textNode != NULL && textNode->getNodeType() == xercesc::DOMNode::TEXT_NODE)
		{
			text = XMLSubsystem::XMLChToString(textNode->getNodeValue());
		}
	}
	return text;
}

void DOMHandler::SetDOMDocument(xercesc::DOMDocument* document)
{
	// Release any previous loaded document
	if(this->xmlDocument != NULL)
	{
		this->xmlDocument->release();
	}
	this->xmlDocument = document;
}


bool Core::IO::XML::DOMHandler::FindElementByName( const std::string &inputName )
{
	bool found = false;
	xercesc::DOMElement* elementRoot = GetRootElement( );
	xercesc::DOMNodeList*      children = elementRoot->getChildNodes();
	const  XMLSize_t nodeCount = children->getLength();
	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		xercesc::DOMNode* currentNode = children->item(xx);

		if ( currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE )
		{
			xercesc::DOMNamedNodeMap *AttributesList = currentNode->getAttributes();
			char *name = xercesc::XMLString::transcode(currentNode->getNodeName());
			if ( strcmp( name, inputName.c_str() ) == 0 )
			{
				found = true;
			}
			xercesc::XMLString::release( &name );

			if ( AttributesList )
			{
				for(int j = 0; j < AttributesList->getLength(); j++) 
				{
					char *name = xercesc::XMLString::transcode(AttributesList->item(j)->getNodeName());
					char *value = xercesc::XMLString::transcode(AttributesList->item(j)->getNodeValue());
					if ( strcmp( name, "name" ) == 0 && strcmp(value,inputName.c_str()) == 0 )
					{
						found = true;
					}

					xercesc::XMLString::release( &name );
					xercesc::XMLString::release( &value );
				}
			}

		}
	}

	return found;
}
