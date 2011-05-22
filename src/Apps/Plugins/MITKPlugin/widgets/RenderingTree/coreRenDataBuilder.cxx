/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenDataBuilder.h"

#include "coreContourRenDataBuilder.h"
#include "coreITKImageRenDataBuilder.h"
#include "corePointSetRenDataBuilder.h"
#include "coreVTKImageDataRenDataBuilder.h"
#include "coreVTKPolyDataRenDataBuilder.h"
#include "coreVTKUnstructuredGridRenDataBuilder.h"

Core::RenDataBuilder::RenDataBuilder( void )
{
	SetNumberOfInputs( 1 );
	SetNumberOfOutputs( 0 );
}

Core::RenDataBuilder::~RenDataBuilder( void )
{

}

bool Core::RenDataBuilder::CheckInput()
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );
	if ( dataEntity.IsNull( ) || 
		dataEntity->GetNumberOfTimeSteps() == 0 )
	{
		return false;
	}

	//mitk::BaseData::Pointer mitkData;
	//Core::CastAnyProcessingData( dataEntity->GetRenderingData(), mitkData);
	//if ( mitkData.IsNotNull() &&
	//	mitkData->GetMTime() > dataEntity->GetMTime() )
	//{
	//	return false;
	//}

	return true;
}

