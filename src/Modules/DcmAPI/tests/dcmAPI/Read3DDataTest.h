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
 * \brief Tests for reading general 3D DICOM data.
 * \ingroup dcmAPI
 */
class Read3DDataTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test the reading of a single file with a single slice.
	 */
	void TestReadSingleSliceFile();

	/**
	 * \test Test the reading of a single file with multiple slices.
	 */
	void TestReadMultiSliceFile();

	/**
	 * \test Test the reading of a directory.
	 */
	void TestReadDirectory();

	/**
	 * \test Test the reading of a DICOM DIR.
	 */
	void TestReadDicomDir();

	/**
	 * \test Test the reading of a directory dcmAPI::DataSet.
	 */
	void TestReadDirectoryDataSet();

	/**
	 * \test Test the reading of a DICOM DIR dcmAPI::DataSet.
	 */
	void TestReadDicomDirDataSet();
	
private:

   /**
   * ReadAndCompare method possible input types.
   */
   enum TYPE{ FILE, FOLDER, DICOMDIR } InputType;

   /**
   * Compare images read from input files.
   * \param dcmInputNameEnd The path of the DICOM file, folder or DICOMDIR (relative to the CISTIB_TOOLKIT_FOLDER).
   * \param vtkFileNameEnd The path of the vtk file (relative to the CISTIB_TOOLKIT_FOLDER).
   * \param type The type of the input.
   * \return True if both datas are equal (according to blImageUtils::CompareImages).
   */
   bool ReadAndCompare( 
      const std::string& dcmInputNameEnd, 
      const std::string& vtkFileNameEnd,
      const TYPE& inputType ) const;

}; // class Read3DDataTest

