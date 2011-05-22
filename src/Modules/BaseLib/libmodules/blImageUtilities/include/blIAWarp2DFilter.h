/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAWarp2DFilter_h
#define __blIAWarp2DFilter_h
#include "blMacro.h"

//#include <iostream>

#include <vnl/vnl_vector.h>

#include <itkImageRegionIterator.h>
#include <itkLinearInterpolateImageFunction.h>

//#include "blImage.h"
#include "blPDShape2D.h"
#include "blIAMaskedSampler.h"
#include "bliaTriangularSampler.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
//#include "blDefines.h"


//-----------------------------------------------
/**
 * \brief Image warping class
 * \ingroup blImageUtilities
 *
 * \note USE MULTIBAND IMAGE
 *
 *	Pass to the class input blImage, TesselationCells (for now only 2D)	
 *  also output blPDShape, the result will be warped blImage.
 *  Warping is based on Delaunay tesselation. 
 *  Prior tesselation is optional. shapeOut will be tesselated anyway
 *
 *  Can be parallelized and each triangle mapped independently.
 *
 *	Usage:
 * \code 
 *		SetInputParameters()
 *		SetOutputParameters()
 *		Update()
 * \endcode 
 *
 *	If you have several input parameters and output parameters never 
 *  change, then it is better to do the following
 *
 * \code 
 *		SetOutputParameters()
 *		Precalculate()
 *
 *		....your code....
 *
 *		for( i )
 *		{
 *			SetInputParameters()
 *			Update()
 *		}
 * \endcode 
 *
 *	Precalculate can also be used if you want to split things in initialization 
 *  and calculation
 *
 *  Funcion GetMask can give you the mask that can be uset to get pixels from the 
 *  corresponding image. In the mask every element that is larger than 0
 *  is a number of the triangle, the position in the TesselationCells of the shapes
 *
 */
template <class TImage>
class blIAWarp2DFilter: public blLightObject 
{
	public:
		typedef blIAWarp2DFilter Self;
		typedef blSmartPointer<Self> Pointer;

		typedef TImage ImageType;
        typedef typename ImageType::PixelType ImagePixelType;
        typedef blIATriangularSampler<ImagePixelType> TriangularSamplerType;
		typedef typename TriangularSamplerType::MaskType MaskType;
		typedef typename TriangularSamplerType::MaskIteratorTypeWithIndex MaskIteratorTypeWithIndex;

		typedef itk::ImageRegionIterator<typename ImageType::ImageType> IteratorType;

		typedef itk::LinearInterpolateImageFunction<typename ImageType::ImageType> LinearInterpolatorType;
		typedef typename LinearInterpolatorType::Pointer LinearInterpolatorTypePointer;

		blNewMacro(Self);

		/** \brief Set parameters
		 *	image with mesh in shapeIn is warped to mesh from shapeOut 
		 */
		void SetInputParameters(	ImageType* image, 
									blPDShape2D* shapeIn )
		{
			_originalImage	= image;
			_shapeIn		= shapeIn;
		};

		void SetOutputParameters(	ImageType* image,
									blPDShape2D* shapeOut )
		{
			_warpedImage	= image;
			_shapeOut		= shapeOut;
			_precalculated  = false;
		};




		/** \brief Precalculate some thngs that depend only on output shape shapeOut
		 *  because in iterative pprocessing all the images are warped onto shapeOut
		 *	but shapeOut never changes.
		 *  
		 *  If you don't call it explictly, it will be called from Update()
		 */
		void Precalculate();

		/** \brief Warp!
		 *
		 */
		void Update();

		
		/** \brief Get output
		 *	returns blImage
		 */
		typename ImageType::Pointer GetOutput() const { return _warpedImage; };
		
		/** \brief Get used mask
		 *	Mask contains face partitioned into triangles, pixels corresponding 
		 *  to one triangle have the same value, which equals to the position of 
		 *  triangle in the tesselation array + 1 (because 0 is for masked pixels)
		 */
		typename MaskType::Pointer GetMask() const {return _mask; };

	protected:
		/** \brief Constructor and destructor
		 *	
		 */
		blIAWarp2DFilter();
		virtual ~blIAWarp2DFilter();

	private:
		blIAWarp2DFilter(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented
		
		//!< Image storage
		typename ImageType::Pointer _originalImage; 
		typename ImageType::Pointer _warpedImage; 

		//!< shape storage
		typename blPDShape2D::Pointer _shapeIn; 
		typename blPDShape2D::Pointer _shapeOut;

		typename MaskType::Pointer _mask; //!<mask

		//!<precalculation stuff
		int										_NumberOfDimensions;
		bool									_precalculated;
		vnlVectorDouble							_landmarksOutRaw;
		const typename blPDShape2D::ShapeCellsContainer* _triangles;
		typename TriangularSamplerType::Pointer	_sampler;
		LinearInterpolatorTypePointer			_interpolator;
		int										_NumberOfTriangles;

		typedef double TRIPLE_OF_DOUBLES [3];
		TRIPLE_OF_DOUBLES *_A, *_B, *_C;
		double *_determinant; //determinat of the system of equatons 

};


#include "blIAWarp2DFilter.txx"

#endif


