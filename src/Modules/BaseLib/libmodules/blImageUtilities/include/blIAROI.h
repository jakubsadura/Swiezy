/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIAROI_h
#define _blIAROI_h

// own includes
//-------------------------------------
//#include "blDefines.h"
//#include "blIADefines.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

// itk includes
//-------------------------------------
#include "itkRegionOfInterestImageFilter.h"
//#include "itkSmartPointer.h"


/**
 * \brief This class implements the Region Of Interest for images.
 * \ingroup blImageUtilities
 *
 *  There is no new functionality to add. So it only wraps itk filter.
 *
 */
template <class TInputImage, class TOutputImage>
class blIAROI : public itk::RegionOfInterestImageFilter<TInputImage,TOutputImage>
{
	public:


		// Typedefs
		//-----------------------------------------------------

		typedef blIAROI Self;
		typedef blSmartPointer<Self> Pointer;

		typedef itk::RegionOfInterestImageFilter<TInputImage, TOutputImage> Superclass;


		blNewMacro(Self);

		
		/** \brief Get the image belonging to the ROI. The rest is discarded
		 *
		 *	Region will always have squared shape
		 */
		TOutputImage * GetOutput();

		/** \brief Set region size (real coordinates)
		 *
		 *	Region will always have squared shape
		 */
		void SetRegionSize(float * size);

		/** \brief Set region start
		 *
		 *	("low-left" corner, real coordinates)
		 */
		void SetRegionStart(float * start);

		/** \brief Set image where ROI will be applied
		 *
		 *	Region will always have squared shape
		 */
		void SetInput(TInputImage * image);


	protected:
		blIAROI();
		virtual ~blIAROI();

	private:
		blIAROI(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented	

		typename TOutputImage::IndexType start; 
		typename TOutputImage::SizeType size;

};

//
// this typedef should be removed from here...
///typedef blIAROI<ImageType, ImageType> ROIFilter;


#include "blIAROI.txx"
#endif


