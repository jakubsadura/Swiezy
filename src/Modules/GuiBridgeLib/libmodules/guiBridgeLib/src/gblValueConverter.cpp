/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblValueConverter.h"
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/replace.hpp"
#include <cmath>

//! Retains maximal precision.
std::string gbl::ValueConverter::NumberToText(double _x, const int _maxNrOfDecimals)
{
	std::stringstream ss;
	ss.precision(std::numeric_limits<double>::digits10 + 2);
	ss << std::fixed  << _x;
	
	//used to find the first significant digit
	std::stringstream scientific_ss;
	scientific_ss.precision(std::numeric_limits<double>::digits10 + 2);
	scientific_ss << std::scientific <<_x;

	std::string result = ss.str();
	size_t n = result.find('.');
	// find the first significant digit
	bool found = false;
	int lastSignificantZero = n + 1;

	for( int m = n+1; m < result.length() && !found; m++)
	{
		if( result[m] == '0' )
		{
			lastSignificantZero = m;
		}
		else
			found = true;
	}

	const int nrDecimals = result.length() - n - 1;
	if( n != std::string::npos && /*nrDecimals*/ lastSignificantZero > _maxNrOfDecimals && found )
	{
		//result = result.substr(0, n + 1 + _maxNrOfDecimals);
		result = result.substr(0, n + 1 + lastSignificantZero);
	}	
	else
	{
		if ( _maxNrOfDecimals == 0 )
		{
			result = result.substr(0, n );
		}
		else
		{
			result = result.substr(0, n + 1 +_maxNrOfDecimals);
		}
	}

	return result;
}

//! Throws if x cannot be converted to a number.
double gbl::ValueConverter::TextToNumber(const std::string& x)
{
	//if( x.length() >= 1 && (x[0] == '.' || x[x.length()-1] == '.' ) )
	//{
	//	std::string message = x + " does not have the valid format of a number.";
	//	throw gbl::ValueConverterException(message.c_str());
	//}
	return boost::lexical_cast<double>(x); // also checks that x is indeed a number.
}

//! Returns "true" or "false".
std::string gbl::ValueConverter::FlagToText(bool x)
{
	return x ? "true" : "false";
}

//! Returns true if the lower case version of x is "true", "yes", "on" or "1".
//! Returns false if the lower case version of x is "false", "no", "off", "0" or "".
//! Throws ValueConverterException in all other cases.
bool gbl::ValueConverter::TextToFlag(const std::string& x)
{
	std::string xLowerCase = x;
	boost::to_lower(xLowerCase);
	bool result(false);
	if( xLowerCase == "true" || xLowerCase == "yes" || xLowerCase == "on" || xLowerCase == "1" )
	{
		result = true;
	}
	else if( xLowerCase == "false" || xLowerCase == "no" || xLowerCase == "off" || xLowerCase == "0" || xLowerCase == "" )
	{
		result = false;
	}
	else
	{
		std::string message = "Cannot convert " + x + " to a flag value";
		throw gbl::ValueConverterException(message.c_str());
	}

	return result;
}

double gbl::RoundToXDecimals( double _x, int _nrDecimals )
{
	const double factor = pow(10.0f, _nrDecimals);
	double tmp = std::floor(_x * factor + 0.5);
	return tmp / factor;
}
