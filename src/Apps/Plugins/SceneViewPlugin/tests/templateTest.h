/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef TEMPLATETEST_H
#define TEMPLATETEST_H

#include "cxxtest/TestSuite.h"

/**
*\brief Tests for MyLib
*\ingroup MyLibTests
*/
class templateTest : public CxxTest::TestSuite 
{
public:

void TestOne()
{
   TSM_ASSERT( "TestOne run ok.", true );
}

}; // class templateTest

#endif TEMPLATETEST_H
