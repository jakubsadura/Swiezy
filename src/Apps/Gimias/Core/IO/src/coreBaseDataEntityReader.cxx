/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseDataEntityReader.h"
#include "coreFile.h"

#include "blXMLTagMapReader.h"

Core::IO::BaseDataEntityReader::BaseDataEntityReader( void )
{
	SetNumberOfOutputs( 1 );
}

Core::IO::BaseDataEntityReader::~BaseDataEntityReader( void )
{

}

bool Core::IO::BaseDataEntityReader::ReadHeader( )
{
	return false;
}

bool Core::IO::BaseDataEntityReader::CanRead( Core::DataEntityType type )
{
	return Check( type );
}

bool Core::IO::BaseDataEntityReader::CanRead( std::string ext )
{
	return Check( ext );
}

void Core::IO::BaseDataEntityReader::ReadAllTimeSteps( Core::DataEntityType type )
{
	if ( m_Filenames.size() == 0 )
	{
		return ;
	}

	// Read type from metadata and overwrite input parameter
	if ( GetNumberOfOutputs() && GetOutputDataEntity( 0 ).IsNotNull() )
	{
		int temp;
		blTag::Pointer tag;
		tag = GetOutputDataEntity( 0 )->GetMetadata()->FindTagByName( "Type" );
		if ( tag.IsNotNull() && tag->GetValue( temp ) && temp != UnknownTypeId )
		{
			type = Core::DataEntityType( temp );
		}
	}


	Core::DataEntity::Pointer dataEntity = NULL;

	// Sort 00, 01, 02, ...
	std::vector<std::string> fileNamesSorted;
	for ( unsigned i = 0 ; i < m_Filenames.size( ) ; i++ )
	{
		fileNamesSorted.push_back( m_Filenames[ i ].c_str( ) );
	}
	std::sort( fileNamesSorted.begin( ), fileNamesSorted.end( ), CompareStringNumbers );

	// Load a vector of vtkImageData
	std::vector< boost::any > processingDataVector;
	for ( unsigned iTimeStep = 0 ; iTimeStep < fileNamesSorted.size() ; iTimeStep++ )
	{
		boost::any processingData = 
			ReadSingleTimeStep( iTimeStep, fileNamesSorted[ iTimeStep ].c_str() );

		if ( !processingData.empty() )
		{
			processingDataVector.push_back( processingData );
		}
	}

	// Create the DataEntity
	if( processingDataVector.size() > 0 )
	{
		// Use DataEntity factory to build DataEntity from vtkPolyData
		dataEntity = GetOutputDataEntity( 0 );
		if ( dataEntity.IsNull() )
		{
			dataEntity = Core::DataEntity::New( );
		}
		dataEntity->SetType( type );
		dataEntity->AddTimeSteps( processingDataVector );
		if(dataEntity.IsNotNull())
		{	
			dataEntity->GetMetadata( )->SetName( RemoveLastDigits(fileNamesSorted[ 0 ]) );

			SetOutputDataEntity(0, dataEntity );
		}
	}
}

bool Core::IO::BaseDataEntityReader::ReadMetaData()
{
	if ( m_Filenames.empty() || GetNumberOfOutputs() == 0 )
	{
		return false;
	}

	// Add "gmi" extension GiMias Information
	std::string filename = itksys::SystemTools::GetFilenameWithoutExtension( m_Filenames[ 0 ] );
	filename = itksys::SystemTools::GetFilenamePath( m_Filenames[ 0 ] ) + "/" + filename + ".gmi";

	try
	{
		blXMLTagMapReader::Pointer xmlTagMapReader = blXMLTagMapReader::New( );
		xmlTagMapReader->SetFilename( filename.c_str() );
		xmlTagMapReader->Update();
		blTagMap::Pointer tagMap = xmlTagMapReader->GetOutput();
		if ( tagMap.IsNull() )
		{
			return false;
		}

		Core::DataEntity::Pointer dataEntity = Core::DataEntity::New();
		dataEntity->GetMetadata()->AddTags( tagMap );
		SetOutputDataEntity( 0, dataEntity );
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void Core::IO::BaseDataEntityReader::Update()
{
	ReadMetaData();
	ReadData( );
}

void Core::IO::BaseDataEntityReader::ReadData()
{

}
