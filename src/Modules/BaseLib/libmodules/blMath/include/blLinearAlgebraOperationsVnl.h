/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLLINEARALGEBRAOPERATIONSVNL_H
#define BLLINEARALGEBRAOPERATIONSVNL_H

#include "blLinearAlgebraOperations.h"
#include "blMatrix.h"
#include "blVector.h"
#include "boost/shared_ptr.hpp"
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector_ref.h>
#include "vnl/vnl_fastops.h"
#include "blUblasMatrix.h"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/shared_ptr.hpp"
#include <algorithm>

namespace Ublas = boost::numeric::ublas;

/**
This namespace contains wrapper functions for various linear algebra operations.
It is intended to unify the interfaces of different matrix libraries.

\author Maarten Nieber
\date 09 jun 2008
*/

CILAB_BEGIN_NAMESPACE(bllao)

//! Returns number of rows of \a m.
template< class S >
int NrRows(const vnl_matrix<S>& m)
{
	return m.rows();
}

//! Returns number of cols of \a m.
template< class S >
int NrCols(const vnl_matrix<S>& m)
{
	return m.cols();
}

//! Puts transpose of \a m in \a result.
template< class S >
void Transpose(const vnl_matrix<S>& m, vnl_matrix<S>& result)
{
	result = m.transpose();
}

//! Copies \a i-th row of \a m to \a v.
template< class S >
void CopyRowToVector(const vnl_matrix<S>& m, int i, vnl_vector<S>& v)
{
	v = vnl_vector_ref<S>(m.cols(), (const_cast< vnl_matrix<S>& >(m)) [i]);
}

//! Copies \a j-th row of \a m to \a v.
template< class T >
void CopyColumnToVector(const vnl_matrix<T>& m, int j, vnl_vector<T>& v)
{
	v = m.get_column(j);
}

//! Copies \a v to \a i-th row of \a m.
template< class S >
void CopyVectorToRow(vnl_matrix<S>& m, int i, const vnl_vector<S>& v)
{
	m.set_row(i, v);
}

//! Copies \a v to \a i-th row of \a m.
template< class S >
void CopyVectorToColumn(vnl_matrix<S>& m, int j, const vnl_vector<S>& v)
{
	m.set_column(j, v);
}

//!
template< class S >
int Size(const vnl_vector<S>& v)
{
	return v.size();
}

//! Returns a raw pointer to the start of the memory block of \a v.
template< class S >
S* StartOfMemoryBuffer(vnl_vector<S>& v)
{
	return Size(v) > 0 ? &(v[0]) : NULL;
}

//! Returns a raw pointer to the start of the memory block of \a v.
template< class S >
const S* StartOfMemoryBuffer(const vnl_vector<S>& v)
{
	return Size(v) > 0 ? &(v[0]) : NULL;
}

//!
template< class S >
void Resize(vnl_vector<S>& v, const int newSize)
{
	if ( bllao::Size(v) != newSize )
	{
		v.set_size(newSize);
	}
}

//!
template< class S >
void Resize(vnl_matrix<S>& m, const int nrRows, const int nrCols)
{
	if ( (bllao::NrRows(m) != nrRows) || (bllao::NrCols(m) != nrCols) )
	{
		m.set_size(nrRows, nrCols);
	}
}

//! Get a pointer to the start of the memory block of row \a i in matrix \a m.
template< class S >
S* StartOfMemoryBuffer(vnl_matrix<S>& m, int i = 0)
{
	return &(m(i, 0));
}

//! Get a pointer to the start of the memory block of row \a i in matrix \a m.
template< class S >
const S* StartOfMemoryBuffer(const vnl_matrix<S>& m, int i = 0)
{
	return &(m(i, 0));
}

/**
This templated struct contains typedefs that are dependent on MatrixType. It will be specialized for different
matrix types, such as vnl_matrix and the ublas matrix.

\author Maarten Nieber
\date 10 jun 2008
*/

template< class MatrixType > struct Namespace;

//! Specialization of the Namespace struct for vnl_matrix<S>.
//! bllao::Namespace< vnl_matrix<S> > is equivalent to blMatrix<S>.
template<class S> struct Namespace< vnl_matrix<S> > : public blMatrix<S>
{
};

//! Specialization of the Namespace struct for vnl_vector<S>.
//! bllao::Namespace< vnl_vector<S> > is equivalent to blVector<S>.
template<class S> struct Namespace< vnl_vector<S> > : blVector<S>
{
};

//! Specialization of the Namespace struct for vnl_vector_ref<S>.
template<class S> struct Namespace< vnl_vector_ref<S> >
{
	typedef S value_type;
	typedef vnl_vector_ref<S> VectorType;
	typedef boost::shared_ptr<VectorType> Pointer;
};

//! Stores A * B' in \a result.
template< class T >
void ABt(vnl_matrix<T>& result, const vnl_matrix<T>& A, const vnl_matrix<T>& B)
{
	vnl_fastops::ABt(result, A, B);
}

//! Stores A' * B in \a result.
template< class T >
void AtB(vnl_matrix<T>& result, const vnl_matrix<T>& A, const vnl_matrix<T>& B)
{
	vnl_fastops::AtB(result, A, B);
}

//! Stores A' * A in \a result.
template< class T >
void AtA(vnl_matrix<T>& result, const vnl_matrix<T>& A)
{
	vnl_fastops::AtA(result, A);
}

//! After calling, the norm of each column of m is unity.
template< class T >
void NormalizeColumns(vnl_matrix<T>& m)
{
	m.normalize_columns();
}

CILAB_END_NAMESPACE(bllao)

#endif //BLLINEARALGEBRAOPERATIONSVNL_H
