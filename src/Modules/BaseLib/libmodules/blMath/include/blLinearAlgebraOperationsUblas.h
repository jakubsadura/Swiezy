/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLLINEARALGEBRAOPERATIONSUBLAS_H
#define BLLINEARALGEBRAOPERATIONSUBLAS_H

#include "blLinearAlgebraOperations.h"
#include "blUblasMatrixNamespace.h"
#include "blUblasConvertibleArray.h"
#include "CILabAssertMacros.h"
#include <algorithm>
#include "blLinearAlgebraTypes.h"
#include "boost/shared_ptr.hpp"
#include "blLinearAlgebraOperationsVnl.h"

namespace Ublas = boost::numeric::ublas;

/**
This namespace contains wrapper functions for various linear algebra operations.
It is intended to unify the interfaces of different matrix libraries.

\author Maarten Nieber
\date 09 jun 2008
*/

CILAB_BEGIN_NAMESPACE(bllao)

//! Returns number of rows of \a m.
template< class S, class L, class A >
int NrRows(const Ublas::matrix<S, L, A>& m)
{
	return m.size1();
}

//! Returns number of cols of \a m.
template< class S, class L, class A >
int NrCols(const typename Ublas::matrix<S, L, A>& m)
{
	return m.size2();
}

//!
template< class S, class A >
int Size(const Ublas::vector<S, A>& v)
{
	return v.size();
}

//!
template< class S, class A >
void Resize(Ublas::vector<S, A>& v, const int newSize)
{
	if ( bllao::Size(v) != newSize )
	{
		v.resize(newSize);
	}
}

//!
template< class S, class L, class A >
void Resize(Ublas::matrix<S, L, A>& m, const int nrRows, const int nrCols)
{
	if ( (bllao::NrRows(m) != nrRows) || (bllao::NrCols(m) != nrCols) )
	{
		m.resize(nrRows, nrCols);
	}	
}

//! Get a pointer to the start of the memory block of row \a i in matrix \a m.
template< class S, class L, class A >
S* StartOfMemoryBuffer(Ublas::matrix<S, L, A>& m, int i = 0)
{
	return &(m(i, 0));
}

//! Get a pointer to the start of the memory block of row \a i in matrix \a m.
template< class S, class L, class A >
const S* StartOfMemoryBuffer(const Ublas::matrix<S, L, A>& m, int i = 0)
{
	return &(m(i, 0));
}

//! Puts transpose of \a m in \a result.
template< class S, class L, class A >
void Transpose(const Ublas::matrix<S, L, A>& m, Ublas::matrix<S, L, A>& result)
{
	result = Ublas::trans(m);
}

//! Copies \a i-th row of \a m to \a v.
template< class S, class L, class A, class VectorType >
void CopyRowToVector(const Ublas::matrix<S, L, A>& m, int i, VectorType& v)
{
	const int nrCols = NrCols(m);
	Resize(v, nrCols);
	for( int j = 0; j < nrCols; ++j )
		v[j] = m(i, j);
}

//! Copies \a i-th row of \a m to \a v.
template< class T, class L, class A, class VectorType >
void CopyColumnToVector(const Ublas::matrix<T, L, A>& m, int j, VectorType& v)
{
	const int nrRows = NrRows(m);
	Resize(v, nrRows);
	for( int i = 0; i < nrRows; ++i )
		v[i] = m(i, j);
}

//! Copies \a v to \a i-th row of \a m.
template< class S, class L, class A, class VectorType >
void CopyVectorToRow(Ublas::matrix<S, L, A>& m, int i, const VectorType& v)
{
	const int nrCols = NrCols(m);
	for( int j = 0; j < nrCols; ++j )
		m(i, j) = v[j];
}

//! Copies \a v to \a i-th row of \a m.
template< class S, class L, class A, class VectorType >
void CopyVectorToColumn(Ublas::matrix<S, L, A>& m, int j, const VectorType& v)
{
	const int nrRows = NrRows(m);
	for( int i = 0; i < nrRows; ++i )
		m(i, j) = v[i];
}

//! Returns a raw pointer to the start of the memory block of \a v.
template< class S, class A >
S* StartOfMemoryBuffer(Ublas::vector<S, A>& v)
{
  return v.size() > 0 ? &(v[0]) : NULL;
}

//! Returns a raw pointer to the start of the memory block of \a v.
template< class S, class A >
const S* StartOfMemoryBuffer(const Ublas::vector<S, A>& v)
{
  return v.size() > 0 ? &(v[0]) : NULL;
}


/**
This templated struct contains typedefs that are dependent on MatrixType. It will be specialized for different
matrix types, such as Ublas::matrix and the ublas matrix.

\author Maarten Nieber
\date 10 jun 2008
*/

template< class S > struct Namespace;

//! Specialization of the Namespace struct for Ublas::matrix<S>.
//! bllao::Namespace< Ublas::matrix<S, L, A> > is equivalent to blUblasMatrixNamespace<S, L, A>.
template<class S, class L, class A> struct Namespace< Ublas::matrix<S, L, A> > : public blUblasMatrixNamespace<S, L, A>
{
};

//! Specialization of the Namespace struct for Ublas::vector<S>.
template<class S, class A> struct Namespace< Ublas::vector<S, A> >
{
	typedef S value_type;
	typedef Ublas::vector<S, A> VectorType;
	typedef boost::shared_ptr< VectorType > Pointer;
};

//! Stores A * B' in \a result.
template< class T, class L, class A >
void ABt(Ublas::matrix<T, L, A>& result, const Ublas::matrix<T, L, A>& matrixA, const Ublas::matrix<T, L, A>& matrixB)
{	
	result = Ublas::prod(matrixA, Ublas::trans(matrixB));
}

//! Stores A' * B in \a result.
template< class T, class L, class A >
void AtB(Ublas::matrix<T, L, A>& result, const Ublas::matrix<T, L, A>& matrixA, const Ublas::matrix<T, L, A>& matrixB)
{
	result = Ublas::prod(Ublas::trans(matrixA), matrixB);
}

//! Stores A' * A in \a result.
template< class T, class L, class A >
void AtA(Ublas::matrix<T, L, A>& result, const Ublas::matrix<T, L, A>& matrixA)
{
	result = Ublas::prod(Ublas::trans(matrixA), matrixA);
}

//! After calling, the norm of each column of m is unity.
template< class T, class L, class A >
void NormalizeColumns(Ublas::matrix<T, L, A>& m)
{
	const int nrCols = NrCols(m);
	const int nrRows = NrRows(m);
	for( int j = 0; j < nrCols; ++j )
	{
		double sumOfSquares(0);
		for( int i = 0; i < nrRows; ++i )
		{
			const double term = m(i, j);
			sumOfSquares += term * term;
		}
		const double factor = sqrt(sumOfSquares);
		for( int i = 0; i < nrRows; ++i )
		{
			m(i, j) /= factor;
		}
	}
}

CILAB_END_NAMESPACE(bllao)

CILAB_BEGIN_NAMESPACE(baselib)

//! Writes m to os in binary form.
template <class S, class L, class A>
void WriteToStream( const Ublas::matrix<S, L, A>& m, std::ostream& os )
{
	unsigned rows = bllao::NrRows(m);
	unsigned cols = bllao::NrCols(m);
	os.write( (const char*)&rows, sizeof( rows ) );
	os.write( (const char*)&cols, sizeof( cols ) );
	int nrItems = rows * cols;
	for( int i = 0; i < nrItems; ++i )
	{
		S item = m.data()[i];
		os.write( (const char*) &item, sizeof(S) );
	}
}

//! Reads m from os in binary form.
template <class S, class L, class A>
void ReadFromStream( Ublas::matrix<S, L, A>& m, std::istream& is )
{
	unsigned rows, cols;
	is.read( (char*)&rows, sizeof( rows ));
	is.read( (char*)&cols, sizeof( cols ));
	bllao::Resize(m, rows, cols);
	int nrItems = rows * cols;
	for( int i = 0; i < nrItems; ++i )
	{
		S item;
		is.read( (char*) &item, sizeof(S) );
		m.data()[i] = item;
	}
};

CILAB_END_NAMESPACE(baselib)

#endif //BLLINEARALGEBRAOPERATIONSUBLAS_H
