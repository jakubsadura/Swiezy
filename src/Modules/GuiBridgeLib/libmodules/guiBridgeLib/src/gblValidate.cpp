/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <math.h>
#include "gblValidate.h"

using namespace boost;

gbl::Validator::Validator( )
{
}

boost::tuple<bool, double> gbl::Validator::GetRealNumber(const std::string& _value)
{
	bool isNumber = true;
	double number(0);

	try
	{
		number = gbl::ValueConverter::TextToNumber(_value);
	}
	catch (std::exception& )
	{
		isNumber = false;
	}
	return boost::make_tuple(isNumber, number);
}

boost::tuple<bool, int> gbl::Validator::GetNonNegativeInteger( const std::string& _value )
{
	boost::tuple<bool, double> result = gbl::Validator::GetRealNumber(_value);
	bool nonNegInt = get<0>(result) && (get<1>(result) >= 0) && floor( get<1>(result) ) == get<1>(result);
	return make_tuple( nonNegInt, int(get<1>(result)) );
}

boost::tuple<bool, int> gbl::Validator::GetPositiveInteger( const std::string& _value )
{
	boost::tuple<bool, double> result = gbl::Validator::GetRealNumber(_value);
	bool posInt = get<0>(result) && (get<1>(result) >= 1.0) && floor( get<1>(result) ) == get<1>(result);
	return make_tuple( posInt, int(get<1>(result)) );
}

boost::tuple<bool, double> gbl::Validator::GetNonNegativeRealNumber( const std::string& _value )
{
	boost::tuple<bool, double> result = gbl::Validator::GetRealNumber(_value);
	get<0>(result) = get<0>(result) && get<1>(result) >= 0.0;
	return result;
}

boost::tuple<bool, double> gbl::Validator::GetPositiveRealNumber( const std::string& _value )
{
	boost::tuple<bool, double> result = gbl::Validator::GetRealNumber(_value);
	get<0>(result) = get<0>(result) && get<1>(result) > 0.0;
	return result;
}
