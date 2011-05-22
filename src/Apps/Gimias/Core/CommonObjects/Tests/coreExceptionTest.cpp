// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreExceptionTest.h"
#include "coreException.h"

void coreExceptionTest::TestWhat()
{
	std::string message("Test what?");
	Core::Exceptions::Exception e("coreExceptionTest::TestWhat", message.c_str());
	TS_ASSERT_EQUALS(std::string(e.what()), message) 
}
