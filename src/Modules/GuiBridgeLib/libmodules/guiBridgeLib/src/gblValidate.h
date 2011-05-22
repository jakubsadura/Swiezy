/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLVALIDATE_H
#define GBLVALIDATE_H

#include "gblBridge.h"
#include "CILabNamespaceMacros.h"
#include "boost/tuple/tuple.hpp"

CILAB_BEGIN_NAMESPACE(gbl)

enum ValidatorCondition {VC_REAL, VC_NONNEGATIVE_INTEGER, VC_POSITIVE_INTEGER, VC_NONNEGATIVE_REAL, VC_POSITIVE_REAL};

/**
This class helps to validate if the contents of the bridge are correct.

\author Maarten Nieber
\date 05/12/2007
*/

class Validator
{
public:
	Validator();

	boost::tuple<bool, double> GetRealNumber(const std::string& _value);
	boost::tuple<bool, int> GetNonNegativeInteger(const std::string& _value);
	boost::tuple<bool, int> GetPositiveInteger(const std::string& _value);
	boost::tuple<bool, double> GetNonNegativeRealNumber(const std::string& _value);
	boost::tuple<bool, double> GetPositiveRealNumber(const std::string& _value);
};

CILAB_END_NAMESPACE(gbl)

#endif //GBLVALIDATE_H
