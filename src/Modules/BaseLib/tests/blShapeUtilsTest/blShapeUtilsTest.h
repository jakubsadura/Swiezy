// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef blShapeUtilsTest_H
#define blShapeUtilsTest_H

#include "cxxtest/TestSuite.h"
#include "blImageUtils.h"

/** 
\brief Tests for blShapeUtils::ShapeUtils::LoadShapeFromFile( )
\ingroup BaseLibVTKTest
\author Xavi Planes
\date 18-06-08
*/
class blShapeUtilsTest : public CxxTest::TestSuite
{
public:
	/**
	\brief Test loading VTP vtkPolyData file
	 */
	void TestLoadVTPShapeFromFile();
	/**
	\brief Test loading VTK (non plydata) vtkPolyData files
	 */
	void TestLoadVTKShapeFromFile();
	/**
	\brief Test loading VTK Polydata vtkPolyData files
	 */
	void TestLoadVTKPolyShapeFromFile();
	/**
	\brief Test loading STL vtkPolyData files
	 */
	void TestLoadStlShapeFromFile();
	
	/**
	\brief Test loading a Cube and computing the volume
	 */
	void TestComputeVolume();
	/**
	\brief Test loading a mesh with holes and computing the number of holes
	*/
	void TestComputeNumberOfHoles();
};

#endif //blShapeUtilsTest_H

