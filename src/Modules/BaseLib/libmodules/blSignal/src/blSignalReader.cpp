/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignalReader.h"
#include "blTextUtils.h"

#include "blCSVSignalReader.h"
#include "blGDFFileReader.h"
#include "blBVFileReader.h"

#include <fstream>
#include <stdlib.h>


blSignalReader::blSignalReader() 
{
	m_TagMapHeader = blTagMap::New( );
}

blSignalReader::~blSignalReader()
{
}

void blSignalReader::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void blSignalReader::Update()
{
	std::string ext = blTextUtils::GetFilenameLastExtension( m_Filename );

	if ( ext == ".csv" )
	{
		blCSVSignalReader::Pointer reader = blCSVSignalReader::New( );
		reader->SetFilename( m_Filename.c_str() );
		reader->Update( );
		m_Output = reader->GetOutput( );
	}
	else if( ext == ".gdf")
	{
		blGDFFileReader::Pointer reader = blGDFFileReader::New( );
		reader->SetFilename( m_Filename.c_str() );
		reader->Update( );
		m_Output = reader->GetOutput( );
	}
	else if( ext == ".vhdr")
	{
		blBVFileReader::Pointer reader = blBVFileReader::New( );
		reader->SetFilename( m_Filename.c_str() );
		reader->SetHeaderTags( m_TagMapHeader );
		reader->Update( );
		m_Output = reader->GetOutput( );
	}

}

blSignalCollective::Pointer blSignalReader::GetOutput()
{
	return m_Output;
}

blSignalReader::ExtensionVectorType blSignalReader::GetValidExtensions()
{
	ExtensionVectorType vector;
	vector.push_back( ".csv" );
	vector.push_back( ".gdf" );
	vector.push_back( ".vhdr" );

	return vector;
}

blTagMap::Pointer blSignalReader::GetTagMapHeader() const
{
	return m_TagMapHeader;
}

bool blSignalReader::ReadHeader()
{
	std::string ext = blTextUtils::GetFilenameLastExtension( m_Filename );

	m_TagMapHeader = blTagMap::New( );

	if ( ext == ".csv" )
	{
		return false;
	}
	else if( ext == ".gdf")
	{
		return false;
	}
	else if( ext == ".vhdr")
	{
		blBVFileReader::Pointer reader = blBVFileReader::New( );
		reader->SetFilename( m_Filename.c_str() );
		reader->ReadHeader( );
		reader->GetHeaderTags( m_TagMapHeader );
	}

	return true;
}

void blSignalReader::SetTagMapHeader( blTagMap::Pointer tagMap )
{
	m_TagMapHeader = tagMap;
}
