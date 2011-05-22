/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#pragma once

#include <string>
#include "cxxtest/TestSuite.h"

/**
 * \brief Tests for reading general 2D DICOM data.
 * \ingroup dcmAPI
 */
class Read2DDataTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test the reading of a single file with a single slice.
	 */
	void TestReadSingleSliceFile();

	/**
	 * \test Test the reading of a slice with rescale data.
	 */
	void TestReadRescale();

private:

   /**
   * Compare images read from input files.
   * \param dcmInputNameEnd The path of the DICOM file (relative to the CISTIB_TOOLKIT_FOLDER).
   * \param vtkFileNameEnd The path of the vtk file (relative to the CISTIB_TOOLKIT_FOLDER).
   * \return True if both datas are equal (according to blImageUtils::CompareImages).
   */
   bool ReadAndCompare( 
      const std::string& dcmInputNameEnd, 
      const std::string& vtkFileNameEnd ) const;

};

