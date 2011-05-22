/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSignalRenDataBuilder.h"
#include "coreDataEntity.h"
#include "blMitkSignal.h"

using namespace Core;

//!
SignalRenDataBuilder::SignalRenDataBuilder(void)
{
}

//!
SignalRenDataBuilder::~SignalRenDataBuilder(void)
{
}

//!
void SignalRenDataBuilder::Update( )
{

	//! Check processing data
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	blMitk::Signal::Pointer mitkSignal = dynamic_cast<blMitk::Signal*> ( mitkData.GetPointer() );

	// Create rendering data
	if ( mitkSignal.IsNull() )
	{
		mitkSignal = blMitk::Signal::New();
	}

	mitkSignal->Expand( dataEntity->GetNumberOfTimeSteps() );

	// For all time steps
	for ( unsigned iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps() ; iTimeStep++ )
	{
		// Be careful with this code. If you create a new rendering data
		// The observers to the data entity needs to update the node in the
		// rendering tree with SetData function each time this function is called
		blSignalCollective::Pointer signal;
		dataEntity->GetProcessingData( signal, iTimeStep, false );

		mitkSignal->SetSignal( signal );

	}
		
	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkSignal) );
}

bool Core::SignalRenDataBuilder::CheckProcessingDataIsOk( 
	const Core::DataEntityImpl::Pointer timeStep )
{
	blSignalCollective::Pointer signal;
	bool worked = CastAnyProcessingData( timeStep->GetDataPtr(), signal ); 
	return worked;
}
