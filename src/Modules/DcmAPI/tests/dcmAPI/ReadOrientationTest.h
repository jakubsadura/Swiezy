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
 * \brief Tests for reading oriented DICOM data. This is done 
 * \ingroup dcmAPI
 * At the moment the tests are using dcmAPI::ImageUtilities::ReadImageFromFile
 * to read the DICOM files, they should use regular API (DataSetReader)
 * and possibly other storage cases (DICOMDIR, one slice file...). 
 */
class ReadOrientationTest : public CxxTest::TestSuite 
{
public:

	/**
	 * \test Test reading of axial data.
	 */
	void TestReadAxial();
	
	/**
	 * \test Test reading of axial2 data.
	 */
	void TestReadAxial2();

	/**
	 * \test Test reading of coronal data.
	 */
	void TestReadCoronal();
	
	/**
	 * \test Test reading of coronal2 data.
	 */
	void TestReadCoronal2();

	/**
	 * \test Test reading of sagittal data.
	 */
	void TestReadSagittal();
	
	/**
	 * \test Test reading of sagittal2 data.
	 */
	void TestReadSagittal2();

private:

   /**
   * Core testing code.
   * \param dcmFileNameEnd The path of the DICOM file (relative to the CISTIB_TOOLKIT_FOLDER).
   * \param vtkFileNameEnd The path of the vtk file (relative to the CISTIB_TOOLKIT_FOLDER).
   * \return True if both datas are equal (according to blImageUtils::CompareImages).
   */
   bool ReadAndCompare( const std::string& dcmFileNameEnd, const std::string& vtkFileNameEnd ) const;
   
};

