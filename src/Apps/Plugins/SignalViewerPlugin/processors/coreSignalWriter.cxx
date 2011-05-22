// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreSignalWriter.h"
#include "coreDataEntity.h"
#include "coreException.h"

#include "blSignalCollective.h"
#include "blSignalWriter.h"

using namespace Core::IO;

//!
SignalWriter::SignalWriter(void) 
{
	blSignalWriter::ExtensionVectorType extensions;
	extensions = blSignalWriter::GetValidExtensions();
	m_ValidExtensionsList.insert( 
		m_ValidExtensionsList.begin(), extensions.begin(), extensions.end() );
	m_ValidTypesList.push_back( SignalTypeId );
}

//!
SignalWriter::~SignalWriter(void)
{
}

//!
void SignalWriter::Update( )
{
	WriteAllTimeSteps( );
}

void Core::IO::SignalWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimeStep )
{
	blSignalCollective::Pointer signalCollective = NULL;
	bool worked = dataEntity->GetProcessingData( signalCollective, iTimeStep );
	if( !worked || signalCollective == NULL )
	{
		throw Core::Exceptions::Exception(
			"SignalWriter::WriteSingleTimeStep",
			"Input data is not of the correct type" );
	}


	blSignalWriter::Pointer writer = blSignalWriter::New( );
	writer->SetFilename( fileName.c_str() );
	writer->SetInput( signalCollective );
	writer->Update();
}

