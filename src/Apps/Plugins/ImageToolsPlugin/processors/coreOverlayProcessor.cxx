/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreOverlayProcessor.h"
#include "coreDataEntityHelper.h"

Core::OverlayProcessor::OverlayProcessor( )
{
	SetName( "Overlay Images" );

	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input image" );
	GetInputPort( 0 )->SetDataEntityType( 
		Core::DataEntityType( Core::ImageTypeId | Core::ROITypeId ) );
	GetInputPort( 1 )->SetName( "Overlay image" );
	GetInputPort( 1 )->SetDataEntityType( 
		Core::DataEntityType( Core::ImageTypeId | Core::ROITypeId ) );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Overlayed image" );
	GetOutputPort( 0 )->SetDataEntityName( "Overlayed image" );
}

void Core::OverlayProcessor::Update()
{
	try
	{
		vtkImageDataPtr inputImage1;
		GetProcessingData( 0, inputImage1 );

		vtkImageDataPtr inputImage2;
		GetProcessingData( 1, inputImage2 );

		int dims1[ 3 ];
		inputImage1->GetDimensions( dims1 );
		int dims2[ 3 ];
		inputImage2->GetDimensions( dims2 );
		bool sameDimensions = true;
		for ( int i = 0 ; i < 3 ; i++ )
		{
			if ( dims1[ i ] != dims2[ i ] )
			{
				sameDimensions = false;
			}
		}
		if ( !sameDimensions )
		{
			throw Core::Exceptions::Exception(
				"OverlayProcessor::Update",
				"Cannot overlay images of different dimensions" );
		}

		inputImage2->SetOrigin( inputImage1->GetOrigin( ) );
		inputImage2->SetSpacing( inputImage1->GetSpacing( ) );
		//inputImage2->SetDirection( inputImage1->GetDirection( ) );

		// Update the output subject
		GetInputDataEntity( 1 )->Modified( );
		GetOutputDataEntityHolder( 0 )->SetSubject( GetInputDataEntity( 1 ) );

	}
	coreCatchExceptionsAddTraceAndThrowMacro( OverlayProcessor::Update )
}

