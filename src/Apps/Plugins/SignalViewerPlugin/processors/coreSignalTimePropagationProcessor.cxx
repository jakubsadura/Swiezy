/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSignalTimePropagationProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"
#include "blSignalCollective.h"

Core::SignalTimePropagationProcessor::SignalTimePropagationProcessor( )
{
	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input data entity" );
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	GetInputPort( 1 )->SetName( "Input signal" );
	GetInputPort( 1 )->SetDataEntityType( Core::SignalTypeId );
	SetNumberOfOutputs( 0 );
	
	SetName( "SignalTimePropagationProcessor" );

	m_MarkersPerScan = 1;
}

void Core::SignalTimePropagationProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		int numOfTimeSteps = GetInputDataEntity(0)->GetNumberOfTimeSteps();

		blSignalCollective::Pointer signal ;
		GetProcessingData(1, signal);

		std::vector< blSignalAnnotation::Pointer > annots;
		for ( int i = 0 ; i < signal->GetNumberOfSignals() ; i++ )
		{
			if ( signal->GetSignal(i)->GetAnnotation( 0 ).IsNotNull() )
			{
				annots = signal->GetSignal(i)->GetAnnotationsVector();
			}
		}

		// if you don't have markers just split the xaxis with the number of time steps
		if (annots.size() == 0)
		{
			int numOfSamples = signal->GetSignal(0)->GetNumberOfValues();

			for (int it = 0; it < numOfTimeSteps; it++)
			{
				blSignal::SampleType time;
				time = signal->GetSignal(0)->GetValueX( int(it*numOfSamples/numOfTimeSteps) );
				GetInputDataEntity(0)->SetTimeAtTimeStep(it, time );
			}
		}
		else
		{


			// Propagate time information
			int annotationIndex = 0;
			int timeSt = 0;
			while (annotationIndex < annots.size() && timeSt < numOfTimeSteps )
			{
				blSignal::SampleType time;
				time = signal->GetSignal(0)->GetValueX( annots.at(annotationIndex)->GetXPos() );
				GetInputDataEntity(0)->SetTimeAtTimeStep( timeSt, time);
				annotationIndex += m_MarkersPerScan;
				timeSt++;
			}
		}

		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::SignalTimePropagationProcessor::Update)
}

int Core::SignalTimePropagationProcessor::GetMarkersPerScan() const
{
	return m_MarkersPerScan;
}

void Core::SignalTimePropagationProcessor::SetMarkersPerScan( int val )
{
	m_MarkersPerScan = val;
}
