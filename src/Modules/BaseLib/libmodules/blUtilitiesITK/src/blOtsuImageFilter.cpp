/*=========================================================================
This file was copied from EvoApi module in order to
remove dependencies between the otsu segmentation and EvoLib.
Authors: xmellado and xplanes. Date: 2009-04-22$
=========================================================================*/


#include "blOtsuImageFilter.h"


/**
 *    Constructor
 */
OtsuImageFilter
::OtsuImageFilter()
{
	this->m_ComputationTime         = 0.0;
	this->insideValue = 1;
	this->outsideValue = 0;
	this->numberOfHistogramBins = 128;
}

/**
 *  Compute Otsu segmentation
 */
void
OtsuImageFilter
::Update()
{
	clock_t start, finish;
	start = clock();

	typedef itk::OtsuThresholdImageFilter< InputImageType, OutputImageType >  OtsuFilterType;

	OtsuFilterType::Pointer filter = OtsuFilterType::New();
	filter->SetInput( this->m_Input );
	filter->SetOutsideValue( this->outsideValue );
	filter->SetInsideValue( this->insideValue );
	filter->SetNumberOfHistogramBins( this->numberOfHistogramBins );
	try
	{
		filter->Update();
	}
	catch ( itk::ExceptionObject &err )
	{
		throw err;
	}
	catch (...)
	{
		itkExceptionMacro( << "evoOtsuImageFilterClass::Update() - Unexpected error!" << std::endl );
	}

	this->m_Threshold = filter->GetThreshold();

	finish = clock();
	this->m_ComputationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;


	this->m_Output = filter->GetOutput();
} // end Update()

/**
 *  Print Self
 */
void 
OtsuImageFilter::
PrintSelf(std::ostream& os, Indent indent) const
{
	os << indent << "Computation Time  : " << this->m_ComputationTime << std::endl;
}

double OtsuImageFilter::GetIsoValue() const
{
	return 0.5;
}


//!
void OtsuImageFilter::SetInsideValue(float _insideValue)
{
	insideValue = _insideValue;
}

//!
void OtsuImageFilter::SetOutsideValue(float _outsideValue)
{
	outsideValue = _outsideValue;
}

//!
void OtsuImageFilter::SetNumberOfHistogramBins(unsigned int _numberOfHistogramBins)
{
	numberOfHistogramBins = _numberOfHistogramBins;
}

//!
float OtsuImageFilter::GetInsideValue(void)
{
	return insideValue;
}

//!
float OtsuImageFilter::GetOutsideValue(void)
{
	return outsideValue;
}

//!
unsigned int OtsuImageFilter::GetNumberOfHistogramBins(void)
{
	return numberOfHistogramBins;
}

