/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreXMLSubsystem.h"
#include "coreException.h"
#include <xercesc/util/PlatformUtils.hpp>

using namespace Core::IO::XML;

bool XMLSubsystem::isInitialized = false;

void XMLSubsystem::Initialize(void)
{
	try
	{
		if(!isInitialized)
		{
			xercesc::XMLPlatformUtils::Initialize();
			isInitialized = true;
		}
	}
	catch(const xercesc::XMLException& exc)
	{
		std::string message = xercesc::XMLString::transcode(exc.getMessage());
		Core::Exceptions::InitializingXMLSubsystemException e("XMLSubsystem::Initialize");
		e.Append("\nDescription: ");
		e.Append(message.c_str());
		throw e;
	}
}

void XMLSubsystem::Terminate(void)
{
    if(isInitialized) {
        xercesc::XMLPlatformUtils::Terminate();
        isInitialized = false;
    }
}

std::string XMLSubsystem::XMLChToString(const XMLCh* str)
{
	char *outStrC = xercesc::XMLString::transcode(str);
	std::string outStr(outStrC);
	xercesc::XMLString::release(&outStrC);
	return outStr;
}

const XMLCh* XMLSubsystem::StringToXMLCh(std::string& str)
{    
	return xercesc::XMLString::transcode(str.c_str());
}
