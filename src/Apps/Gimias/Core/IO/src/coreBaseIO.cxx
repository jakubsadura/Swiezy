/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseIO.h"
#include "coreFile.h"
#include <cctype>


long Core::IO::BaseIO::GetLastDigits( const std::string &filename )
{
	std::string name = Core::IO::File::GetFilenameName( filename );

	// Remove extension
	name = itksys::SystemTools::GetFilenameWithoutExtension( name );

	// Remove last digits
	long pos = name.size() - 1;
	while ( name.size() && std::isdigit( name[ pos ] ) )
	{
		pos--;
	}
	pos++;

	if ( pos < 0 || pos >= name.size() )
	{
		return -1;
	}

	std::string digit = name.substr( pos, name.size() - 1 );
	return atoi( digit.c_str() );
}

bool Core::IO::BaseIO::CompareStringNumbers( const std::string &str1,const std::string &str2 )
{
	return GetLastDigits( str1 ) < GetLastDigits( str2 ); 
}

std::string Core::IO::BaseIO::RemoveLastDigits( const std::string &filename )
{
	std::string name = Core::IO::File::GetFilenameName( filename );

	// Remove extension
	name = itksys::SystemTools::GetFilenameWithoutExtension( name );

	// Remove last digits
	while ( name.size() && std::isdigit( name[ name.size() - 1 ] ) )
	{
		name = name.substr( 0, name.size() - 1 );
	}

	// Remove '_' at the end
	if ( name.size() && name[ name.size() - 1 ] == '_' )
	{
		name = name.substr( 0, name.size() - 1 );
	}
	
	return name;
}

Core::IO::BaseIO::BaseIO( void )
{
	m_TagMap = blTagMap::New( );
}

Core::IO::BaseIO::~BaseIO( void )
{

}

void Core::IO::BaseIO::SetFileNames( 
	const std::vector< std::string > &filenames )
{
	m_Filenames = filenames;
}

bool Core::IO::BaseIO::Check( Core::DataEntityType type )
{
	ValidTypesListType types = GetValidDataEntityTypes( );
	ValidTypesListType::iterator it;
	for ( it = types.begin() ; it != types.end() ; it++ )
	{
		if ( (*it) & type )
		{
			return true;
		}
	}

	return false;
}

bool Core::IO::BaseIO::Check( std::string ext )
{
	ValidExtensionsListType extensions = GetValidExtensions( );
	ValidExtensionsListType::iterator it;
	for ( it = extensions.begin() ; it != extensions.end() ; it++ )
	{
		if ( (*it) == ext )
		{
			return true;
		}
	}

	return false;
}

Core::IO::BaseIO::ValidExtensionsListType 
Core::IO::BaseIO::GetValidExtensions()
{
	return m_ValidExtensionsList;
}

Core::IO::BaseIO::ValidTypesListType 
Core::IO::BaseIO::GetValidDataEntityTypes()
{
	return m_ValidTypesList;
}

blTagMap::Pointer Core::IO::BaseIO::GetTagMap() const
{
	return m_TagMap;
}

void Core::IO::BaseIO::SetTagMap( blTagMap::Pointer val )
{
	m_TagMap = val;
}

