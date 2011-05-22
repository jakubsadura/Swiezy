/*=========================================================================
This file was copied from EvoApi module in order to
remove dependencies between the otsu segmentation and EvoLib.
Authors: xmellado and xplanes. Date: 2009-04-22$
=========================================================================*/

#ifndef _blOtsuImageFilter_h
#define _blOtsuImageFilter_h

#include <time.h>

//ITK
//#include <itkImage.h>
#include <itkOtsuThresholdImageFilter.h>

using namespace itk;

/**
 * \brief Threshold an image using the Otsu Threshold
 *
 * This filter creates a binary thresholded image that separates an
 * image into foreground and background components. The filter
 * computes the threshold using the OtsuThresholdImageCalculator and
 * applies that threshold to the input image using the
 * BinaryThresholdImageFilter. The NunberOfHistogram bins can be set
 * for the Calculator. The InsideValue and OutsideValue can be set
 * for the BinaryThresholdImageFilter.
 *
 * \ingroup SegmentationAlgorithms
 */

class OtsuImageFilter :
    public LightObject
{
public:
	/** Standard class typedefs. */
	typedef OtsuImageFilter   Self;
	typedef SmartPointer<Self>        Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	/** Method for creation through the object factory */
	itkNewMacro(Self);

	/** float for input image. */
	typedef itk::Image< float, 3 > InputImageType;

	/** Unsigned for output image. */
	typedef itk::Image< unsigned char, 3 > OutputImageType;

	/** The dimension of the input and output images. */
	itkStaticConstMacro(InputImageDimension, unsigned int,
		InputImageType::ImageDimension);
	itkStaticConstMacro(OutputImageDimension, unsigned int,
		OutputImageType::ImageDimension);

	/** Pointer Type for input image. */
	typedef InputImageType::ConstPointer InputImagePointer;

	/** Pointer Type for the output image. */
	typedef OutputImageType::Pointer OutputImagePointer;

	/** Get computation time for last execution. */
	double GetComputationTime( void ){ return this->m_ComputationTime; }

	/** Get segmentation output image. */
	OutputImageType::Pointer GetOutput( void ) { return this->m_Output; }

	/** Get segmentation output threshold. */
	InputImageType::PixelType GetThreshold( void ) { return this->m_Threshold; }

	/** Set segmentation output image. */
	void SetInput( InputImageType::Pointer Input ) { this->m_Input = Input; }

	/** Get segmentation input image. */
	InputImageType::Pointer GetInput( void ) { return this->m_Input; }

	/** Compute Geodesic Active Regions front evolution inside a narrow band. */
	void Update();

	//! Returns the iso value that must be used to calculate the surface from the segmented image.
	double GetIsoValue() const;

	//!
	void SetInsideValue(float _insideValue);

	//!
	void SetOutsideValue(float _outsideValue);

	//!
	void SetNumberOfHistogramBins(unsigned int _numberOfHistogramBins);

	//!
	float GetInsideValue(void);

	//!
	float GetOutsideValue(void);

	//!
	unsigned int GetNumberOfHistogramBins(void);


protected:
	OtsuImageFilter();
	~OtsuImageFilter() {};
	void PrintSelf(std::ostream& os, Indent indent) const;
	
private:
	OtsuImageFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented
	
	double	m_ComputationTime;

	InputImageType::Pointer   m_Input;
	OutputImageType::Pointer  m_Output;
	InputImageType::PixelType m_Threshold;

	float insideValue;
	float outsideValue;
	unsigned int numberOfHistogramBins;
}; 

#endif // _OtsuImageFilter_h
