/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginResampleProcessor.h"

#include <string>
#include <iostream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"

#include "itkResampleImageFilter.h"

#include "vtkSmartPointer.h"

SandboxPlugin::ResampleProcessor::ResampleProcessor( )
{
	SetName( "ResampleProcessor" );

	BaseProcessor::SetNumberOfInputs( 2 );
	GetInputPort( INPUT_IMAGE )->SetName( "Input Image" );
	GetInputPort( INPUT_IMAGE )->SetDataEntityType( Core::ImageTypeId );
	GetInputPort( INPUT_POINT )->SetName( "Input Point" );
	GetInputPort( INPUT_POINT )->SetDataEntityType( Core::PointSetTypeId );

	BaseProcessor::SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::ImageTypeId );

	// Create default transform
	m_ScaleTransform = ScaleTransformType::New( );
	ScaleTransformType::ScaleType scale;
	scale[ 0 ] = 0.5;
	scale[ 1 ] = 0.5;
	scale[ 2 ] = 0.5;
	m_ScaleTransform->SetScale( scale );
}

SandboxPlugin::ResampleProcessor::~ResampleProcessor()
{
}

void SandboxPlugin::ResampleProcessor::Update()
{
	// Get the first image
	ImageType::Pointer itkInputImage;
	GetProcessingData( INPUT_IMAGE, itkInputImage);

	Core::vtkPolyDataPtr vtkInputPoint;
	GetProcessingData( INPUT_POINT, vtkInputPoint );

	if ( vtkInputPoint->GetNumberOfPoints() == 0  )
	{
		throw Core::Exceptions::Exception(
			"ResampleProcessor::Update",
			"Please, set an input point" );
	}

	ScaleTransformType::InputPointType point;
	double vtkPoint[ 3 ];
	vtkInputPoint->GetPoint( 0, vtkPoint );
	point[ 0 ] = vtkPoint[ 0 ];
	point[ 1 ] = vtkPoint[ 1 ];
	point[ 2 ] = vtkPoint[ 2 ];
	m_ScaleTransform->SetCenter( point );

	// Call the function 
	typedef itk::ResampleImageFilter<ImageType,ImageType> ResampleImageFilterType;
	ResampleImageFilterType::Pointer filter = ResampleImageFilterType::New( );
	filter->SetInput( itkInputImage );
	filter->SetTransform( m_ScaleTransform );
	filter->SetOutputParametersFromImage( itkInputImage );
	filter->Update( );

	// Set the output to the output of this processor
	std::vector<ImageType::Pointer> outputVector;
	outputVector.push_back( filter->GetOutput() );
	UpdateOutput<ImageType::Pointer>( 
		0,
		outputVector, 
		"ResampleProcessor", 
		true, 
		GetInputDataEntity( 0 ) );	
}

SandboxPlugin::ResampleProcessor::ScaleTransformType::Pointer 
SandboxPlugin::ResampleProcessor::GetScaleTransform() const
{
	return m_ScaleTransform;
}
