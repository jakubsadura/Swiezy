/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynDataTransferCLP.h"
#include "coreDataEntityWriter.h"
#include "coreDataEntityReader.h"
#include "coreFile.h"
#include "coreITKTransformImpl.h"
#include "itksys/SystemTools.hxx"
#include "blTextUtils.h"
#include "coreDirectory.h"

struct DigitsToCharacters
{
	char operator() (char in)
	{
		if (in >= 48 && in <= 57)
		{
			return in + 17;
		}

		return in;
	}
};

Core::DynDataTransferCLP::DynDataTransferCLP()
{
}

Core::DynDataTransferCLP::~DynDataTransferCLP()
{
}


void Core::DynDataTransferCLP::UpdateModuleParameter( 
	ModuleParameter* param, 
	int num )
{
	std::string pid;
	std::ostringstream pidString;

	// Encode process id into a string.  To avoid confusing the
	// Archetype reader, convert the numbers in pid to characters [0-9]->[A-J]
#ifdef _WIN32
	pidString << GetCurrentProcessId();
#else
	pidString << getpid();
#endif
	pid = pidString.str();
	std::transform(pid.begin(), pid.end(), pid.begin(), DigitsToCharacters());

	// To avoid confusing the Archetype readers, convert any
	// numbers in the filename to characters [0-9]->[A-J]
	std::string fname = param->GetName();
	std::transform(fname.begin(), fname.end(),
		fname.begin(), DigitsToCharacters());

	// By default, the filename is based on the temporary directory and
	// the pid
	fname = this->m_TemporaryDirectory + "/" + pid + "_" + fname;

	std::string extension;
	if ( param->GetFileExtensions().size( ) == 0 )
	{
		Core::DataEntityType type = Core::UnknownTypeId;
		if ( param->GetTag() == "image" )
		{
			type = Core::ImageTypeId;
		}
		else if ( param->GetTag() == "geometry" )
		{
			type = Core::SurfaceMeshTypeId;
		}
		else if ( param->GetTag() == "transform" )
		{
			type = Core::TransformId;
		}

		extension = Core::IO::DataEntityWriter::GetDefaultFileTypeForWrite( type );
		if ( extension.empty() )
		{
			throw Core::Exceptions::Exception("DynProcessor::GetInputFilename", 
				"Cannot find a proper extension to save input data" );
		}
	}
	else
	{
		extension = param->GetFileExtensions()[ 0 ];
	}

	std::string fullFilename = fname + extension;
	m_FilesToDelete.insert( fullFilename );
	m_FilesToDelete.insert( fname + ".gmi" );

	// Write input data to disk 
	if ( param->GetChannel() == "input" )
	{
		Core::IO::DataEntityWriter::Pointer writer = Core::IO::DataEntityWriter::New();
		writer->SetFileName( fullFilename );
		writer->SetInputDataEntity( 0, m_DynProcessor->GetInputPort( num )->GetDataEntity() );
		writer->Update( );
	}

	param->SetDefault( fullFilename );
}

void Core::DynDataTransferCLP::UpdateProcessorOutput( ModuleParameter* param, int num )
{
	if ( m_DynProcessor->GetModuleExecution()->GetSaveScript() )
	{
		return;
	}

	std::string filenames = SearchMultipleTimeSteps( param->GetDefault() );

	// Split default into single files using ";" as separator
	std::list<std::string> files;
	blTextUtils::ParseLine( filenames, ';', files );
	std::vector< std::string > filesVector;
	std::list<std::string>::iterator it;
	for ( it = files.begin() ; it != files.end() ; it++ )
	{
		filesVector.push_back( *it );
	}

	// Read data entity
	Core::IO::DataEntityReader::Pointer reader = Core::IO::DataEntityReader::New( );
	reader->SetFileNames( filesVector );
	reader->Update();
	if ( reader->GetNumberOfOutputs() == 0 )
	{
		throw Core::Exceptions::Exception("DynProcessor::GetInputFilename", 
			"Cannot load output data" );
	}

	Core::DataEntity::Pointer output = reader->GetOutputDataEntity( 0 );
	if ( output.IsNull() )
	{
		throw Core::Exceptions::Exception("DynProcessor::GetInputFilename", 
			"Cannot load output data" );
	}

	// Find input parent data
	Core::DataEntity::Pointer parentData = FindInputReferenceData( param );

	// UpdateOutput
	output->GetMetadata( )->SetName( param->GetName() );
	output->SetFather( parentData );
	if ( m_DynProcessor->GetOutputDataEntity( num ).IsNotNull( ) )
	{
		m_DynProcessor->GetOutputDataEntity( num )->Copy( output );
	}
	else
	{
		m_DynProcessor->SetOutputDataEntity( num, output );
	}
}

void Core::DynDataTransferCLP::CleanTemporaryFiles()
{
	if ( m_DynProcessor->GetModuleExecution()->GetSaveScript() )
	{
		return;
	}

	bool removed;
	std::set<std::string>::iterator fit;
	for (fit = m_FilesToDelete.begin(); fit != m_FilesToDelete.end(); ++fit)
	{
		if (itksys::SystemTools::FileExists((*fit).c_str()))
		{
			removed = itksys::SystemTools::RemoveFile((*fit).c_str());
			if (!removed)
			{
				std::stringstream information;
				information << "Unable to delete temporary file " << *fit << std::endl;
				std::cerr << information.str();
			}
		}
	}

}

std::string Core::DynDataTransferCLP::SearchMultipleTimeSteps( const std::string &filename )
{
	std::string addedFileNames;

	if ( filename.find( ";" ) != std::string::npos )
	{
		return filename;
	}

	// If file doesn't exist -> Check multiple time steps adding the 
	// number of the time step at the end
	if ( !itksys::SystemTools::FileExists( filename.c_str() ) )
	{
		std::string path = itksys::SystemTools::GetFilenamePath( filename );
		std::string fileName = itksys::SystemTools::GetFilenameWithoutExtension( filename );
		std::string ext = itksys::SystemTools::GetFilenameExtension( filename );

		// Read all files of the folder
		Core::IO::Directory::Pointer directory = Core::IO::Directory::New( );
		directory->SetDirNameFullPath( path );
		Core::IO::FileNameList list = directory->GetContents();
		Core::IO::FileNameList::iterator it;

		// Check for files with the same file name and extension
		bool found = false;
		for ( it = list.begin() ; it != list.end() ; it++ )
		{
			std::string curFileName = itksys::SystemTools::GetFilenameWithoutExtension( *it );
			curFileName = Core::IO::BaseIO::RemoveLastDigits( curFileName );
			std::string curExt = itksys::SystemTools::GetFilenameExtension( *it );
			if ( fileName == curFileName && ext == curExt )
			{
				addedFileNames.append( *it + ";" );
			}
		}
	}
	else
	{
		addedFileNames = filename;
	}

	return addedFileNames;
}
