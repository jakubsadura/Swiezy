/*=========================================================================
This file was copied from EvoAPI in order to remove
dependencies between the threshold segmentation and EvoAPI.
Authors: xmellado and xplanes. Date: 2008-02-01$
=========================================================================*/

#include "blConnectedThresholdImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"

/**
 *    Constructor
 */
blConnectedThresholdImageFilter
::blConnectedThresholdImageFilter()
{
	this->m_ComputationTime         = 0.0;
}


/**
 *  Compute connected threshold segmentation
 */
void
blConnectedThresholdImageFilter
::Update()
{
	clock_t start, finish;
	start = clock();

	typedef itk::CurvatureAnisotropicDiffusionImageFilter< InputImageType, InputImageType >  SmoothFilterType;

	SmoothFilterType::Pointer smoothing = SmoothFilterType::New();
	smoothing->SetInput( this->m_Input );
	smoothing->SetNumberOfIterations( GetNumberOfIterations() );
	smoothing->SetTimeStep( GetTimeStep() );
	smoothing->SetConductanceParameter( GetConductance() );
	if ( GetImageSpacing() )
	{
		smoothing->UseImageSpacingOn();
	}
	else
	{
		smoothing->UseImageSpacingOff();
	}
	smoothing->Update();

	typedef itk::ConnectedThresholdImageFilter< InputImageType, OutputImageType > ConnectedFilterType;
	ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
	connectedThreshold->SetInput( smoothing->GetOutput() );
	connectedThreshold->SetLower( GetLowerThreshold() );
	connectedThreshold->SetUpper( GetUpperThreshold() );
	connectedThreshold->SetReplaceValue( GetReplaceValue() );
	//InputImageType::IndexType  index;
	//index[0] = 23;
	//index[1] = 41;
	//index[2] = 25;
	//this->m_ConnectedThresholdParameters->PrintSelf( std::cout, 2 );
	connectedThreshold->SetSeed( GetSeed() );

	try
	{
		connectedThreshold->Update();
	}
	catch ( itk::ExceptionObject &err )
	{
		throw err;
	}
	catch (...)
	{
		itkExceptionMacro( << "blConnectedThresholdImageFilter::Update() - Unexpected error!" << std::endl );
	}

	finish = clock();
	this->m_ComputationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;

	this->m_Output = connectedThreshold->GetOutput();
} // end Update()

/**
 *  Print Self
 */
void 
blConnectedThresholdImageFilter::
PrintSelf(std::ostream& os, Indent indent) const
{
//	this->m_OtsuParameters->PrintSelf( os, indent );
	os << indent << "Computation Time  : " << this->m_ComputationTime << std::endl;
}

double blConnectedThresholdImageFilter::GetIsoValue() const
{
	return 0.5;
}


//! 
double blConnectedThresholdImageFilter::GetLowerThreshold()
{
	return m_LowerThreshold;
}
//! 
double blConnectedThresholdImageFilter::GetUpperThreshold()
{
	return m_UpperThreshold;
}
//! 
unsigned char blConnectedThresholdImageFilter::GetReplaceValue()
{
	return m_ReplaceValue;
}
//! 
blConnectedThresholdImageFilter::InputImageType::IndexType blConnectedThresholdImageFilter::GetSeed()
{
	return m_Seed;
}
//! 
unsigned int blConnectedThresholdImageFilter::GetNumberOfIterations()
{
	return m_NumberOfIterations;
}
//! 
double blConnectedThresholdImageFilter::GetTimeStep()
{
	return m_TimeStep;
}
//! 
double blConnectedThresholdImageFilter::GetConductance()
{
	return m_Conductance;
}
//! 
bool blConnectedThresholdImageFilter::GetImageSpacing()
{
	return m_UseImageSpacing;
}
//! 
void blConnectedThresholdImageFilter::SetLowerThreshold(double d)
{	
	m_LowerThreshold = d;
}
//! 
void blConnectedThresholdImageFilter::SetUpperThreshold(double d)
{
	m_UpperThreshold = d;
}
//! 
void blConnectedThresholdImageFilter::SetReplaceValue(unsigned char u)
{
	m_ReplaceValue = u;
}
//! 
void blConnectedThresholdImageFilter::SetSeed(InputImageType::IndexType index)
{
	m_Seed = index;
}
//! 
void blConnectedThresholdImageFilter::SetNumberOfIterations(unsigned int u)
{
	m_NumberOfIterations = u;
}
//! 
void blConnectedThresholdImageFilter::SetTimeStep(double d)
{
	m_TimeStep = d;
}
//! 
void blConnectedThresholdImageFilter::SetConductance(double d)
{
	m_Conductance = d;
}
//! 
void blConnectedThresholdImageFilter::SetImageSpacing(bool b)
{
	m_UseImageSpacing = b;
}



