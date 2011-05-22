/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreXMLSubsystem_H
#define coreXMLSubsystem_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include <xercesc/util/XMLString.hpp>
#include <string>

namespace Core
{
namespace IO
{
namespace XML
{
/**
\brief The XMLSubsystem is a wrapper class of the standard XMLPlatformUtils

It gathers some tools for XML character encoding, initializing the 
XML processors and so. The Kernel has to initialize the XML processor 
before any other call to an XML routine or class, and terminate it 
while program exit, so as to release pending XML documents in memmory or 
orphaned nodes.

The XML Subsystem has its garbage collector, that releases everything on 
call to Terminate()

\ingroup gmIO
\author Juan Antonio Moya
\date 08 Apr 2007
*/
class GMIO_EXPORT XMLSubsystem
{
public:
	static void Initialize(void);
	static void Terminate(void);
	static std::string XMLChToString(const XMLCh* str);
	static const XMLCh* StringToXMLCh(std::string& str);
	
	static bool IsInitialized(void);

private:
	static bool isInitialized;
};
}
}
}

#endif // coreXMLSubsystem_H
