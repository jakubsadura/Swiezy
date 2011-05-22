/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREDataEntityTest_H
#define COREDataEntityTest_H

#include "cxxtest/TestSuite.h"

/** Test Core::DataEntity
\author Xavi Planes
\date 22 sept 2010
\ingroup CoreTests
 */
class CoreDataEntityTest : public CxxTest::TestSuite
{
public:
	//!
	void setUp();
	//!
	void tearDown();
public:
	//! Test shape
	void TestShape();
	//! Test image
	void TestImage();
	//! Test Copy
	void TestCopy();
};

#endif //COREDataEntityTest_H
