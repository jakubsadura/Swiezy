/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginDataTransferTaverna.h"
#include "itksys/SystemTools.hxx"
#include "ModuleDescriptionUtilities.h"
#include "coreDirectory.h"
#include "blTextUtils.h"

TavernaPlugin::DataTransferTaverna::DataTransferTaverna()
{
}

TavernaPlugin::DataTransferTaverna::~DataTransferTaverna()
{
}

void TavernaPlugin::DataTransferTaverna::UpdateProcessorOutput( ModuleParameter* param, int num )
{
	// Open result file
	std::string outputFileName = m_OutputDirectory + "/" + param->GetName();
	if ( itksys::SystemTools::FileIsDirectory( outputFileName.c_str() ) )
	{
		// Store all output files separated by ; and the number of the file
		std::string allOutputFiles;
		Core::IO::Directory::Pointer outputDir = Core::IO::Directory::New( );
		outputDir->SetDirNameFullPath( outputFileName );
		Core::IO::FileNameList files = outputDir->GetContents();
		Core::IO::FileNameList::iterator it;
		for ( it = files.begin( ) ; it != files.end( ) ; it++ )
		{
			std::ifstream outputFile( it->c_str() );
			if ( !outputFile.is_open() )
			{
				continue;
			}

			std::string outFilePath;
			outputFile >> outFilePath;
			blTextUtils::StrSub( outFilePath, "\"", "" );
			if ( !itksys::SystemTools::FileExists( outFilePath.c_str() ) )
			{
				continue;
			}

			// Get file path components
			std::string path = itksys::SystemTools::GetFilenamePath( param->GetDefault() );
			std::string fileName = itksys::SystemTools::GetFilenameWithoutExtension( param->GetDefault() );
			std::string ext = itksys::SystemTools::GetFilenameExtension( param->GetDefault() );
			std::string tavernaFileName = itksys::SystemTools::GetFilenameWithoutExtension( it->c_str() );
			int fileNumber = atoi( tavernaFileName.c_str() );

			// Create complete output file name path
			char gimiasOutputFile[ 2048 ];
			sprintf( gimiasOutputFile, "%s/%s%03d%s", path.c_str( ), fileName.c_str(), fileNumber, ext.c_str() );
			allOutputFiles.append( std::string( gimiasOutputFile ) + ";" );

			// Copy and remove intermediate file
			itksys::SystemTools::CopyAFile( outFilePath.c_str(), gimiasOutputFile );
			itksys::SystemTools::RemoveFile( outFilePath.c_str() );
		}

		param->SetDefault( allOutputFiles );

	}
	else if ( itksys::SystemTools::FileExists( outputFileName.c_str() ) )
	{
		std::ifstream outputFile( outputFileName.c_str() );
		if ( !outputFile.is_open() )
		{
			return;
		}

		// Move output files to parameter specification
		std::string outFilePath;
		outputFile >> outFilePath;
		blTextUtils::StrSub( outFilePath, "\"", "" );
		itksys::SystemTools::CopyAFile( outFilePath.c_str(), param->GetDefault().c_str() );
		itksys::SystemTools::RemoveFile( outFilePath.c_str() );
	}

	// Call parent class to load output
	DynDataTransferCLP::UpdateProcessorOutput( param, num );
}

void TavernaPlugin::DataTransferTaverna::UpdateAllOutputs()
{
	// Get name of output dir
	ModuleParameter* param = findModuleParameter( m_DynProcessor->GetModule(), "", 0, "", "directory" );
	if ( param == NULL )
	{
		throw Core::Exceptions::Exception( "UpdateAllOutputs", "Cannot find output directory parameter" );
	}

	m_OutputDirectory = m_DynProcessor->GetModuleExecution( )->GetWorkingDirectory( ); 
	m_OutputDirectory += "/" + param->GetDefault();

	// Update all outputs
	Core::DynDataTransferBase::UpdateAllOutputs();
}

void TavernaPlugin::DataTransferTaverna::CleanTemporaryFiles()
{
	DynDataTransferCLP::CleanTemporaryFiles();

	//itksys::SystemTools::RemoveADirectory( m_OutputDirectory.c_str() );
}

