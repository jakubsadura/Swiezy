// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blUblasConvertibleArrayTest.h"
#include "vnl/vnl_matrix.h"

void blUblasConvertibleArrayTest::TestConversion()
{
	vnl_matrix<double> mat(10, 20);
	blUblasConvertibleArray<double, std::allocator<double> > array(mat.data_block(), mat.size());
	TS_ASSERT_EQUALS( mat.size(), array.size() );
	TS_ASSERT_EQUALS( mat(0,9), array[9] );
}
