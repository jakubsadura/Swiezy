/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkHistogramHelper_TXX
#define wxMitkHistogramHelper_TXX

#include "itkImage.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkScalarImageToHistogramGenerator2.h"

using namespace mitk;

/**
Generates the histogram of the gradient of a mitk::Image.
The histogram of the gradient shows the frequencies of the gradient changes 
between scalars of an image. It internally computes the Magnitude Gradient 
of the image and generates its histogram.
*/
template <typename TPixel, unsigned int VImageDimension>
void wxMitkHistogramHelper::InternalGenerateGradientHistogram(
	itk::Image<TPixel, VImageDimension>* itkImage, 
	wxMitkHistogramHelper::InternalHistogramType::ConstPointer& histogram)
{
	typedef itk::Image<TPixel, VImageDimension> ImageType;
	typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> 
		GradientFilterType;
	typename ImageType::Pointer outputImage;

	// Run an itk gradient magnitude filter
	typename GradientFilterType::Pointer gradientImageFilter;
	gradientImageFilter = GradientFilterType::New();
	gradientImageFilter->SetInput(itkImage);
	gradientImageFilter->Update();
	outputImage = gradientImageFilter->GetOutput();

	// Generate the histogram
	typedef itk::Statistics::ScalarImageToHistogramGenerator2<ImageType> 
		HistogramGeneratorType;
    typename HistogramGeneratorType::Pointer histogramGenerator;
	histogramGenerator = HistogramGeneratorType::New();
    histogramGenerator->SetInput(outputImage);
	histogramGenerator->SetNumberOfBins(100);
	histogramGenerator->Compute();
	histogram = histogramGenerator->GetOutput();
}


template < typename TPixel, unsigned int VImageDimension > 
void wxMitkHistogramHelper::InternalComputeHistogram(itk::Image< TPixel, VImageDimension >* itkImage, 
					 wxMitkHistogramHelper::InternalHistogramType::ConstPointer& histogram)
{
	typedef itk::Statistics::ScalarImageToHistogramGenerator2< itk::Image< TPixel, VImageDimension > >
		HistogramGeneratorType;
	typename HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();

  histogramGenerator->SetInput( itkImage );

  histogramGenerator->SetNumberOfBins( 100 );
//  histogramGenerator->SetMarginalScale( 10.0 );
  histogramGenerator->Compute();
  
  histogram = histogramGenerator->GetOutput();

}


#endif // wxMitkHistogramHelper_TXX


