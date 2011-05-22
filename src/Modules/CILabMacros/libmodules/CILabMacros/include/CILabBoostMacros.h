/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CILABBOOSTMACROS_H
#define CILABBOOSTMACROS_H

#include "boost/format.hpp"

/**
This macro makes it easier to create a formatted string.
E.g.

int i = 3, j = 5;
cilabBoostFormatMacro("Indexes are %d and %d", i % j, message);
std::cout << message;

\author Maarten Nieber
\date 05 mar 2008
*/

#define cilabBoostFormatMacro(formatString, formatArgs, stringName) \
	std::string stringName = (boost::format(formatString) % formatArgs).str();

#define cistibBoostPointerMacro(className) \
	typedef boost::shared_ptr<className> Pointer; \
	static Pointer New() {return Pointer(new className);}; 

/*
This macro serializes an itk pointer to/from an archive.
\example: 

cilabSerializeItkPointerMacro(ar, evoGradientParameters, gradientParameters)

will expand into

evoGradientParameters* gradientParameters = this->gradientParameters;
ar & BOOST_SERIALIZATION_NVP(gradientParameters);
if(this->gradientParameters != gradientParameters) // if gradientParameters contains new instantiation...
{
this->gradientParameters = gradientParameters;
gradientParameters->Delete(); // correct reference count
}

\author Maarten Nieber
\date 30 ene 2009
*/

#define cilabSerializeItkPointerMacro(ARG_ARCHIVE, ARG_TYPE, ARG_MEMBER) \
	ARG_TYPE* ARG_MEMBER = this->ARG_MEMBER.GetPointer(); \
	ARG_ARCHIVE & BOOST_SERIALIZATION_NVP(ARG_MEMBER); \
	if( this->ARG_MEMBER.GetPointer() != ARG_MEMBER ) \
	{ \
	this->ARG_MEMBER = ARG_MEMBER; \
	ARG_MEMBER->Delete(); \
	}

#endif //CILABBOOSTMACROS_H
