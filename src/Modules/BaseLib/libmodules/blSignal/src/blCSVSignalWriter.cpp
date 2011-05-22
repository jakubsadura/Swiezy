/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blCSVSignalWriter.h"

#include <fstream>

blCSVSignalWriter::blCSVSignalWriter()
{
}

blCSVSignalWriter::~blCSVSignalWriter()
{
}

void blCSVSignalWriter::SetInput( blSignalCollective::Pointer signal )
{
	m_Input = signal;
}

void blCSVSignalWriter::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void blCSVSignalWriter::Update()
{
	// save the file as CSV
	std::ofstream signalFile;
	signalFile.open ( m_Filename.c_str() );

	if (signalFile.good())
	{
		for(unsigned i=0; i<m_Input->GetNumberOfSignals(); i++)
		{
			blSignal::Pointer signal = m_Input->GetSignal(i);
			std::vector< blSignal::SampleType > xVector = signal->GetXVector();
			std::vector< blSignal::SampleType > yVector = signal->GetYVector();

			//First write a header to know that it's a signal and not a numeric data entity (both .csv files)
			signalFile<< "Signal Data V1.1" << std::endl;

			signalFile << signal->GetName();
			signalFile << std::endl;
			signalFile << signal->GetXUnit();
			signalFile << std::endl;
			signalFile << signal->GetYUnit();
			signalFile << std::endl;

			signalFile << signal->GetNumberOfRecords();
			signalFile << std::endl;
			signalFile << signal->GetSamplesPerRecord();
			signalFile << std::endl;
			signalFile << signal->GetRecordDuration().numerator;
			signalFile << std::endl;
			signalFile << signal->GetRecordDuration().dominator;
			signalFile << std::endl;
			signalFile << signal->GetInternalStartTime();
			signalFile << std::endl;

			//write xVector values
			for(unsigned j=0; j<xVector.size(); j++)
			{
				signalFile << xVector.at(j);
				signalFile << ";";
			}
			signalFile << std::endl;

			//write yVector values
			for(unsigned j=0; j<yVector.size(); j++)
			{
				signalFile << yVector.at(j);
				signalFile << ";";
			}
			signalFile << std::endl;

			// write annotations
			std::vector< blSignalAnnotation::Pointer > annotationsVector;
			annotationsVector = signal->GetAnnotationsVector( );
			signalFile << annotationsVector.size();
			signalFile << ";";
			for(unsigned j=0; j<annotationsVector.size(); j++)
			{
				signalFile << annotationsVector[ j ]->GetType();
				signalFile << ";";
				signalFile << annotationsVector[ j ]->GetXPos();
				signalFile << ";";
				signalFile << annotationsVector[ j ]->GetValueY();
				signalFile << ";";
				signalFile << annotationsVector[ j ]->GetDuration();
				signalFile << ";";
			}
			signalFile << std::endl;

			// Two end of lines
			signalFile << std::endl << std::endl;
		}
		signalFile.close();
	}
}


