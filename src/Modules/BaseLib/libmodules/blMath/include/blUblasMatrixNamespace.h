/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLUBLASMATRIXNAMESPACE_H
#define BLUBLASMATRIXNAMESPACE_H

#include "boost/shared_ptr.hpp"
#include "blUblasMatrix.h"
#include "CILabAssertMacros.h"
#include "boost/numeric/ublas/vector.hpp"

namespace Ublas = boost::numeric::ublas;

CILAB_BEGIN_NAMESPACE(baselib)

/**
This class contains the implementation of SVD for uBLAS matrices.
At the moment, SVD is actually not implemented (this class is just for making the code compile), 
and trying to use this class will raise an assertion.

\author Maarten Nieber
\date 04 jul 2008
*/

template< class MatrixType >
class UblasSvd
{
public:
	UblasSvd(const MatrixType& m, double precision)
	{
		cilabAssertMacro(false && "Not implemented");
	}
	MatrixType inverse()
	{
		cilabAssertMacro(false && "Not implemented");
		return MatrixType();
	}
};

CILAB_END_NAMESPACE(baselib)

/**
This class functions like a namespace. It is templated: for example, blUblasMatrixNamespace<double> contains types 
and functions for boost::numeric::ublas::matrix<double>.

\author Maarten Nieber
\date 04 jul 2008
*/

template< class S, class L, class A >
class blUblasMatrixNamespace
{
public:
	//! MatrixType described by this namespace.
	typedef Ublas::matrix<S, L, A> MatrixType;
	//! Scalar type of the MatrixType.
	typedef S value_type;
	//! A smart pointer to the MatrixType.
	typedef boost::shared_ptr< MatrixType > Pointer;
	//! A smart pointer to the const MatrixType.
	typedef boost::shared_ptr<const MatrixType> ConstPointer;
	//! The SVD class for this MatrixType.
	typedef typename baselib::UblasSvd<MatrixType> SVD;

	//! Returns a smart pointer with a new instantiation of the matrix type.
	static Pointer New(void) 
	{ 
		return Pointer(new MatrixType()); 
	};

	//! Returns a smart pointer with a new instantiation of the matrix type with \a r rows and \a c columns.
	static Pointer New(unsigned r, unsigned c) 
	{ 
		return Pointer(new MatrixType(r, c)); 
	};

	//! Returns a smart pointer with a new instantiation of the matrix type with \a r rows and \a c columns,
	//! initialized to \a v0.
	static Pointer New(unsigned r, unsigned c, value_type const& v0) 
	{ 
		return Pointer(new MatrixType(r, c, v0)); 
	};

	//! Returns a smart pointer with a new instantiation of the matrix type with \a r rows and \a c columns.
	//! The first \a n elements, are initialised row-wise, to \a values.
	static Pointer New(unsigned r, unsigned c, unsigned n, value_type const values[]) 
	{ 
		return Pointer(new MatrixType( r, c, n, values)); 
	};

	//! Returns a smart pointer with a new instantiation of the matrix type with \a r rows and \a c columns.
	//! Values are initialised by a memory block (the contents are copied to the matrix).
	static Pointer New(value_type const* data_block, unsigned r, unsigned c) 
	{ 
		return Pointer(new MatrixType( data_block, r, c )); 
	};
};

#endif //BLUBLASMATRIXNAMESPACE_H
