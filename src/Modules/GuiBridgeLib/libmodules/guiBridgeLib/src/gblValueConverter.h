/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLVALUECONVERTER_H
#define GBLVALUECONVERTER_H

#include "gblException.h"
#include <typeinfo>
#include <string>
#include <limits>

CILAB_BEGIN_NAMESPACE(gbl)

/**
This class is used in ValueConverter when a value cannot be converted.
\author Maarten Nieber
\date 1 Sep 007
*/
gblDeclareExceptionMacro(ValueConverterException, std::bad_cast)

/**
This class provides a common convertion mechanism for all ValueProxy objects.
For example, ValueConverter::TextToFlag("Yes") will return true, but ValueConverter::TextToFlag("No") will return false,
and ValueConverter::TextToFlag("YesIndeed") will throw an exception.
By using this class in all ValueProxy classes, they will convert values in the same way and therefore it will be easier to replace one ValueProxy instance with another.

\author Maarten Nieber
\date 1 Sep 007
*/

class GUIBRIDGELIB_EXPORT ValueConverter
{
public:
	//!
	static std::string NumberToText(double _x, const int _maxNrOfDecimals = std::numeric_limits<double>::digits10 + 2);
	//!
	static double TextToNumber(const std::string& x);
	//!
	static std::string FlagToText(bool x);
	//!
	static bool TextToFlag(const std::string& x);
};

//! Returns _x rounded to a maximum of _nrDecimals
double RoundToXDecimals(double _x, int _nrDecimals);

CILAB_END_NAMESPACE(gbl)

#endif //GBLVALUECONVERTER_H
