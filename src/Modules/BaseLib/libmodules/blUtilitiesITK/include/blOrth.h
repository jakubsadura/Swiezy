/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blOrth_h_ 
#define __blOrth_h_

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

/**
 * \ingroup blUtilities
 * vectors in columns
 * for Gramm Shmidt make sure that matrix doesn't have zero columns
 */
namespace blOrth
{
	/** 
	 * \brief If transposed = false - vectors are in columns, if true - in rows 
	 * default is false (for some reason couldn't put it here, it's in TXX file)
	 * \ingroup blUtilities
	 */
	template <class TElement> 
	void GramSchmidt( vnl_matrix<TElement>* matrix, bool transposed /* =false */);

	template <class TElement> 
	void GramSchmidtModified( vnl_matrix<TElement>* matrix, 
						bool transposed /* =false */, 
						vnl_vector<TElement>* lengths /*=NULL*/ );

	//borrowed from matlab orth.m
        //supposed to be the best choice
	template <class TElement> 
	void OrthonormalizeViaSVD( vnl_matrix<TElement>* matrix );
}

#include "blOrth.txx"
#endif
