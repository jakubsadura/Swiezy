// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blLinearAlgebraOperationsTest.h"
#include "vnl/vnl_matrix.h"
#include <fstream>

namespace Private
{
	template< class Matrix1, class Matrix2 >
	bool AssertEqualMatrices(const Matrix1& m1, const Matrix2& m2, double delta)
	{
		int nRows = bllao::NrRows(m1);
		int nCols = bllao::NrCols(m1);
		TS_ASSERT_EQUALS( bllao::NrRows(m2), nRows );
		TS_ASSERT_EQUALS( bllao::NrCols(m2), nCols );
		for( int i = 0; i < nRows; ++i )
		{
			for( int j = 0; j < nCols; ++j )
			{
				TS_ASSERT_DELTA( m1(i, j), m2(i, j), delta );
			}
		}
	}
}

/**
Generic set of tests that can be run for ublas matrix and vnl_matrix.

\author Maarten Nieber
\date 11 jun 2008
*/

template< class MatrixType, class VectorType >
class MatrixTests
{
public:
	static MatrixType CreateMatrix( int nRows, int nCols, double add = 0 )
	{
		MatrixType m(nRows, nCols);
		for( int i = 0; i < nRows; ++i )
		{
			for( int j = 0; j < nCols; ++j )
			{
				m(i, j) = i * 10 + j + add;
			}
		}
		return m;
	}

	static void TestCopyRowToVector()
	{
		int nRows = 2, nCols = 3;
		MatrixType m = CreateMatrix(nRows, nCols);
		VectorType row;
		const int i = 1;
		bllao::CopyRowToVector(m, i, row);
		for( int j = 0; j < nCols; ++j )
		{
			TS_ASSERT_EQUALS( row[j], m(i, j) );
		}
	}

	static void TestCopyVectorToRow()
	{
		int nRows = 2, nCols = 3;
		MatrixType m = CreateMatrix(nRows, nCols);
		VectorType row(nCols);
		for( int j = 0; j < nCols; ++j )
			row[j] = 100 + j * 234;

		const int i = 1;
		bllao::CopyVectorToRow(m, i, row);
		for( int j = 0; j < nCols; ++j )
		{
			TS_ASSERT_EQUALS( row[j], m(i, j) );
		}
	}

	static void TestCopyMatrix()
	{
		typedef vnl_matrix<double> VnlMatrix;
		typedef vnl_vector<double> VnlVector;
		int nRows = 2, nCols = 3;
		VnlMatrix vnlMatrix = MatrixTests<VnlMatrix, VnlVector>::CreateMatrix(nRows, nCols);

		MatrixType m;
		bllao::CopyMatrix(vnlMatrix, m);
		Private::AssertEqualMatrices(m, vnlMatrix, 0.00001);
	}

	static void TestCopyVector()
	{
		vnl_vector<double> v(3);
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		VectorType v2;
		bllao::Copy(v, v2);
		TS_ASSERT_EQUALS( bllao::Size(v2), 3 );
		TS_ASSERT_EQUALS( v[0], v2[0] );
		TS_ASSERT_EQUALS( v[1], v2[1] );
		TS_ASSERT_EQUALS( v[2], v2[2] );
	}

	static void TestBasicStuff()
	{
		int nRows = 2, nCols = 3;
		MatrixType m = CreateMatrix(nRows, nCols);
		TS_ASSERT_EQUALS(nRows, bllao::NrRows(m));
		TS_ASSERT_EQUALS(nCols, bllao::NrCols(m));
	}
};

void blLinearAlgebraOperationsVnlTest::TestCopyRowToVector()
{
	MatrixTests<MatrixType, VectorType>::TestCopyRowToVector();
}

void blLinearAlgebraOperationsVnlTest::TestCopyVectorToRow()
{
	MatrixTests<MatrixType, VectorType>::TestCopyVectorToRow();
}

void blLinearAlgebraOperationsVnlTest::TestBasicStuff()
{
	MatrixTests<MatrixType, VectorType>::TestBasicStuff();
}

void blLinearAlgebraOperationsVnlTest::TestCopyVector()
{
	MatrixTests<MatrixType, VectorType>::TestCopyVector();
}

void blLinearAlgebraOperationsUblasTest::TestCopyRowToVector()
{
	MatrixTests<MatrixType, VectorType>::TestCopyRowToVector();
}

void blLinearAlgebraOperationsUblasTest::TestCopyVectorToRow()
{
	MatrixTests<MatrixType, VectorType>::TestCopyVectorToRow();
}

void blLinearAlgebraOperationsUblasTest::TestBasicStuff()
{
	MatrixTests<MatrixType, VectorType>::TestBasicStuff();
}

void blLinearAlgebraOperationsUblasTest::TestCopyVector()
{
	MatrixTests<MatrixType, VectorType>::TestCopyVector();
}

void blLinearAlgebraOperationsUblasTest::TestCopyMatrix()
{
	MatrixTests<MatrixType, VectorType>::TestCopyMatrix();
}

void blLinearAlgebraOperationsUblasTest::TestNormalizeColumns()
{
	const int nRows(4), nCols(5);
	baselib::VnlMatrixType vnlMatrix = 
		MatrixTests<baselib::VnlMatrixType, baselib::VnlVectorType>::CreateMatrix(nRows, nCols);
	baselib::UblasMatrixType ublasMatrix = 
		MatrixTests<baselib::UblasMatrixType, baselib::UblasVectorType>::CreateMatrix(nRows, nCols);
	bllao::NormalizeColumns(vnlMatrix);
	bllao::NormalizeColumns(ublasMatrix);
	Private::AssertEqualMatrices(vnlMatrix, ublasMatrix, 0.00001);
}

void blLinearAlgebraOperationsUblasTest::TestABt()
{
	const int n(4), m(5);
	baselib::VnlMatrixType vnlMatrix1 = 
		MatrixTests<baselib::VnlMatrixType, baselib::VnlVectorType>::CreateMatrix(n, m);
	baselib::VnlMatrixType vnlMatrix2 = 
		MatrixTests<baselib::VnlMatrixType, baselib::VnlVectorType>::CreateMatrix(n, m, 123.4);
	baselib::UblasMatrixType ublasMatrix1 = 
		MatrixTests<baselib::UblasMatrixType, baselib::UblasVectorType>::CreateMatrix(n, m);
	baselib::UblasMatrixType ublasMatrix2 = 
		MatrixTests<baselib::UblasMatrixType, baselib::UblasVectorType>::CreateMatrix(n, m, 123.4);

	baselib::VnlMatrixType vnlResultMatrix;
	bllao::ABt(vnlResultMatrix, vnlMatrix1, vnlMatrix2);
	baselib::UblasMatrixType ublasResultMatrix;
	bllao::ABt(ublasResultMatrix, ublasMatrix1, ublasMatrix2);

	Private::AssertEqualMatrices(vnlResultMatrix, ublasResultMatrix, 0.00001);
}

void blLinearAlgebraOperationsUblasTest::TestAtA()
{
	const int n(4), m(4);
	baselib::VnlMatrixType vnlMatrix1 = 
		MatrixTests<baselib::VnlMatrixType, baselib::VnlVectorType>::CreateMatrix(n, m);
	baselib::UblasMatrixType ublasMatrix1 = 
		MatrixTests<baselib::UblasMatrixType, baselib::UblasVectorType>::CreateMatrix(n, m);

	baselib::VnlMatrixType vnlResultMatrix;
	bllao::AtA(vnlResultMatrix, vnlMatrix1);
	baselib::UblasMatrixType ublasResultMatrix;
	bllao::AtA(ublasResultMatrix, ublasMatrix1);

	Private::AssertEqualMatrices(vnlResultMatrix, ublasResultMatrix, 0.00001);
}
