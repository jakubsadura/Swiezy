/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVectorRef_txx
#define __blVectorRef_txx

#include "blVectorRef.h"

template <class TElement>
blVectorRef<TElement>::blVectorRef() 
{
}

template <class TElement>
blVectorRef<TElement>::~blVectorRef() 
{
	this->data = 0;
}

/** \brief Creates vector containing n elements.
* Elements are not initialized.
*/
template <class TElement>
blVectorRef<TElement>::blVectorRef(unsigned size, TElement *space) : Superclass()
{
    this->data = space;
    this->num_elmts = size;

#if VCL_HAS_SLICED_DESTRUCTOR_BUG
    vnl_vector_own_data = 0;
#endif
}

#endif
