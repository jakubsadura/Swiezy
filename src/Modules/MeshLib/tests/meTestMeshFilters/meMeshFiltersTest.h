// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef meMeshFiltersTest_H
#define meMeshFiltersTest_H

#include "cxxtest/TestSuite.h"

/** 
\brief Tests for meMeshFilters
\ingroup meMeshFiltersTest
\author Chiara Riccobene
\date 3-11-09
*/
class meMeshFiltersTest : public CxxTest::TestSuite
{
public:
	/**
	\brief Test ExtractMainSurface
	 */
	void TestExtractMainSurface();
	/**
	\brief Test VolumeClosing
	 */
	void TestVolumeClosing();
	/**
	\brief Test Smoothing Taubin
	*/
	void TestSmoothingTaubin();
	/**
	\brief Test Smoothing Umbrella
	*/
	void TestSmoothingUmbrella();
	/**
	\brief Test LoopSubdivisionRefiner
	*/
	void TestLoopSubdivisionRefiner();
	/**
	\brief Test Local Refiner
	*/
	void TestLocalRefiner();
	/**
	\brief Test EdgeSwapping
	*/
	void TestEdgeSwapping();
	/**
	\brief Test TetraGeneration
	*/
	void TestTetraGeneration();
	/**
	\brief Test Skeletonization
	*/
	void TestSkeletonization();
	/**
	\brief Test VolumeClipping
	*/
	void TestVolumeClipping();
	
};

#endif //meMeshFiltersTest_H

