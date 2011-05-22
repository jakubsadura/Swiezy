/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreChangeOrientationProcessor.h"
#include "coreDataEntityHelper.h"

#include "vtkImageReslice.h"


Core::ChangeOrientationProcessor::ChangeOrientationProcessor( )
{
	SetName( "ChangeOrientation" );

	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input volume" );
	GetInputPort( 0 )->SetDataEntityType( 
		Core::DataEntityType( Core::ImageTypeId | Core::ROITypeId ) );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Oriented image" );
	GetOutputPort( 0 )->SetDataEntityName( "Output image" );
}

void Core::ChangeOrientationProcessor::Update()
{
	try
	{
		vtkImageDataPtr inputImage;
		GetProcessingData( 0, inputImage );

		vtkSmartPointer<vtkImageReslice> imageReslice;
		imageReslice = vtkSmartPointer<vtkImageReslice>::New();
		imageReslice->SetInput( inputImage );

		double axes[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
		if ( m_FlipLeftRight )
		{
			axes[ 0 ][ 0 ] = -1;
		}
		if ( m_FlipUpDown )
		{
			axes[ 2 ][ 2 ] = -1;
		}
		if ( m_InvertSliceOrder )
		{
			axes[ 1 ][ 1 ] = -1;
		}
		imageReslice->SetResliceAxesDirectionCosines(axes[0], axes[1], axes[2]);

		double inOrigin[3];
		inputImage->GetOrigin( inOrigin );

		imageReslice->Update();

		vtkImageDataPtr outputImage = imageReslice->GetOutput();
		outputImage->SetOrigin( inOrigin );


		UpdateOutput( 0, outputImage, "Oriented image", true, 1, GetInputDataEntity( 0 ) );

	}
	coreCatchExceptionsAddTraceAndThrowMacro( ChangeOrientationProcessor::Update )
}

bool Core::ChangeOrientationProcessor::GetInvertSliceOrder() const
{
	return m_InvertSliceOrder;
}

void Core::ChangeOrientationProcessor::SetInvertSliceOrder( bool val )
{
	m_InvertSliceOrder = val;
}

bool Core::ChangeOrientationProcessor::GetFlipUpDown() const
{
	return m_FlipUpDown;
}

void Core::ChangeOrientationProcessor::SetFlipUpDown( bool val )
{
	m_FlipUpDown = val;
}

bool Core::ChangeOrientationProcessor::GetFlipLeftRight() const
{
	return m_FlipLeftRight;
}

void Core::ChangeOrientationProcessor::SetFlipLeftRight( bool val )
{
	m_FlipLeftRight = val;
}
