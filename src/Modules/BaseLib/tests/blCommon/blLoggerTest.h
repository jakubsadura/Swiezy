// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef BLLOGGERTEST_H
#define BLLOGGERTEST_H

#include "cxxtest/TestSuite.h"

class blLoggerTest : public CxxTest::TestSuite
{
public:
   
   /**
   * Test the loading of a vtk data.
   */
   void TestLog();
	
};

#endif // BLLOGGERTEST_H

