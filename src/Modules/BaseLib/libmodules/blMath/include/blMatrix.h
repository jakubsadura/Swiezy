/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blMatrix_h
#define __blMatrix_h

#include "blSmartPointer.h"
#include "blLightObject.h"
#include "blLinearAlgebraTypes.h"
#include "CILabAssertMacros.h"
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_svd.h>
#include <iostream>

/**
 * \brief Matrix type, derived from vnl_matrix
 * \ingroup blMath
 */
template< class TElement >
class blMatrix
{
public:
	typedef vnl_matrix<TElement> MatrixType;
	typedef TElement value_type;
	typedef boost::shared_ptr<MatrixType> Pointer;
	typedef boost::shared_ptr<const MatrixType> ConstPointer;
	typedef vnl_svd<TElement> SVD;

	static Pointer New(void) 
	{ 
		return Pointer(new MatrixType()); 
	};

	/** \brief Construct a matrix of size r rows by c columns
		* Contents are unspecified.
		* Complexity $O(1)$
		*/
	static Pointer New(unsigned r, unsigned c) 
	{ 
		return Pointer(new MatrixType(r, c)); 
	};

	/** \brief  Construct a matrix of size r rows by c columns, and all emelemnts equal to v0
		* Complexity $O(r.c)$
		*/
	static Pointer New(unsigned r, unsigned c, TElement const& v0) 
	{ 
		return Pointer(new MatrixType(r, c, v0)); 
	};

	/** \brief  Construct a matrix of size r rows by c columns, initialised by an automatic array
		* The first n elements, are initialised row-wise, to values.
		* Complexity $O(n)$
		*/
	static Pointer New(unsigned r, unsigned c, unsigned n, TElement const values[]) 
	{ 
		return Pointer(new MatrixType( r, c, n, values)); 
	};

	/** \brief  Construct a matrix of size r rows by c columns, initialised by a memory block
		* The values are initialise row wise from the data.
		* Complexity $O(r.c)$
		*/
	static Pointer New(TElement const* data_block, unsigned r, unsigned c) 
	{ 
		return Pointer(new MatrixType( data_block, r, c )); 
	};
};

CILAB_BEGIN_NAMESPACE(baselib)

//binary stream input/output (at least until we have functional readers/writers)
template <class TElement>
void WriteToStream( const vnl_matrix<TElement>& m, std::ostream& os );

template <class TElement>
void ReadFromStream( vnl_matrix<TElement>& m, std::istream& is );

CILAB_END_NAMESPACE(baselib)

#include "blMatrix.txx"

#endif
