/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVectorRef_h
#define __blVectorRef_h

#include "blSmartPointer.h"
#include "blVector.h"

/**  
 * \brief reference vector type, similar to vnl_vector_ref
 * \ingroup blMath
 * 
 * Very useful when a function needs a reference to blVector<TElment> 
 * but what you have is simply an array of TElement. Then istead of 
 * creating an instance of blVector or separate function that takes an 
 * array, you can use this class.
 * 
 * For example:
 * \code
 * void DoSmth( blVector<double>* aaa )
 * {
 * ....	 
 * }
 *
 * void main(...)
 * {
 * 	double vector[20];
 *		blVectorRef<double> vector_ref( 20, vector );
 *		DoSmth( &vector_ref );
 * }
 * \endcode
 *
 * Also this class should allow calling all the standard blVector functions except the set_size()
 * which will fail whenever the specified size is different from the size of the array.
 *
 * Data, pointer to which is stored in this class are not freed by the class.
 *
 * Note that there is no static constructor!
 */
template <class TElement>
class blVectorRef: public blVector<TElement>::VectorType
{
public:
	typedef typename blVector<TElement>::VectorType Superclass;
	typedef TElement value_type;

	/** \brief Creates vector containing n elements.
		* Elements are not initialized.
		*/
	blVectorRef(unsigned size, TElement *space);

	bool set_size (unsigned n)
	{
	assert( n == this->size() );
	return n == this->size();
	}
	
	virtual ~blVectorRef();

protected:
	blVectorRef();



private:
	blVectorRef(const blVectorRef&); //purposely not implemented
	void operator=(const blVectorRef&); //purposely not implemented
};


#include "blVectorRef.txx"

#endif
