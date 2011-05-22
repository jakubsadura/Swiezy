/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenDataFactory.h"

#include "coreContourRenDataBuilder.h"
#include "coreITKImageRenDataBuilder.h"
#include "corePointSetRenDataBuilder.h"
#include "coreVTKImageDataRenDataBuilder.h"
#include "coreVTKPolyDataRenDataBuilder.h"
#include "coreVTKUnstructuredGridRenDataBuilder.h"
#include "coreSignalRenDataBuilder.h"
#include "coreITKTensorRenDataBuilder.h"
#include "coreITKTransformRenDataBuilder.h"

Core::RenDataFactory::RenDataFactory( void )
{
	SetNumberOfInputs( 1 );
	SetNumberOfOutputs( 0 );
}

Core::RenDataFactory::~RenDataFactory( void )
{

}

void Core::RenDataFactory::Update()
{
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );
	if ( dataEntity.IsNull( ) || 
		 dataEntity->GetNumberOfTimeSteps() == 0 )
	{
		return ;
	}

	// Switch correct builder
	RenDataBuilder::Pointer builder;
	if ( dataEntity->IsImage() )
	{
		builder = VTKImageDataRenDataBuilder::New();
	}
	else if ( dataEntity->IsSurfaceMesh() )
	{
		builder = VTKPolyDataRenDataBuilder::New();
	}
	else if ( dataEntity->IsVolumeMesh() )
	{
		builder = VTKUnstructuredGridRenDataBuilder::New();
	}
	else if ( dataEntity->IsSignal() )
	{
		builder = SignalRenDataBuilder::New();
	}
	else if ( dataEntity->IsPointSet() )
	{
		builder = PointSetRenDataBuilder::New();
	}
	else if ( dataEntity->IsContour() )
	{
		builder = ContourRenDataBuilder::New();
	}
	else if ( dataEntity->IsTensor())
	{
		builder = ITKTensorRenDataBuilder::New();
	}
	else if ( dataEntity->IsTransform())
	{
		builder = ITKTransformRenDataBuilder::New();
	}

	if ( builder )
	{
		builder->SetInputDataEntity( 0, dataEntity );
		builder->Update( );
	}

}

