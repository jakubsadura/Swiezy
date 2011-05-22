/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#pragma once

#include "cxxtest/TestSuite.h"
#include "blMemLeakDetect.h"
#include <iostream>

/**
\brief Memory leaks detector functionality for CxxTest
\ingroup BaseLibCxxTest
\author Xavi Planes
\date 06-06-08

I would like to subclass it from CxxTest::TestSuite but then
the test class will not be derived from CxxTest::TestSuite and
the source file for the test cannot be generated automatically.

\note We need to define the functions here because the used macros
of CxxTest need to be included directly from the test.

*/
class blCxxTest 
{
// OPERATIONS
public:

	//! Create blMemLeakDetect
	void setUp()
	{
		std::cout << std::endl;
		#if defined(_DEBUG) && defined(WIN32)
			m_blMemLeakDetect = new blMemLeakDetect;
		#endif
	};
	
	//! If debug and windows -> TS_ASSERT( num memory leaks == 0 )
	void tearDown()
	{
		#if defined(_DEBUG) && defined(WIN32)
			std::cout 
				<< "Memory leaks: " 
				<< m_blMemLeakDetect->GetNumMemoryLeaks( ) 
				<< std::endl;
			TS_ASSERT( m_blMemLeakDetect->GetNumMemoryLeaks( ) == 0 );
			delete m_blMemLeakDetect;
		#else
			std::cout 
				<< "Waring: This test can only run in DEBUG and WINDOWS" 
				<< std::endl;
			TS_ASSERT( true );
		#endif
	}


// ATTRIBUTES
protected:

	#if defined(_DEBUG) && defined(WIN32)
		blMemLeakDetect		*m_blMemLeakDetect;
	#endif
};

