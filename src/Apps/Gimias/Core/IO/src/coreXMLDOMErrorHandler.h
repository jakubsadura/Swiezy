/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDOMErrorHandler_H
#define coreDOMErrorHandler_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include <string>
#include <xercesc/dom/DOMErrorHandler.hpp>


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

\ingroup gmIO
\author Juan Antonio Moya
\date 14 Apr 2007
*/
class GMIO_EXPORT DOMErrorHandler 
	: public Core::SmartPointerObject, public xercesc::DOMErrorHandler
{
public:
	coreDeclareSmartPointerClassMacro(DOMErrorHandler, Core::SmartPointerObject);
	
	virtual bool handleError(const xercesc::DOMError& domError);

protected:
	DOMErrorHandler(void);
	~DOMErrorHandler(void);

private:
	coreDeclareNoCopyConstructors(DOMErrorHandler);
};
}
}
}

#endif // coreDOMErrorHandler_H



