// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreSignalReader.h"

#include "blSignalReader.h"

//!
Core::IO::SignalReader::SignalReader(void) 
{
	blSignalReader::ExtensionVectorType extensions;
	extensions = blSignalReader::GetValidExtensions();
	m_ValidExtensionsList.insert( 
		m_ValidExtensionsList.begin(), extensions.begin(), extensions.end() );
	m_ValidTypesList.push_back( SignalTypeId );
}

//!
Core::IO::SignalReader::~SignalReader(void)
{
}

//!
void Core::IO::SignalReader::Update( )
{
	// build the data entity
	return ReadAllTimeSteps( Core::SignalTypeId );
}

bool Core::IO::SignalReader::ReadHeader()
{
	m_TagMap->AddTag( "DataEntityType", Core::SignalTypeId );

	blSignalReader::Pointer reader = blSignalReader::New( );
	reader->SetFilename( m_Filenames[ 0 ].c_str() );
	reader->ReadHeader();

	m_TagMap->AddTags( reader->GetTagMapHeader() );

	return true;
}

boost::any Core::IO::SignalReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	blSignalReader::Pointer reader = blSignalReader::New( );
	reader->SetFilename( filename.c_str() );
	reader->SetTagMapHeader( m_TagMap );
	reader->Update();
	blSignalCollective::Pointer signal;
	signal = reader->GetOutput();

	if ( signal.IsNull() )
	{
		return boost::any( );
	}

	return signal;

	// Read first channel only
	//blSignalCollective::Pointer temp = blSignalCollective::New();
	//temp->AddSignal( reader->GetOutput( )->GetSignal( 0 ) );
	//return temp;
}

