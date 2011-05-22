/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginSubtractProcessor.h"

#include <string>
#include <iostream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"

#include "itkSubtractImageFilter.h"

SandboxPlugin::SubtractProcessor::SubtractProcessor( )
{
	SetName( "SubtractProcessor" );

	BaseProcessor::SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input Image" );
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	GetInputPort( 1 )->SetName( "Input Image" );
	GetInputPort( 1 )->SetDataEntityType( Core::ImageTypeId );

	BaseProcessor::SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
}

SandboxPlugin::SubtractProcessor::~SubtractProcessor()
{
}

void SandboxPlugin::SubtractProcessor::Update()
{
	// Get the first image
	ImageType::Pointer itkInputImage;
	GetProcessingData( 0, itkInputImage);
	
	// Get the second image
	ImageType::Pointer itkInputImage2;
	GetProcessingData( 1, itkInputImage2 );

	// Call the function 
	typedef itk::SubtractImageFilter<ImageType> SubtractType;
	SubtractType::Pointer filter = SubtractType::New();
	filter->SetInput1( itkInputImage );
	filter->SetInput2( itkInputImage2 );
	filter->Update();

	ImageType::Pointer itkOutputImage = filter->GetOutput();

	UpdateOutput(  
		0 , 
		filter->GetOutput(), 
		"SubtractProcessor",
		true, 
		1,
		GetInputDataEntity( 0 ) );

}
