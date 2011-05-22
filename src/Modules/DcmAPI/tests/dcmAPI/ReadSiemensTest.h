/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#pragma once

#include "cxxtest/TestSuite.h"

/**
 * \brief Tests for reading Siemens DICOM
 * \ingroup dcmAPI
 */
class ReadSiemensTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test reading of Siemens NM.
	 */
	void TestReadSiemensNuclearModality();

};

