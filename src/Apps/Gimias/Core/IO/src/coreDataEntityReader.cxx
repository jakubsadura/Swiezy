/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityReader.h"
#include "coreDataEntityInfoHelper.h"
#include "coreScanFolder.h"
#include "coreFile.h"
#include "coreStringHelper.h"

#include <itksys/SystemTools.hxx>

Core::IO::DataEntityReader::RegisteredReadersType Core::IO::DataEntityReader::m_RegisteredReaders;

//!
Core::IO::DataEntityReader::DataEntityReader()
{
	SetNumberOfOutputs( 0 );
	m_TagMap = blTagMap::New( );
}

//!
Core::IO::DataEntityReader::~DataEntityReader(void)
{
}

//!
Core::IO::BaseIO::ValidExtensionsListType
Core::IO::DataEntityReader::GetRegisteredReaderFormats(Core::DataEntityType type)
{
	BaseIO::ValidExtensionsListType formatList;
	RegisteredReadersType::iterator it;
	for(it = m_RegisteredReaders.begin(); it != m_RegisteredReaders.end(); ++it)
	{
		if ( it->second->CanRead( type ) )
		{
			BaseIO::ValidExtensionsListType extensions;
			extensions = it->second->GetValidExtensions();
			formatList.insert(formatList.end(), extensions.begin(), extensions.end());
		}
	}

	return formatList;
}

void Core::IO::DataEntityReader::SetFileName( const std::string &filename )
{
	std::string fullPathFilename = itksys::SystemTools::CollapseFullPath( filename.c_str() );
	m_Filenames.clear();
	m_Filenames.push_back( fullPathFilename );
}

void Core::IO::DataEntityReader::SetFileNames( const std::vector< std::string > &filenames )
{
	m_Filenames.clear();
	for ( int i = 0 ; i < filenames.size() ; i++ )
	{
		std::string fullPathFilename;
		// On windows, network paths gives an error when calling CollapseFullPath( )
		if ( filenames[ i ].find( "\\\\" ) != 0 )
		{
			fullPathFilename = itksys::SystemTools::CollapseFullPath( filenames[ i ].c_str() );
		}
		else
		{
			fullPathFilename = filenames[ i ];
		}
		m_Filenames.push_back( fullPathFilename );
	}
}

bool  Core::IO::DataEntityReader::ReadHeader()
{
	blTagMap::Pointer tagMap;
	bool correctlyRead = false;
	std::string fileName0 = m_Filenames[ 0 ];

	// Infer the extension from the filename
	std::string ext = itksys::SystemTools::GetFilenameLastExtension(fileName0);
	std::string extension = itksys::SystemTools::LowerCase(ext);

	// For each reader, check if it can read
	RegisteredReadersType::iterator it = m_RegisteredReaders.begin();
	while( !correctlyRead  && it != m_RegisteredReaders.end() )
	{
		if ( it->second->CanRead( extension ) )
		{
			// Found a reader, instantiate it and try read header
			BaseDataEntityReader::Pointer reader;
			reader = static_cast<Core::IO::BaseDataEntityReader*>(it->second->CreateAnother().GetPointer());

			reader->SetFileNames( m_Filenames );
			if ( reader->ReadHeader( ) )
			{
				m_TagMap = reader->GetTagMap();
				// Infer the extension from the filename
				m_TagMap->AddTag( "Extension", extension );
				correctlyRead = true;
			}
		}
		++it;
	}
	return correctlyRead;
}

void Core::IO::DataEntityReader::Update()
{

	Core::StringHelper::setLocale();

	// Try to read the complete folder using DICOM reader
	ReadGroup( "", m_Filenames );
	if ( GetNumberOfOutputs() > 0 )
	{
		return;
	}

	// Else try to read the grouped filenames
	ScanFolder::Pointer scanFolder = ScanFolder::New( );
	scanFolder->SetFileNames( m_Filenames );
	scanFolder->Update();

	ScanFolder::GroupMapType groups = scanFolder->GetGroups();
	ScanFolder::GroupMapType::iterator itGroup;
	for ( itGroup  = groups.begin() ; itGroup != groups.end() ; itGroup++ )
	{
		ReadGroup( itGroup->first, itGroup->second.GetFilenames( ) );
	}

	Core::StringHelper::restoreLocale();
}


std::string Core::IO::DataEntityReader::GetFileFilterTypesForRead(void) 
{
	Core::IO::DataEntityReader::Pointer dataEntityReader;
	dataEntityReader = Core::IO::DataEntityReader::New( );
	std::ostringstream filter;
	filter << "All files(*.*)|*.*";
	bool addedToFilter = false;

	// For each entity type
	Core::DataEntityInfoHelper::EntityList entityList = Core::DataEntityInfoHelper::GetAvailableEntityTypes();
	Core::DataEntityInfoHelper::EntityList::iterator itType;
	for( itType = entityList.begin();itType != entityList.end();++itType)
	{
		BaseIO::ValidExtensionsListType formatList;
		BaseIO::ValidExtensionsListType::iterator formatListIt;

		formatList = dataEntityReader->GetRegisteredReaderFormats( (*itType) );
		formatListIt = formatList.begin();
		// Append the Entity type string
		addedToFilter = formatListIt != formatList.end();
		if( addedToFilter )
		{
			filter << "|" << Core::DataEntityInfoHelper::GetEntityTypeAsString((*itType)) << "(";
		}

		// Append the types to the string
		while(formatListIt != formatList.end())
		{
			filter << "*" << (*formatListIt);
			++formatListIt;
			if(formatListIt != formatList.end())
				filter << ", ";
		}
		if(addedToFilter)
			filter << ")|";

		// Append now the wildcards
		for(formatListIt = formatList.begin(); formatListIt != formatList.end() && addedToFilter; ++formatListIt)
		{
			filter << "*" << (*formatListIt);
			if(formatListIt != formatList.end())
				filter << ";";
		}
	}

	// return the cumulated filter
	return filter.str();
}

void Core::IO::DataEntityReader::UnRegisterAllFormats()
{
	m_RegisteredReaders.clear();
}


void Core::IO::DataEntityReader::RegisterFormatReader(
	BaseDataEntityReader::Pointer reader)
{
	m_RegisteredReaders[ reader->GetNameOfClass() ] = reader;
}

void Core::IO::DataEntityReader::UnRegisterFormatReader(
	BaseDataEntityReader::Pointer reader)
{
	RegisteredReadersType::iterator it = m_RegisteredReaders.begin();
	bool found = false;
	while ( it != m_RegisteredReaders.end() && !found )
	{
		if ( typeid( it->second.GetPointer() ) == typeid( *reader.GetPointer() ) )
		{
			found = true;
		}
		else
		{
			++it;
		}
	}

	if (found) 
	{
		m_RegisteredReaders.erase( it );
	}
}

blTagMap::Pointer Core::IO::DataEntityReader::GetTagMap() const
{
	return m_TagMap;
}

void Core::IO::DataEntityReader::SetTagMap( blTagMap::Pointer val )
{
	m_TagMap = val;
}

void Core::IO::DataEntityReader::ReadGroup( 
	const std::string name,
	const std::vector<std::string> &filenames )
{
	if ( filenames.size() == 0 )
	{
		return;
	}

	std::string fileName0 = filenames[ 0 ];

	// Infer the extension from the filename
	std::string ext = itksys::SystemTools::GetFilenameLastExtension(fileName0);
	std::string extension = itksys::SystemTools::LowerCase(ext);

	// For each reader, check if it can read
	RegisteredReadersType::iterator it = m_RegisteredReaders.begin();
	bool correctlyRead = false;
	while( it != m_RegisteredReaders.end() && !correctlyRead )
	{
		if ( it->second->CanRead( extension ) )
		{
			// Found a reader, instantiate it and try read
			BaseDataEntityReader::Pointer reader;
			reader = static_cast<Core::IO::BaseDataEntityReader*>(it->second->CreateAnother().GetPointer());
			reader->SetFileNames( filenames );
			reader->SetTagMap( m_TagMap );

			// Try other readers for dcm files
			try
			{
				reader->Update( );
			}
			catch(...)
			{

			}

			if ( reader->GetNumberOfOutputs() && 
				 reader->GetOutputDataEntity(0).IsNotNull() && 
				 reader->GetOutputDataEntity(0)->GetNumberOfTimeSteps() > 0 )
			{
				correctlyRead = true;
				for ( int i = 0 ; i < reader->GetNumberOfOutputs() ; i++ )
				{
					Core::DataEntity::Pointer dataEntity = reader->GetOutputDataEntity( i );
					AddOutputDataEntity( dataEntity );

					// If data entiy has no name
					if ( dataEntity->GetMetadata( )->GetName( ).empty( ) ||
						 dataEntity->GetMetadata( )->GetName( ) == "Unnamed" )
					{
						if ( filenames.size() > 1 )
						{
							dataEntity->GetMetadata( )->SetName( Core::IO::File::GetFilenameWithoutExtension( name ) );
						}
						else
						{
							std::string filenameWithoutExt = File::GetFilenameWithoutExtension( fileName0 );
							dataEntity->GetMetadata( )->SetName( filenameWithoutExt );
						}
					}
				}
			}

		}

		++it;
	}
}

Core::IO::BaseDataEntityReader::Pointer 
Core::IO::DataEntityReader::GetRegisteredReader( const std::string &name )
{
	RegisteredReadersType::iterator it = m_RegisteredReaders.find( name );
	if ( it == m_RegisteredReaders.end() )
	{
		return NULL;
	}	

	return it->second;
}
