// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "blCxxTest.h"
#include "coreSettings.h"


/**
\brief Tests for Core::Settings
\ingroup CoreTests
\author Xavi Planes
\date 06-06-08

Tests a memory leak in the constructor

*/
class CoreSettingsTest : public CxxTest::TestSuite, public blCxxTest
{
// OPERATIONS
public:
	void setUp( )
	{
		blCxxTest::setUp( );
	}

	void tearDown( )
	{
		blCxxTest::tearDown( );
	}

	void test0( void ){
		m_Settings = Core::Runtime::Settings::New( "" );
		m_Settings = NULL;
	}

// ATTRIBUTES
protected:

	Core::Runtime::Settings::Pointer	m_Settings;

};

