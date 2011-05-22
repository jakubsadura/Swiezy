/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAMaskedSampler_h
#define __blIAMaskedSampler_h

//-----------------------------------------------
// Our filex
//#include "blIASampler.h"

//#include "blIADefines.h"
//#include "blImage.h"
#include "blImageMask.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

//-----------------------------------------------
// ITK dependencies
#include <itkImageBase.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionIteratorWithIndex.h>

//-----------------------------------------------
/**
 * \brief Masked sampler
 * \ingroup blImageUtilities
 *
 *	Basic class for all samplers that use mask to define sampled area
 *  Pixels outside sampled area have 0 in the mask and pixels inside are >0
 *  Specifically 1 if SetMaskValue was never called. Otherwise it would be any desired 
 *  value
 *
 *  TPixel is the pixeltype for mask
 *	
 */
template <class TImage, class TPixel, unsigned int VImageDimension=2>
class blIAMaskedSampler : public blLightObject
{
	public:
		typedef blIAMaskedSampler Self;
		typedef blSmartPointer<Self> Pointer;

		typedef TPixel MaskPixelType; //16 bits - should be enough
		typedef blImageMask<MaskPixelType, VImageDimension> MaskType;
		typedef itk::ImageRegionIteratorWithIndex< MaskType > MaskIteratorTypeWithIndex;
		typedef itk::ImageRegionIterator< MaskType > MaskIteratorType;

		typedef TImage ImageType;

		blNewMacro(Self);

		/** \brief Return sampled fragment with triangular mask
		*  
		*/
		typename ImageType::Pointer GetOutput() const
			{ return _sampledImage; };

		/** \brief Get image mask 
		*   
		*   If mask(i,j)>0 - corresponding pixel is in triangle
		* 
		*/
		typename MaskType::Pointer GetImageMask() const
			{ return _imageMask; };

		/** \brief Set image mask
		*    needed in case you don't want mask to be recreated 
		*/
		void SetImageMask( MaskType* newMask ) 
			{ _imageMask = newMask; };


		/** \brief Set value for unmasked pixel
		*    needed in case you don't want mask to be recreated 
		*    Thus mask can have several nonintersecting regions, and 
		*    within each region.
		*    Default for this class (if this function was never called) 
		*    is valueNonMasked=1
		*    0 corresponds to masked pixels
		*/
		void SetMaskValue( MaskPixelType value ) 
		{ 
			_nonMaskedPixelValue = value; 
		};


		/** \brief Set/Get bounding box of the sampled area
		*/
		void SetRegion(typename ImageType::RegionType & region) 
                     { _boundingRegion = region; }
		const typename ImageType::RegionType& GetRegion() const {return _boundingRegion; };

		/** \brief Set input image
		*/
		void SetInputImage( ImageType* image )
		{
			_originalImage = image;
		};

	protected:
	
		blIAMaskedSampler();
		virtual ~blIAMaskedSampler();
	

		MaskPixelType _nonMaskedPixelValue; //!<value for nonmasked pixels in the mask

                typename ImageType::Pointer _originalImage;
		typename ImageType::Pointer _sampledImage;
		typename MaskType::Pointer _imageMask;

		typename ImageType::RegionType _boundingRegion; //!< bounding box of the sampled area

	private:
		blIAMaskedSampler(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented
};

#include "blIAMaskedSampler.txx"

#endif


