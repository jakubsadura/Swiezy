/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blCSVSignalReader.h"
#include "blTextUtils.h"

#include <fstream>
#include <stdlib.h>


blCSVSignalReader::blCSVSignalReader() 
{
}

blCSVSignalReader::~blCSVSignalReader()
{
}

void blCSVSignalReader::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void blCSVSignalReader::Update()
{
	// save the file as CSV
	std::ifstream signalFile;
	signalFile.open ( m_Filename.c_str() );

	if (signalFile.good())
	{

		m_Output = blSignalCollective::New( );

		bool fileisOk = true;
		std::string line;
		while ( fileisOk )
		{
			blSignal::Pointer signal = blSignal::New( );
			std::vector<blSignal::SampleType> xVector;
			std::vector<blSignal::SampleType> yVector;

			fileisOk = std::getline(signalFile,line) && fileisOk;
			// check that the type is a signal (the first line), and not a numeric data entity
			if ( line != "Signal Data V1.1" && fileisOk)
			{
				m_Output = NULL;
				return;
			}

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetName( line );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetXUnit( line );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetYUnit( line );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetNumberOfRecords( atoi( line.c_str() ) );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetSamplesPerRecord( atoi( line.c_str() ) );

			blSignal::Duration dur;
			fileisOk = std::getline(signalFile,line) && fileisOk;
			dur.numerator = atoi( line.c_str() );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			dur.dominator = atoi( line.c_str() );
			signal->SetRecordDuration( dur );

			fileisOk = std::getline(signalFile,line) && fileisOk;
			signal->SetInternalStartTime( atoi( line.c_str() ) );


			fileisOk = std::getline(signalFile,line) && fileisOk;
			ParseLine( line, xVector );
			signal->SetXVector( xVector );
			
			fileisOk = std::getline(signalFile,line) && fileisOk;
			ParseLine( line, yVector );
			signal->SetYVector( yVector );

			// Annotations
			fileisOk = std::getline(signalFile,line) && fileisOk;
			std::list<std::string> annotationData;
			blTextUtils::ParseLine( line, ';', annotationData );
			std::list<std::string>::iterator it = annotationData.begin();
			if ( it != annotationData.end() )
			{
				int numAnnotations = atoi( (it++)->c_str() );
				for ( int i = 0 ; i < numAnnotations ; i++ )
				{
					blSignalAnnotation::Pointer annotation = blSignalAnnotation::New();

					if(it != annotationData.end())
						annotation->SetType( atoi( (it++)->c_str( ) ) );

					if(it != annotationData.end())
						annotation->SetXPos( atoi( (it++)->c_str( ) ) );

					if(it != annotationData.end())
						annotation->SetValueY( float( atof( (it++)->c_str( ) ) ) );

					if(it != annotationData.end())
						annotation->SetDuration( atoi( (it++)->c_str( ) ) );

					signal->AddAnnotation( annotation );
				}
			}

			// Two lines at the end of each signal
			fileisOk = std::getline(signalFile,line) && fileisOk;
			fileisOk = std::getline(signalFile,line) && fileisOk;

			// Create the signal
			if ( fileisOk )
			{
				m_Output->AddSignal( signal );
			}
		}

		signalFile.close();
	}
}

blSignalCollective::Pointer blCSVSignalReader::GetOutput()
{
	return m_Output;
}

void blCSVSignalReader::ParseLine( 
	std::string &line,
	std::vector<blSignal::SampleType> &data )
{
	std::list<std::string> numbers;

	blTextUtils::ParseLine( line, ';', numbers );

	// Convert from string to float
	while ( numbers.size() )
	{
		data.push_back( float( atof( numbers.front( ).c_str() ) ) );
		numbers.pop_front();
	}
}
