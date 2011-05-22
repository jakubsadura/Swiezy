/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSignalESEDPropagationProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"
#include "blSignalCollective.h"

Core::SignalESEDPropagationProcessor::SignalESEDPropagationProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input signal" );
	GetInputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	SetNumberOfOutputs( 0 );
	
	SetName( "SignalESEDPropagationProcessor" );
}

void Core::SignalESEDPropagationProcessor::Update()
{
	try
	{
		Core::DataEntity::Pointer signalDataEntity;
		signalDataEntity = GetInputDataEntity( 0 );
		if ( signalDataEntity.IsNull() )
		{
			return;
		}

		blSignalCollective::Pointer signalData;
		signalDataEntity->GetProcessingData( signalData );

		Core::DataEntity::Pointer fatherDataEntity;
		fatherDataEntity = signalDataEntity->GetFather();

		if ( signalData.IsNull() || fatherDataEntity.IsNull())
		{
			return;
		}

		//SetState(Core::Runtime::APP_STATE_PROCESSING);

		Core::TimeStepIndex fatherTimeStep;
		blSignalAnnotation::Pointer annotation;

		// End-Systole = 1
		unsigned signalPosition;
		annotation = signalData->FindAnnotation( 1, signalPosition );
		if ( annotation )
		{
			blSignal::Pointer signal = signalData->GetSignal( signalPosition );
			blSignal::SampleType timePosition = signal->GetValueX( annotation->GetXPos() );
			fatherTimeStep = fatherDataEntity->FindTimeStep( timePosition );
			signalDataEntity->GetMetadata()->SetESTimeStep(fatherTimeStep);
			fatherDataEntity->GetMetadata()->SetESTimeStep(fatherTimeStep);
		}


		// End-Diastole = 2
		annotation = signalData->FindAnnotation( 2, signalPosition );
		if ( annotation )
		{
			blSignal::Pointer signal = signalData->GetSignal( signalPosition );
			blSignal::SampleType timePosition = signal->GetValueX( annotation->GetXPos() );
			fatherTimeStep = fatherDataEntity->FindTimeStep( timePosition );
			signalDataEntity->GetMetadata()->SetEDTimeStep(fatherTimeStep);
			fatherDataEntity->GetMetadata()->SetEDTimeStep(fatherTimeStep);
		}


		//SetState(Core::Runtime::APP_STATE_IDLE);
	}
	catch(...)
	{
		//SetState(Core::Runtime::APP_STATE_IDLE);
		throw;
	}
}

