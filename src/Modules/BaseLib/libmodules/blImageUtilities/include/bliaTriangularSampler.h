/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIATriangularSampler_h
#define __blIATriangularSampler_h

//-----------------------------------------------
// Our filex
#include "blPDShape.h"	// GetNormals
#include "blIAMaskedSampler.h"

//#include "blIADefines.h"
//#include "blImage.h"
#include "blImageMask.h"
#include "blDelaunay2D.h"
#include "blMath.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

//-----------------------------------------------
// std files
//#include <stdlib.h>
//#include <iostream.h>

//-----------------------------------------------
// ITK dependencies
//#include <itkImage.h>
#include <vnl/vnl_vector.h>
#include <blIAROI.h>

//-----------------------------------------------
/**
 * \brief Triangular sampler
 * \ingroup blImageUtilities
 *
 *	Extracts the smallest image fragment containing specified triangle
 *  and sets image mask: 0 for pixels outside triangle, 1 for pixels inside 
 *  trianlge
 *	
 *  Careful: SampledImage = OriginalImage. If you want to change sampled 
 *  image make a copy
 *  Other possibility is to call Update(true). See Update() for details
 *
 *  First call 
 * \code
 *	SetInputImage()	
 *  SetTriangleVertices()
 *	Update()
 * \endcode
 *
 *  Additional stuff:
 *
 *	   you can use Set/GetImageMask() to get the mask where this triangle is marked 
 *		(e.g. everything otside the triangle is masked)
 *
 *	   if you assign the mask - it is your duty to fill it with zeroes wherever you 
 *     need. This allows for some blexibility:
 *			You can define several triangles on the same mask, you can also use 
 *			SetMaskValue() to assign different values to the pixels in the mask
 *			for each triangle. 
 *	   
 *	Between sequentual calls to this class mask is preserved (so it allows the 
 *  above multiple triangle thing again).
 *	If you don't like this pehaviour - you can use ClearMask() after each call to 
 *  Update()
 */

//TPixel - pixeltype of the image
template<class TPixel>
class blIATriangularSampler : public blIAMaskedSampler< itk::Image<TPixel, 2>, unsigned short, 2 >
{
	public:
		typedef blIATriangularSampler Self;
		typedef blSmartPointer<Self> Pointer;
		typedef blIAMaskedSampler< itk::Image<TPixel, 2>, unsigned short, 2 > Superclass;

		typedef typename Superclass::MaskPixelType MaskPixelType; //16 bits - should be enough
		typedef typename Superclass::MaskType MaskType;
		typedef typename Superclass::MaskIteratorTypeWithIndex MaskIteratorTypeWithIndex;
		typedef typename Superclass::MaskIteratorType MaskIteratorType;

		typedef itk::Image<TPixel, 2> ImageType;
		typedef blIAROI<ImageType, ImageType> ROIFilter;


		blNewMacro(Self);

		
		/** \brief Set triangle for sampling
		*  
		*/
		void SetTriangleVertices(const ContinuousPoint2D* triangleVertices);

		/** \brief Perform sampling
		*  If createNewImage=true then _sampledImage is created and its size 
		*  equals to the size of the bounding box around the sampled area
		* 
		*  !!! If mask was never assigned to the class, it will be created
		*  !!! if mask was assigned, it is supposed that user will fill it with 
		*  !!! zeroes before or inbetween samplings if he needs it
		*/
		void Update(bool createNewImage = false);

		void ClearMask()
		{
			this->_imageMask.Delete();
		}

	protected:
	
		/** \brief Check if the pooint is inside the triangle
		*  
		*/
		bool IsPointInsideTriangle(const typename MaskType::IndexType& index); 

		blIATriangularSampler();
		virtual ~blIATriangularSampler();
	

	private:
		blIATriangularSampler(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		//vnlMatrixDouble _triangleVertices; //!< Storage for triangle vertices. Vertices in columns
		ContinuousPoint2D _vertex[3]; //!< Storage for triangle vertices. 

		double _param[3][3]; //precalculated parameters to check if point is inside the triangle
};

#include "bliaTriangularSampler.txx"

#endif


