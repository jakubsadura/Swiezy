/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blITKImageUtils_h
#define _blITKImageUtils_h

/**
* \brief Image utilities
* \ingroup blITKImageUtils
* \date 18 Dec 2008
* \author Xavi Planes
*/
namespace blITKImageUtils
{

	/**\brief saves an itk image of any type to file 
	* \param paramImage input image
	* \param paramFileName file name
	*/
	template <class TInputImage>
	void SaveImageToFile( 
		const typename TInputImage::Pointer paramImage, 
		const char* paramFileName);

	/**\brief Loads an itk image of any type to file 
	* \param paramFileName file name
	*/
	template <class TInputImage>
	typename TInputImage::Pointer LoadImageFromFile( const char* paramFileName);

	/**\brief Compare two ITK images.
	* \param image1 First image.
	* \param image2 Second image.
	* \param tolerance Tolerance on parameters (size, spacing).
	* \param toleranceValue Tolerance on pixel values (mean error as percent of range, default to 0).
	*/
	template< typename TInputImage >
	bool CompareImages( 
	   const typename TInputImage::Pointer image1, 
	   const typename TInputImage::Pointer image2,
	   double tolerance, 
	   double toleranceValue = 0. );

	/** Flip 2D or 3D itk image in specific directions (this method is used 
	when we read dicom images by gdcm library)
	*/
	template < typename ImageType > 
	typename ImageType::Pointer FlipItkImage( 
		const ImageType* itkImage );

	//! use dicom orientation to apply proper orientation to image
	template < typename ImageType > 
	typename ImageType::Pointer ApplyOrientationToImage( 
		typename ImageType::Pointer itkImage );

	//!
	template < typename ThreeDInputImageType, typename TwoDOutputImageType > 
	typename TwoDOutputImageType::Pointer ExtractSliceFromThreeDItkImage( 
		typename ThreeDInputImageType::Pointer threeDInputImage,
		int sliceNr );

};

#include "blITKImageUtils.txx"

#endif // _blITKImageUtils_h
