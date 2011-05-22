/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#pragma once

#include "cxxtest/TestSuite.h"

/**
 * \brief Tests for the MultiSliceReader.
 * \ingroup dcmAPI
 */
class MultiSliceReaderTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test reading without a specific reader.
	 */
	void TestReadWithoutReader();

};

