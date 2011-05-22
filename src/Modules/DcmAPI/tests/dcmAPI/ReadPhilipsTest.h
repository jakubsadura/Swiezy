/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#pragma once

#include "cxxtest/TestSuite.h"

/**
 * \brief Tests for reading Philips specific DICOM.
 * \ingroup dcmAPI
 */
class ReadPhilipsTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test reading of Philips 3DRA.
	 */
	void TestReadPhilips3DRAprojection();

};

