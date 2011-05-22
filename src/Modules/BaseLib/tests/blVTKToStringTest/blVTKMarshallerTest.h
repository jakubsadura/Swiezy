/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLVTKMARSHALLERTEST_H
#define BLVTKMARSHALLERTEST_H

#include "CILabNamespaceMacros.h"
#include "cxxtest/TestSuite.h"

class blVTKMarshallerTest : public CxxTest::TestSuite
{
public:
	void TestWithPolyData();
	void TestWithImage();
};

#endif //BLVTKMARSHALLERTEST_H
