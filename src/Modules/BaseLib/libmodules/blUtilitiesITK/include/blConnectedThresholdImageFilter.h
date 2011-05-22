/*=========================================================================
This file was copied from EvoAPI in order to remove
dependencies between the threshold segmentation and EvoAPI.
Authors: xmellado and xplanes. Date: 2008-02-01$
=========================================================================*/

#ifndef _blConnectedThresholdImageFilter_h
#define _blConnectedThresholdImageFilter_h

#include <time.h>

//ITK
//#include <itkImage.h>
#include <itkConnectedThresholdImageFilter.h>

using namespace itk;


/**
 * \brief Label pixels that are connected to a seed and lie within a range of values.
 *
 * The blConnectedThresholdImageFilter has two main parameters to be defined. They are the lower
 * and upper thresholds of the interval in which intensity values should fall in order to be included
 * in the region. Setting these two values too close will not allow enough flexibility for the region
 * to grow. Setting them too far apart will result in a region that engulfs the image.
 *
 * The output of this filter is a binary image with zero-value pixels everywhere except on
 * the extracted region. The intensity value set inside the region is selected with the parameter
 * replaceValue.
 *
 * The initialization of the algorithm requires the user to provide a seed point. It is convenient to
 * select this point to be placed in a typical region of the anatomical structure to be segmented.
 * The seed is passed in the form of a itk::Index to the parameter seed.
 *
 * \ingroup SegmentationAlgorithms
 */

class blConnectedThresholdImageFilter :
    public LightObject
{
public:
	/** Standard class typedefs. */
	typedef blConnectedThresholdImageFilter  Self;
	typedef SmartPointer<Self>                     Pointer;
	typedef SmartPointer<const Self>               ConstPointer;

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

	//! 
	double GetLowerThreshold();
	//! 
	double GetUpperThreshold();
	//! 
	unsigned char GetReplaceValue();
	//! 
	InputImageType::IndexType GetSeed();
	//! 
	unsigned int GetNumberOfIterations();
	//! 
	double GetTimeStep();
	//! 
	double GetConductance();
	//! 
	bool GetImageSpacing();
	//! 
	void SetLowerThreshold(double d);
	//! 
	void SetUpperThreshold(double d);
	//! 
	void SetReplaceValue(unsigned char u);
	//! 
	void SetSeed(InputImageType::IndexType index);
	//! 
	void SetNumberOfIterations(unsigned int u);
	//! 
	void SetTimeStep(double d);
	//! 
	void SetConductance(double d);
	//! 
	void SetImageSpacing(bool b);

	/** Get computation time for last execution. */
	double GetComputationTime( void ){ return this->m_ComputationTime; }

	/** Get RealImageType segmentation output image. */
	OutputImageType::Pointer GetOutput( void ) { return this->m_Output; }

	/** Set RealImageType segmentation output image. */
	void SetInput( InputImageType::Pointer Input ) { this->m_Input = Input; }

	/** Get RealImageType segmentation input image. */
	InputImageType::Pointer GetInput( void ) { return this->m_Input; }

	/** Compute Geodesic Active Regions front evolution inside a narrow band. */
	void Update();

	//! Returns the iso value that must be used to calculate the surface from the segmented image.
	double GetIsoValue() const;

protected:
	blConnectedThresholdImageFilter();
	~blConnectedThresholdImageFilter() {};
	void PrintSelf(std::ostream& os, Indent indent) const;
	
private:
	blConnectedThresholdImageFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented
	
	//! Lower threshold pixel value. The default value is 0.
	double m_LowerThreshold;
	//! Upper threshold pixel value. The default value is 0.
	double m_UpperThreshold;
	//! Intensity value in segmented image.
	unsigned char m_ReplaceValue;
	//! Seed point to start growing region algorithm.
	InputImageType::IndexType m_Seed;
	//! Number of iterations for anisotropic curvature smoothing filter. The default value is 5.
	unsigned int m_NumberOfIterations;
	//! Time step for anisotropic curvature smoothing filter. The default value is 0.0125.
	double m_TimeStep;
	//! Conductance for anisotropic curvature smoothing filter. The default value is 1.0.
	double m_Conductance;
	//! Use image spacing when applying anisotropic curvature smoothing filter. The default value is true.
	bool m_UseImageSpacing;
	//!
	double	m_ComputationTime;
	//!
	InputImageType::Pointer   m_Input;
	//!
	OutputImageType::Pointer  m_Output;

}; // end of blConnectedThresholdImageFilter class


#endif // _blConnectedThresholdImageFilter_h
