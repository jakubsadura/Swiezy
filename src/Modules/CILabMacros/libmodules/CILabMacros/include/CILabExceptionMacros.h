/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CILABEXCEPTIONMACROS_H
#define CILABEXCEPTIONMACROS_H

#include <exception>
#include <string>

/**
This macro defines an exception class that inherits from Y (typically an std::exception). Its constructor takes 
a raw string. Its what() function returns this raw string.

\author Maarten Nieber
\date 1 Sep 2007
*/

#define cilabDeclareExceptionMacro(ARG_CLASS, ARG_BASE) \
class ARG_CLASS : public ARG_BASE \
{ \
public: \
	ARG_CLASS(const char* _what) : whatString(_what) \
	{ \
	}; \
	~ARG_CLASS() throw() \
		{ \
		}; \
		const char* what() const throw() \
	{ \
	return this->whatString.c_str(); \
	}; \
private: \
	std::string whatString; \
};

/**
This macro is similar to cilabDeclareExceptionMacro, but it also accepts a dll export argument.

\author Maarten Nieber
\date 1 Sep 2007
*/

#define cilabDeclareDllExportExceptionMacro(ARG_CLASS, ARG_BASE, ARG_EXPORT) \
class ARG_EXPORT ARG_CLASS : public ARG_BASE \
{ \
public: \
	ARG_CLASS(const char* _what) : whatString(_what) \
	{ \
	}; \
	~ARG_CLASS() throw() \
		{ \
		}; \
		const char* what() throw() \
	{ \
	return this->whatString.c_str(); \
	}; \
private: \
	std::string whatString; \
};

#endif //CILABEXCEPTIONMACROS_H
