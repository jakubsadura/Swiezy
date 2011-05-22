/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blImageFunctions_h
#define _blImageFunctions_h


#include <itkImage.h>

/**
 * \brief Paint a line in an image
 * \ingroup blImage 
 * If you get the error "Could not deduce template argument"
 * You hace to tell the compiler the type of TInputImage
 * Exaple: Line< InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
 */
template <class TInputImage>
void Line( typename TInputImage::Pointer, 
		   const typename TInputImage::IndexType&, 
		   const typename TInputImage::IndexType&, 
		   const typename TInputImage::PixelType );

/**
 * \brief Paint a line in an image
 * \ingroup blImage 
 * If you get the error "Could not deduce template argument"
 * You hace to tell the compiler the type of TInputImage
 * Exaple: Line< InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
 */
template <class TInputImage>
void Line( typename TInputImage::Pointer, 
		   const typename TInputImage::IndexType&, 
		   const typename TInputImage::IndexType&, 
		   const typename TInputImage::PixelType,
		   const int iSize );

template <class TInputImage>
void ZeroImage( typename TInputImage::Pointer );

#include "blImageFunctions.txx"

#endif
