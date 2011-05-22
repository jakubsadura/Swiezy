/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignalCompare.h"
#include "blSignalReader.h"
#include "blSignalCollective.h"
#include "blNumericUtils.h"

#include <fstream>
#include <stdlib.h>
#include <math.h>


blSignalCompare::blSignalCompare() 
{
	m_bResult = false;
}

blSignalCompare::~blSignalCompare()
{
}

void blSignalCompare::SetFilename1( const char* filename )
{
	m_Filename1 = filename;
}

void blSignalCompare::SetFilename2( const char* filename )
{
	m_Filename2 = filename;
}

void blSignalCompare::Update()
{
	m_bResult = false;

	blSignalReader::Pointer reader;
	reader = blSignalReader::New( );
	reader->SetFilename( m_Filename1.c_str() );
	reader->Update( );
	blSignalCollective::Pointer signalCollective1 = reader->GetOutput( );

	reader = blSignalReader::New( );
	reader->SetFilename( m_Filename2.c_str() );
	reader->Update( );
	blSignalCollective::Pointer signalCollective2 = reader->GetOutput( );

	m_bResult = true;

	if ( signalCollective1.IsNull( ) || signalCollective2.IsNull( ) )
	{
		m_bResult = false;
		return;
	}

	// Compare number of signals
	if ( signalCollective1->GetNumberOfSignals() != signalCollective2->GetNumberOfSignals() )
	{
		std::cout 
			<< "[blSignalCompare::Update] Number of signals is different" 
			<< signalCollective1->GetNumberOfSignals() << " != "
			<< signalCollective2->GetNumberOfSignals()
			<< std::endl;
		m_bResult = false;
		return;
	}

	// Compare each signal
	for( unsigned iSignalNum = 0; iSignalNum < signalCollective1->GetNumberOfSignals() ; iSignalNum++ )
	{
		blSignal::Pointer signal1;
		blSignal::Pointer signal2;
		signal1 = signalCollective1->GetSignal( iSignalNum );
		signal2 = signalCollective2->GetSignal( iSignalNum );
		if( signal1->GetNumberOfRecords() != signal2->GetNumberOfRecords() )
		{
			std::cout 
				<< "[blSignalCompare::Update] Number of records is different " 
				<< signal1->GetNumberOfRecords() << " != " 
				<< signal2->GetNumberOfRecords()
				<< std::endl;
			m_bResult = false;
			return;
		}

		std::vector< blSignal::SampleType > signal1Data = signal1->GetYVector();
		std::vector< blSignal::SampleType > signal2Data = signal2->GetYVector();
		float fMaxDifference = 0;
		int iMaxPointPosition = -1;
		float fEpsilon = 1.5;
		bool bRes = true;
		for ( unsigned iSampleNum = 0 ; iSampleNum < signal1Data.size() ; iSampleNum++ )
		{
			bRes = bRes && FLOAT_EQ_INCLUSIVE( 
				signal1Data.at( iSampleNum ), 
				signal2Data.at( iSampleNum ), 
				fEpsilon );
			float diff = fabs( signal1Data.at( iSampleNum ) - signal2Data.at( iSampleNum ) );
			if ( fMaxDifference < diff )
			{
				fMaxDifference = diff;
				iMaxPointPosition = iSampleNum;
			}
		}

		if ( !bRes )
		{
			std::cout 
				<< "[blSignalCompare::Update] Signal data is different "
				<< "at point " << iMaxPointPosition 
				<< " of signal " << iSignalNum << ": "
				<< signal1Data.at( iMaxPointPosition ) << " != " 
				<< signal2Data.at( iMaxPointPosition )
				<< std::endl;
			m_bResult = false;
			return;
		}

	}
}

bool blSignalCompare::GetOutput()
{
	return m_bResult;
}

