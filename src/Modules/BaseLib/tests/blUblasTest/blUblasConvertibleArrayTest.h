// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "cxxtest/TestSuite.h"

// BaseLib
#include "blUblasConvertibleArray.h"

// CLASS DEFINITION

/**
\brief Tests for itk::ImageToVTKImageFilter
\ingroup BaseLibVTKTest
\author Xavi Planes
\date 16-05-08
*/
class blUblasConvertibleArrayTest : public CxxTest::TestSuite 
{
public:
	void TestConversion();
};



