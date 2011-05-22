/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blNeighborhoodImageProduct_h
#define __blNeighborhoodImageProduct_h


#include <vnl/vnl_vector.h>

#include <itkConstNeighborhoodIterator.h>
#include <itkConstantBoundaryCondition.h>
#include "blIAGaussianWindow.h"

#include "blLightObject.h"
#include "blMacro.h"
#include "blSmartPointer.h"

//#define DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT



/**
 * \brief Calculates the pixel-wise product between the image and the mask.
 * \ingroup blImageUtilities
 *
 *	This class multiplies each pixel of the mask 
 *	by the corresponding pixel in the image. Then, it returns all the resulting values
 *	(= size of the mask)
 *	
 *	The mask is centred on a given pixel index in the image.
 *	
 *	The image and the mask must have the same Dimension.
 *
 */
template <class TImage, class TNeighborhoodMask>
class blNeighborhoodImageProduct: public blLightObject
{
	public:

		typedef blNeighborhoodImageProduct Self;
		typedef blSmartPointer<Self> Pointer;		

		typedef TImage ImageType;
		typedef typename ImageType::PixelType PixelType;
		typedef typename ImageType::IndexType IndexType;

		typedef TNeighborhoodMask NeighborhoodMaskType;


		blNewMacro(Self);

		/**\brief returns the output values*/
		const vnl_vector<double> & GetOutput()
		{
			return this->outputVector;
		};

		/**\brief sets the central pixel index */
		void SetCentralPixel(const IndexType & pixelIndex)
		{
			this->index = pixelIndex;
		};

		/**\brief sets image from which pixels are read */
		void SetInputImage(ImageType * image)
		{
			this->inputImage = image;
		};

		/**\brief sets the mask with values*/
		void SetNeighborhoodMask(NeighborhoodMaskType * neighborhoodMask);
		
		/**\brief calculates all resulting values for each position*/
		void Update();


	protected:		

		/**\brief constructor*/
		blNeighborhoodImageProduct();

	
		/**\brief destructor*/
		virtual ~blNeighborhoodImageProduct();

	private:
		blNeighborhoodImageProduct(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		typedef itk::ConstantBoundaryCondition<TImage> ConstantBoundaryConditionType;
		typedef itk::ConstNeighborhoodIterator<ImageType, 
							ConstantBoundaryConditionType> NeighborhoodIteratorType;


		NeighborhoodIteratorType neighborhoodIterator;

		typename ImageType::Pointer inputImage;
		IndexType index;

		typename NeighborhoodMaskType::Pointer neighborhoodMask;

		vnl_vector<double> outputVector;

		

};

#include "blNeighborhoodImageProduct.txx"

#endif


