#ifndef COREVTKIMAGEDATACONVERTERTEST_H
#define COREVTKIMAGEDATACONVERTERTEST_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"
#include "coreDataEntity.h"
#include "coreDataHolder.h"

/**
\ingroup CoreTests
*/
class VTKImageDataConverterTest : public CxxTest::TestSuite
{
public:
	void _TestConversion();

private:
	Core::DataEntity::Pointer m_ImageDataEntity;
};

#endif //COREVTKIMAGEDATACONVERTERTEST_H

