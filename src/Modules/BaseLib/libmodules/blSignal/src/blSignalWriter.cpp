/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignalWriter.h"
#include "blTextUtils.h"

#include "blCSVSignalWriter.h"
#include "blGDFFileWriter.h"

#include <fstream>

blSignalWriter::blSignalWriter()
{
}

blSignalWriter::~blSignalWriter()
{
}

void blSignalWriter::SetInput( blSignalCollective::Pointer signal )
{
	m_Input = signal;
}

void blSignalWriter::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void blSignalWriter::Update()
{
	std::string ext = blTextUtils::GetFilenameLastExtension( m_Filename );

	if ( ext == ".csv" )
	{
		blCSVSignalWriter::Pointer writer = blCSVSignalWriter::New( );
		writer->SetFilename( m_Filename.c_str() );
		writer->SetInput( m_Input );
		writer->Update();
	}
	else if (ext == ".gdf" )
	{
		blGDFFileWriter::Pointer writer = blGDFFileWriter::New( );
		writer->SetFilename( m_Filename.c_str() );
		writer->SetInput( m_Input );
		writer->Update();
	}
}

blSignalWriter::ExtensionVectorType blSignalWriter::GetValidExtensions()
{
	ExtensionVectorType vector;
	vector.push_back( ".csv" );
	vector.push_back( ".gdf" );

	return vector;
}


