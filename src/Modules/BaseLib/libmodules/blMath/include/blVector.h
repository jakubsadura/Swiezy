/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVector_h
#define __blVector_h

#include "blSmartPointer.h"
#include "boost/shared_ptr.hpp"
#include "blLightObject.h"

#include <vnl/vnl_vector.h>

/**
 * \brief Vector type, derived from vnl_matrix
 * \ingroup blMath
 *
 * \note To use operators such as [],+, -, etc. don't forget to 
 * dereference pointer E.g., to access data, use (*pixels)[i]
 */
template <class TElement>
class blVector
{
public:
	typedef vnl_vector<TElement> VectorType;
	typedef TElement value_type;
	typedef boost::shared_ptr<VectorType> Pointer;
	typedef boost::shared_ptr<const VectorType> ConstPointer;

	static Pointer New(void) 
	{ 
		return Pointer(new VectorType()); 
	};

	/** \brief Creates vector containing n elements.
		* Elements are not initialized.
		*/
	static Pointer New(unsigned len) 
	{ 
		return Pointer(new VectorType(len)); 
	};

	/** \brief Creates vector of len elements, all set to v0
		* 
		*/
	static Pointer New(unsigned len, TElement const& v0) 
	{ 
		return Pointer(new VectorType(len, v0)); 
	};

	/** \brief Creates a vector of specified length and 
	   	*  initialize first n elements with values. O(n).
		*/
	static Pointer New(unsigned len, int n, TElement const values[]) 
	{ 
		return Pointer(new VectorType(len, n, values)); 
	};

	/** \brief Create n element vector and copy data from data_block
	   	*  
		*/
	static Pointer New(TElement const* data_block,unsigned int n) 
	{ 
		return Pointer(new VectorType(data_block, n)); 
	};


	/** \brief Make sure that all elements are in
	   	*  [minValue, maxValue] range:
		*  If element > maxValue then element = maxValue
		*  If element < minValue then element = minValue
		*
		*   Make sure that comparison operators exist for TElement
		*/
	static void Clamp(VectorType& v, TElement minValue, TElement maxValue)
	{
		for(unsigned i=0; i < v.size(); i++)
		{
			if ( v[i] < minValue )
				v[i] = minValue;
			else if ( v[i] > maxValue )
				v[i] = maxValue;
		}
	};

	/** \brief Return min and max pair 
		* (faster than calling min_value and max_value separately)
		*/
	static void minmax_values( const VectorType& v, TElement& min, TElement& max ) 
	{ 
		min = v[0];
		max = v[0];

		for (unsigned i=1; i < v.size(); ++i)
			if (v[i] < min) min = v[i];
			else if (v[i] > max) max = v[i];
	};

	/** \brief Performs linear transformation [min,max] -> [a,b]
		*/
	static void LinearStretchMinMax( VectorType& v, TElement a, TElement b )
	{
		TElement min, max;
		minmax_values(v, min, max);

		double k1 = (max==min) ? 0 : ((b-a)/(max-min));
		double k2 = a - k1 * min;
		for (unsigned i=0; i < v.size(); ++i)
		{
			v[i] = v[i] * k1 + k2; 
		}
	}

	// added by sandres 2004-02-25
	// assignment operator is not inherited from superclass

	//binary stream input/output (at least until we have functional readers/writers)
	static void WriteToStream( const VectorType& v, std::ostream& os )
	{
		unsigned len = v.size();
		os.write( (const char*)&len, sizeof( len ) );
		os.write( (const char*) v.data_block(), len * sizeof( TElement ) );
	};

	static void ReadFromStream( VectorType& v, std::istream& is )
	{
		unsigned len;
		is.read( (char*)&len, sizeof(len));

		v.set_size(len);
		TElement* s = v.data_block();
		is.read( (char*)s, len * sizeof( TElement ) );
	};
};

#endif
