/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreScanFolder.h"

#include <itksys/SystemTools.hxx>
#include "coreDirectory.h"
#include <locale>

//!
Core::IO::ScanFolder::ScanFolder()
{
}

//!
Core::IO::ScanFolder::~ScanFolder(void)
{
}

void Core::IO::ScanFolder::SetFileName( const std::string &filename )
{
	m_Filenames.clear();
	m_Filenames.push_back( filename );
}

void Core::IO::ScanFolder::SetFileNames( const std::vector< std::string > &filenames )
{
	m_Filenames = filenames;
}

void Core::IO::ScanFolder::Update()
{
	std::vector<std::string>::iterator it;
	for (it = m_Filenames.begin(); it != m_Filenames.end(); ++it) 
	{
		AddFilename( *it );
	}
}

void Core::IO::ScanFolder::AddFilename( const std::string &filename )
{
	if ( itksys::SystemTools::FileIsDirectory( filename.c_str() ) )
	{
		ScanDirectory( filename );
	}
	else
	{
		// Remove last digits before extension
		std::string filteredFilename;
		std::locale loc;
		filteredFilename = Core::IO::File::GetFilenameWithoutExtension( filename );

		// Discard ".gmi" files
		std::string ext = itksys::SystemTools::GetFilenameLastExtension( filename.c_str() );
		std::string extension = itksys::SystemTools::LowerCase(ext);
		if ( extension == ".gmi" )
		{
			return;
		}

		while ( filteredFilename.size() > 0 && 
			    std::isdigit( filteredFilename[ filteredFilename.size() - 1 ], loc ) )
		{
			filteredFilename = filteredFilename.substr( 0, filteredFilename.size() - 1 );
		}

		// Add filename
		m_FilenameGroups[ filteredFilename + extension ].AddFilename( filename );
	}
}

void Core::IO::ScanFolder::ScanDirectory( const std::string &filename )
{
	Core::IO::Directory::Pointer dir = Core::IO::Directory::New();
	dir->SetDirNameFullPath( filename );

	Core::IO::FileNameList filenameList;
	filenameList = dir->GetContents();

	Core::IO::FileNameList::iterator it;
	for (it = filenameList.begin(); it != filenameList.end(); ++it) 
	{
		AddFilename( *it );
	}

}

Core::IO::ScanFolder::GroupMapType& Core::IO::ScanFolder::GetGroups()
{
	return m_FilenameGroups;
}
