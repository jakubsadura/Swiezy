/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blNumericUtils_h
#define __blNumericUtils_h

/**
\brief Compare two float numbers with precision epsilon (for example
\ingroup blPDShape (0.0001)
*/
#define FLOAT_EQ(x,v,epsilon) \
	( ( (v - epsilon) < x ) && (x < ( v + epsilon) ) )

#define FLOAT_EQ_INCLUSIVE(x,v,epsilon) \
	( ( (v - epsilon) <= x ) && (x <= ( v + epsilon) ) )

namespace blNumericUtils
{

	/**
	\ingroup blCommon
	\author Xavi Planes
	\date 15-09-09
	*/
	long round(double a);

}


#endif // __blNumericUtils_h
