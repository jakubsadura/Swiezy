#ifndef COREDATAENTITYLISTTEST_H
#define COREDATAENTITYLISTTEST_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"

/**
\ingroup CoreTests
*/
class CoreDataEntityListTest : public CxxTest::TestSuite
{
public:
	void TestUnknownDataEntityId();
	void TestAdd();
};

#endif //COREDATAENTITYLISTTEST_H
