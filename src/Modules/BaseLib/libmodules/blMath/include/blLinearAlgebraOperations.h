/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLLINEARALGEBRAOPERATIONS_H
#define BLLINEARALGEBRAOPERATIONS_H

#include "CILabNamespaceMacros.h"
#include "blLinearAlgebraOperationsUblas.h"
#include "blLinearAlgebraOperationsVnl.h"

/**
This namespace contains wrapper functions for various linear algebra operations.
It is intended to unify the interfaces of different matrix libraries.

\author Maarten Nieber
\date 09 jun 2008
*/

CILAB_BEGIN_NAMESPACE(bllao)

//! Copy \a from to \a to.
template< class Vector1, class Vector2 >
void Copy(const Vector1& from, Vector2& to)
{
	const int size = Size(from);
	Resize(to, size);
	for( int j = 0; j < size; ++j )
		to[j] = from[j];
}

//! Copy \a from to \a to.
template< class Matrix1, class Matrix2 >
void CopyMatrix(const Matrix1& from, Matrix2& to)
{
	const int nrRows = NrRows(from);
	const int nrCols = NrCols(from);
	Resize(to, nrRows, nrCols);
	
	for( int i = 0; i < nrRows; ++i )
	{
		for( int j = 0; j < nrCols; ++j )
		{
			to(i, j) = from(i, j);
		}
	}
}

//!
template< class Matrix >
void FillMatrix(Matrix& m, typename Namespace<Matrix>::value_type fillValue)
{
	const int nrRows = NrRows(m);
	const int nrCols = NrCols(m);
	for( int i = 0; i < nrRows; ++i )
	{
		for( int j = 0; j < nrCols; ++j )
		{
			m(i, j) = fillValue;
		}
	}
}

//!
template< class Vector >
void Fill(Vector& v, typename Namespace<Vector>::value_type x)
{
	const int size = Size(v);
	for( int j = 0; j < size; ++j )
		v[j] = x;
}

//! Return mean of the elements in \a v.
template< class Vector >
typename Namespace<Vector>::value_type Mean(Vector& v)
{
	typename Namespace<Vector>::value_type result(0);
	const int size = Size(v);
	for( int j = 0; j < size; ++j )
		result += v[j];
	return result / double(size);
}

//! Return first element of \a v.
template< class Vector >
typename Namespace<Vector>::value_type X(const Vector& v)
{
	return v[0];
}

//! Return second element of \a v.
template< class Vector >
typename Namespace<Vector>::value_type Y(const Vector& v)
{
	return v[1];
}

//! Return third element of \a v.
template< class Vector >
typename Namespace<Vector>::value_type Z(const Vector& v)
{
	return v[2];
}

CILAB_END_NAMESPACE(bllao)

#endif //BLLINEARALGEBRAOPERATIONS_H
