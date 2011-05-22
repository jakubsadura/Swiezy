/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseDataEntityWriter.h"
#include "blTextUtils.h"
#include "coreException.h"

#include "blXMLTagMapWriter.h"

Core::IO::BaseDataEntityWriter::BaseDataEntityWriter( void ) 
{
	SetNumberOfInputs( 1 );
}

Core::IO::BaseDataEntityWriter::~BaseDataEntityWriter( void )
{

}

void Core::IO::BaseDataEntityWriter::WriteAllTimeSteps( )
{
	if( GetInputDataEntity( 0 ).IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"BaseDataEntityWriter::WriteSingleTimeStep()", 
			"Input data entity is NULL" );
	}

	//write the image
	for ( size_t iTimeStep = 0 ; iTimeStep < GetInputDataNumberOfTimeSteps( 0 ) ; iTimeStep++ )
	{
		std::string strFileName = GetFilename( iTimeStep );
		WriteSingleTimeStep( strFileName, GetInputDataEntity( 0 ), iTimeStep );
	}

}

void Core::IO::BaseDataEntityWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity,
	int iTimeStep )
{

	throw Core::Exceptions::Exception(
		"Core::IO::BaseDataEntityWriter::WriteSingleTimeStep",
		"Function not implemented");
}

bool Core::IO::BaseDataEntityWriter::CanWrite( Core::DataEntityType type )
{
	return Check( type );
}

bool Core::IO::BaseDataEntityWriter::CanWrite( std::string ext )
{
	return Check( ext );
}

bool Core::IO::BaseDataEntityWriter::WriteMetaData()
{
	if ( m_Filenames.empty() || GetNumberOfInputs() == 0 || GetInputDataEntity( 0 ).IsNull() )
	{
		return false;
	}

	// Add "gmi" extension GiMias Information
	std::string strFileName = GetFilename( 0 );
	std::string filename = itksys::SystemTools::GetFilenameWithoutExtension( strFileName );
	filename = itksys::SystemTools::GetFilenamePath( m_Filenames[ 0 ] ) + "/" + filename + ".gmi";

	try
	{
		blTagMap::Pointer tagMap(GetInputDataEntity( 0 )->GetMetadata( ));
		blXMLTagMapWriter::Pointer writer = blXMLTagMapWriter::New();
		writer->SetFilename( filename.c_str() );
		writer->SetInput( tagMap );
		writer->Update();
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void Core::IO::BaseDataEntityWriter::WriteData()
{

}

void Core::IO::BaseDataEntityWriter::Update()
{
	WriteMetaData( );
	WriteData( );
}

std::string Core::IO::BaseDataEntityWriter::GetFilename( int iTimeStep )
{
	std::string strFileName;
	// If there's more than one image -> Create new name for each one
	if ( GetInputDataEntity( 0 )->GetNumberOfTimeSteps( ) > 1 )
	{
		if ( m_Filenames.size() == 1 )
		{
			strFileName = AddFileNumber( m_Filenames[ 0 ], iTimeStep );
		}
		else
		{
			strFileName = m_Filenames.at( iTimeStep );
		}
	}
	else
	{
		strFileName = m_Filenames[ 0 ];
	}

	return strFileName;
}

