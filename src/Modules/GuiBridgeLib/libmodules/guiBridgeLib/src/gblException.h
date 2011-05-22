/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLEXCEPTION_H
#define GBLEXCEPTION_H

#include <exception>
#include <string>
#include "GuiBridgeLibWin32Header.h"
#include "CILabNamespaceMacros.h"

/**
This macro defines an exception class that inherits from Y (typically an std::exception). Its constructor takes 
a raw string. Its what() function returns this raw string.

\author Maarten Nieber
\date 1 Sep 2007
*/

#define gblDeclareExceptionMacro(X, Y) \
class GUIBRIDGELIB_EXPORT X : public Y \
{ \
public: \
	X(const char* _what) : whatString(_what) \
	{ \
	}; \
        ~X() throw() \
        { \
        }; \
	virtual const char* what() const throw ( )\
	{ \
	return this->whatString.c_str(); \
	}; \
private: \
	std::string whatString; \
};

CILAB_BEGIN_NAMESPACE(gbl)

gblDeclareExceptionMacro(Exception, std::exception)

CILAB_END_NAMESPACE(gbl)

#endif //GBLEXCEPTION_H
