/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDoubleBananaProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"
#include "blArithmeticFilter.h"

Core::DoubleBananaProcessor::DoubleBananaProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input signal" );
	GetInputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetName( "Output signal" );
	GetOutputPort( 0 )->SetDataEntityName( "Double banana" );
	
	SetName( "DoubleBananaProcessor" );
}

void Core::DoubleBananaProcessor::Update()
{
	try
	{
		blSignalCollective::Pointer signal;
		GetProcessingData(0, signal);

		blSignalCollective::Pointer signalOut;
		signalOut = blSignalCollective::New( );

		blArithmeticFilter::Pointer signalOperation;
		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Fp1" ) );
		signalOperation->SetInput2( signal->GetSignal( "F7" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "F7" ) );
		signalOperation->SetInput2( signal->GetSignal( "T7" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "T7" ) );
		signalOperation->SetInput2( signal->GetSignal( "P7" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "P7" ) );
		signalOperation->SetInput2( signal->GetSignal( "O1" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Fp2" ) );
		signalOperation->SetInput2( signal->GetSignal( "F8" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "F8" ) );
		signalOperation->SetInput2( signal->GetSignal( "T8" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "T8" ) );
		signalOperation->SetInput2( signal->GetSignal( "P8" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "P8" ) );
		signalOperation->SetInput2( signal->GetSignal( "O2" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Fp1" ) );
		signalOperation->SetInput2( signal->GetSignal( "F3" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "F3" ) );
		signalOperation->SetInput2( signal->GetSignal( "C3" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "C3" ) );
		signalOperation->SetInput2( signal->GetSignal( "P3" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "P3" ) );
		signalOperation->SetInput2( signal->GetSignal( "O1" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Fp2" ) );
		signalOperation->SetInput2( signal->GetSignal( "F4" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "F4" ) );
		signalOperation->SetInput2( signal->GetSignal( "C4" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "C4" ) );
		signalOperation->SetInput2( signal->GetSignal( "P4" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "P4" ) );
		signalOperation->SetInput2( signal->GetSignal( "O2" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Fz" ) );
		signalOperation->SetInput2( signal->GetSignal( "Cz" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "Cz" ) );
		signalOperation->SetInput2( signal->GetSignal( "Pz" ) );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		// Compute Ref: Mean of all signals
		blSignal::Pointer ref = blSignal::Build( signal->GetSignal( 0 ), 0, false );
		ref->SetName( "Ref" );
		//blSignal::Pointer ref = blSignal::New();
		//ref->Copy( signal->GetSignal( 0 ) );
		//signalOperation = blArithmeticFilter::New();
		//signalOperation->SetOperation( BLSIGNAL_ADD );
		//signalOperation->SetInput1( ref );
		//signalOperation->SetOutput( ref );
		//for ( int i = 1 ; i < signal->GetNumberOfSignals() ; i++ )
		//{
		//	signalOperation->SetInput2( signal->GetSignal( i ) );
		//	signalOperation->Update();
		//}

		//signalOperation = blArithmeticFilter::New();
		//signalOperation->SetOperation( BLSIGNAL_DIVIDE_K );
		//signalOperation->SetK( signal->GetNumberOfSignals() );
		//signalOperation->SetInput1( ref );
		//signalOperation->SetOutput( ref );
		//signalOperation->Update();
		//ref->SetName( "Ref" );

		signalOperation = blArithmeticFilter::New( );
		signalOperation->SetOperation( BLSIGNAL_SUBSTRACT );
		signalOperation->SetInput1( signal->GetSignal( "ECG" ) );
		signalOperation->SetInput2( ref );
		signalOperation->Update( );
		signalOut->AddSignal( signalOperation->GetOutput( ) );

		// Create an empty signal with time information
		blSignal::Pointer output = blSignal::New( );
		output->Copy( signal->GetSignal( "Fp1" ) );
		output->SetName( "Time" );
		std::vector< blSignal::SampleType > values( output->GetNumberOfValues() );
		output->SetYVector( values );

		std::vector< blSignalAnnotation::Pointer > annotations;
		annotations = output->GetAnnotationsVector();
		for ( int i = 0 ; i < annotations.size() ; i++ )
		{
			// Empty name
			annotations[ i ]->SetType( 4 );
		}
		signalOut->AddSignal( output );

		// Set start time to 0
		for ( int i = 0 ; i < signalOut->GetNumberOfSignals() ; i++ )
		{
			output = signalOut->GetSignal( i );
			output->SetStartTime( 0 );
		}

		UpdateOutput( 0, signalOut, "Double banana", true, 1, GetInputDataEntity( 0 ) );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::DoubleBananaProcessor::Update)
}
