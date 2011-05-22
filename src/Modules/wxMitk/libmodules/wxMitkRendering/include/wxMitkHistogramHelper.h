/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkHistogramHelper_H
#define wxMitkHistogramHelper_H

#include "wxMitkRenderingWin32Header.h"
#include <mitkHistogramGenerator.h>
#include "itkHistogram2.h"

namespace mitk
{

/**
This class provides extra helper functions for managing Histograms.

\ingroup wxMitkCore
\author Juan Antonio Moya
\date 10 Dec 2007
*/
class WXMITKRENDERING_EXPORT wxMitkHistogramHelper
{
public:
	typedef itk::Statistics::Histogram2<double> InternalHistogramType;

	/**
	\brief Generates the histogram of the gradient of a mitk::Image.
	The histogram of the gradient shows the frequencies of the gradient 
	changes between scalars of an image. It internally computes the 
	Magnitude Gradient of the image and generates its histogram.
	*/
	static mitk::HistogramGenerator::HistogramType::ConstPointer 
		GenerateGradientHistogram(mitk::Image::Pointer image);


	/**
	*/
	static mitk::HistogramGenerator::HistogramType::ConstPointer ComputeHistogram( 
		const mitk::Image * image );

private:

	//! Internal function for computation
	template <typename TPixel, unsigned int VImageDimension>
		static void InternalGenerateGradientHistogram(
			itk::Image<TPixel, VImageDimension>* itkImage, 
			InternalHistogramType::ConstPointer& histogram);

	//! Internal function for computation
	template < typename TPixel, unsigned int VImageDimension > 
		static void InternalComputeHistogram(
			itk::Image< TPixel, VImageDimension >* itkImage, 
			wxMitkHistogramHelper::InternalHistogramType::ConstPointer& histogram);

};
 
}

// Include the templated members
#include "wxMitkHistogramHelper.txx"

#endif // wxMitkHistogramHelper_H
