/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVolumeClippingProcessor.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityHelper.h"


Core::VolumeClippingProcessor::VolumeClippingProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output surface" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "Taubin Smoothed Surface" );

	SetName( "VolumeClippingProcessor" );
}

void Core::VolumeClippingProcessor::Update()
{
	//SetState(Core::Runtime::APP_STATE_PROCESSING);
	//Core::vtkPolyDataPtr poly;
	//GetProcessingData( 0, poly);

	//vtkSmartPointer<meVTKVolumeClippingFilter> filter;
	//filter = vtkSmartPointer<meVTKVolumeClippingFilter>::New();
	//filter->SetInput( poly );
	////filter->SetParams(param);
	//filter->Update();
	//
	//GetOutputPort( 0 )->UpdateOutput( filter->GetOutput(), 0, GetInputDataEntity(0) );

	SetState(Core::Runtime::APP_STATE_IDLE);
}
