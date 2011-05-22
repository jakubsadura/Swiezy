#ifndef BLLINEARALGEBRAOPERATIONSTEST_H
#define BLLINEARALGEBRAOPERATIONSTEST_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"
#include "blLinearAlgebraOperations.h"

/**
Tests basic linear algebra operations on vnl_matrix.

\author Maarten Nieber
\date 11 jun 2008
*/

class blLinearAlgebraOperationsVnlTest : public CxxTest::TestSuite 
{
public:
	typedef vnl_matrix<double> MatrixType;
	typedef vnl_vector<double> VectorType;

	void TestCopyRowToVector();
	void TestCopyVectorToRow();
	void TestCopyVector();
	void TestBasicStuff();
};

/**
Tests basic linear algebra operations on ublas matrix.

\author Maarten Nieber
\date 11 jun 2008
*/

class blLinearAlgebraOperationsUblasTest : public CxxTest::TestSuite 
{
public:
	typedef Ublas::matrix<double, boost::numeric::ublas::row_major, blUblasConvertibleArray<double> > MatrixType;
	typedef Ublas::vector<double, blUblasConvertibleArray<double> > VectorType;

	void TestCopyRowToVector();
	void TestCopyVectorToRow();
	void TestCopyVector();
	void TestBasicStuff();
	void TestCopyMatrix();
	void TestNormalizeColumns();
	void TestABt();
	void TestAtA();
};

#endif //BLLINEARALGEBRAOPERATIONSTEST_H
