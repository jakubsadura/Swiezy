#ifndef BLIMAGEUTILSTEST_H
#define BLIMAGEUTILSTEST_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"
#include "blImageUtils.h"

class blImageUtilsTest : public CxxTest::TestSuite
{
public:
	/**
	* NOT WORKING...
	*/
	//void TestLoadImageFromFileAsVTK();
   
   /**
   * Test the loading of a vtk data.
   */
   void TestLoadVTKImage();
	
	/**
	* Test the loading and writing of data using the ITK reader/writer to Anlayze format.
	*/
	void TestLoadWriteAnalyzeImage();

	/**
	* Test the loading and writing of data using the ITK reader/writer to Nifti format.
	*/
	void TestLoadWriteNiftiImage();

	/**
	* Test loading Nifti data with Axial orientation.
	*/
	void TestNiftiImageOrientationAxial();

	/**
	* Test loading Nifti data with Coronal orientation.
	*/
	void TestNiftiImageOrientationCoronal();

	/**
	* Test loading Nifti data with Sagittal orientation.
	*/
	void TestNiftiImageOrientationSagittal();

	/**
	* Test the loading and writing (to vtk) of data using the blV3dImageFileReader.
	*/
	void TestLoadWriteV3dImage();
};

#endif //BLIMAGEUTILSTEST_H

