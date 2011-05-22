/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blImageMask_h
#define __blImageMask_h

//----------------------------
// own dependencies
//#include "blIADefines.h"
//#include "blImage.h"
#include "blImageMask.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include <itkImage.h>


/**
 * \brief Implements a Mask 
 * \ingroup blImageUtilities
 *   Useful properties:
 *		-	possibility to store bounding box that contains non masked 
 *			pixels
 *  
 */
template <class TPixel, unsigned int VImageDimension=2>
class blImageMask: public itk::Image<TPixel, VImageDimension>
/** 
 * To iterate over the mask use GetMask to get image and make 
 * ImageRegionIterator from it and not from this class.
 * As for types - use ones defined here:
 *    MaskIteratorTypeWithIndex
 *    MaskIteratorType
 */
{
	public:
		typedef blImageMask Self;
		typedef blSmartPointer<Self> Pointer;
		typedef blSmartPointer<const Self> ConstPointer;
		typedef itk::Image<TPixel, VImageDimension> Superclass;

		typedef typename Superclass::RegionType RegionType; 
		typedef typename Superclass::SizeType SizeType;
		typedef typename Superclass::IndexType IndexType;

		blNewMacro(Self);
	
		/** \brief Returns bounding box of non masked pixels if it was set
		 *   by SetMask. If return is NULL then it is equal to 
		 *	LargestPossibleRegion()
		 *
		 *  Maybe in the future we'll make a function to calculate bounding box.
		 *  For now it is up to external functions
		 */
		const RegionType* GetMaskRegion() const
			{ return _maskRegion; };

		void SetMaskRegion(const RegionType& region)
		{
			if ( _maskRegion == NULL ) _maskRegion = new RegionType;
			*_maskRegion = region;
		};

		/** \brief Initialize mask to 0. Much faster than SetBuffer 
		 * If not zeroWholeMask then only _maskRegion is zeroed
		 * if _maskRegion==null then whole mask is zeroed
		 *
		 * !!!!! Zeroing inside the region implemented (and optimized) for 
		 * !!!!! two dimensions only, for now !!!!
		 */
		void ZeroMask( bool zeroWholeMask = true ); 

	protected:
		blImageMask();
		virtual ~blImageMask();

	private:
		blImageMask(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented
	
		RegionType* _maskRegion; //!< bounding box around non masked pixels
};

#include "blImageMask.txx"

#endif
