#ifndef meSKELETONAPITYPES_H
#define meSKELETONAPITYPES_H

// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

//---------------------------------------------------
// HEADERS
//---------------------------------------------------
#include <vector>
#include <itkImage.h>
#include <itkImageRegionIterator.h>
#include <itkNeighborhoodIterator.h>


//---------------------------------------------------
// TYPES
//---------------------------------------------------

namespace SkeletonAPITypes
{
	/**
	 * \brief Type of Pixel
	 * \ingroup SkeletonAPITypes
	 */
	typedef float RealPixelType;
	typedef RealPixelType InputPixelType;
	typedef unsigned char OutputPixelType;

	const int kDIM_3D = 3;

	/**
	 * \brief internal image type 
	 * \ingroup SkeletonAPITypes
	 */
	typedef itk::Image<InputPixelType, kDIM_3D>			InputImageType;
	typedef itk::Image<OutputPixelType, kDIM_3D>		OutputImageType;
	typedef InputImageType::Pointer						InputImagePointer;
	typedef OutputImageType::Pointer					OutputImagePointer;
	typedef itk::ImageRegionIterator<InputImageType >	InputRegionIteratorType;
	typedef itk::ImageRegionIterator<OutputImageType >  OutputRegionIteratorType;
	typedef itk::NeighborhoodIterator<OutputImageType > OutputNeighborhoodIteratorType;
	
}


#endif // SKELETONAPITYPES_H
