/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkingAreaStorage.h"
#include "coreRenderWindowContainer.h"
#include "coreBaseWindow.h"
#include "coreBaseWindowFactories.h"
#include "coreWorkingAreaFactory.h"
#include "coreWxMitkGraphicalInterface.h"

#include "blXMLTagMapWriter.h"
#include "blXMLTagMapReader.h"
#include "blXMLBoostTagMapReader.h"

#include "itksys/SystemTools.hxx"

const std::string DEFAULT_WORKING_AREA = "OrthoSliceWorkingArea";
const std::string WORKING_AREA_FOLDER = "WorkingAreas";

Core::Widgets::WorkingAreaStorage::WorkingAreaStorage( ) 
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	m_WorkingAreaFolder = settings->GetProjectHomePath() + Core::IO::SlashChar + WORKING_AREA_FOLDER;
}

Core::Widgets::WorkingAreaStorage::~WorkingAreaStorage()
{

}

void Core::Widgets::WorkingAreaStorage::CreateFolder( const std::string &path )
{
	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( path );

	// Create dest dir
	workflowsDir->Create();

	// Scan source dir
	Core::IO::Directory::Pointer sourceWorkflowsDir = Core::IO::Directory::New();
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	sourceWorkflowsDir->SetDirNameFullPath( 
		settings->GetResourcePath() + Core::IO::SlashChar + WORKING_AREA_FOLDER );

	Core::IO::FileNameList list;
	list = sourceWorkflowsDir->GetContents( );

	Core::IO::FileNameList::iterator it;
	for ( it = list.begin() ; it != list.end() ; it++ )
	{
		std::string dest = path +  
			Core::IO::SlashChar + itksys::SystemTools::GetFilenameName( *it );
		if ( !itksys::SystemTools::FileExists( dest.c_str() ) )
		{
			itksys::SystemTools::CopyAFile( it->c_str(), dest.c_str() );
		}
	}
}

void Core::Widgets::WorkingAreaStorage::ScanDirectory( const std::string &projectPath )
{
	std::string path = projectPath + Core::IO::SlashChar + WORKING_AREA_FOLDER;

	CreateFolder( path );

	// Open the default working area first -> This will be the active one
	std::string filename = path + "/" + DEFAULT_WORKING_AREA + ".xml";
	Open( filename );

	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( path );

	Core::IO::FileNameList list;
	list = workflowsDir->GetContents( );

	Core::IO::FileNameList::iterator it;
	for ( it = list.begin() ; it != list.end() ; it++ )
	{
		if ( itksys::SystemTools::GetFilenameWithoutExtension( *it ) == DEFAULT_WORKING_AREA )
		{
			continue;
		}
		Open( *it );
	}

}

void Core::Widgets::WorkingAreaStorage::Save( 
	BaseWindow* workingArea )
{
	try
	{
		wxWindow* window = dynamic_cast<wxWindow*> ( workingArea );
		if ( !window )
		{
			return;
		}

		blTagMap::Pointer tagMap = blTagMap::New();
		workingArea->GetProperties( tagMap );

		blXMLTagMapWriter::Pointer writer = blXMLTagMapWriter::New();
		writer->SetFilename( GetConfigFileName( window->GetName().c_str() ).c_str() );
		writer->SetInput( tagMap );
		writer->Update();
	}
	coreCatchExceptionsLogAndNoThrowMacro(WorkingAreaStorage::Save);
}

wxWindow* Core::Widgets::WorkingAreaStorage::Open( 
	const std::string &path )
{

	wxWindow* window = NULL;

	try
	{

		if ( itksys::SystemTools::GetFilenameExtension( path ) != ".xml" ||
			 !itksys::SystemTools::FileExists( path.c_str() ) )
		{
			return NULL;
		}

		// Try Tiny XML reader
		blTagMap::Pointer tagMap;
		blXMLTagMapReader::Pointer xmlTagMapReader = blXMLTagMapReader::New( );
		xmlTagMapReader->SetFilename( path.c_str() );
		xmlTagMapReader->Update();
		tagMap = xmlTagMapReader->GetOutput();

		if ( tagMap.IsNull() )
		{
			// Try old boost reader
			blXMLBoostTagMapReader::Pointer xmlTagMapReader = blXMLBoostTagMapReader::New( );
			xmlTagMapReader->SetFilename( path.c_str() );
			xmlTagMapReader->Update();
			tagMap = xmlTagMapReader->GetOutput();
		}

		if ( tagMap.IsNull() )
		{
			throw Core::Exceptions::Exception( 
				"WorkingAreaStorage::Open", 
				"Cannot open working area configuration file" );
		}

		std::string name;
		blTag::Pointer tag = tagMap->FindTagByName( "Name" );
		if ( tag.IsNotNull() )
		{
			name = tag->GetValueCasted<std::string>( );

			blTag::Pointer tag = tagMap->FindTagByName( "Icon" );
			std::string bitmapFilename;
			if ( tag.IsNotNull() )
			{
				bitmapFilename = 
					itksys::SystemTools::GetFilenamePath( path ) + Core::IO::SlashChar + 
					tag->GetValueAsString( );
			}
			else
			{
				// Old bitmap filename was OrthoSliceWorkingArea.xpm
				bitmapFilename = GetBitmapFileNameFromConfigFile( path );
			}

			// If the file doesn't exist -> Use the default icon
			// A radio toolbar always needs an active tool
			if ( !itksys::SystemTools::FileExists( bitmapFilename.c_str() ) ||
				 itksys::SystemTools::FileIsDirectory( bitmapFilename.c_str() ) )
			{
				bitmapFilename = GetBitmapFileName( DEFAULT_WORKING_AREA );
			}


			Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
			gIface = Core::Runtime::Kernel::GetGraphicalInterface();
			BaseWindowFactory::Pointer factory;
			factory = WorkingAreaFactory::NewBase( "class Core::Widgets::" + name, tagMap );
			gIface->RegisterFactory( 
				factory,
				WindowConfig( ).WorkingArea().Id( wxNewId() ).Bitmap( bitmapFilename )
				.Caption( name ) );

			if ( m_DefaultWorkingArea.empty() )
			{
				m_DefaultWorkingArea = factory->GetName( );
			}
		}
	}
	coreCatchExceptionsLogAndNoThrowMacro( WorkingAreaStorage::OpenWorkingArea )

	return window;
}

std::string Core::Widgets::WorkingAreaStorage::GetConfigFileName( 
	const std::string &name )
{
	return m_WorkingAreaFolder + Core::IO::SlashChar + name + ".xml";
}

std::string Core::Widgets::WorkingAreaStorage::GetBitmapFileName( 
	const std::string &name )
{
	return m_WorkingAreaFolder + Core::IO::SlashChar + name + ".xpm";
}

void Core::Widgets::WorkingAreaStorage::Delete( BaseWindow* workingArea )
{
	wxWindow* window = dynamic_cast<wxWindow*> ( workingArea );
	if ( !window )
	{
		return;
	}

	Core::IO::File::Pointer file = Core::IO::File::New();
	file->SetFileNameFullPath( GetConfigFileName( window->GetName().c_str() ) );
	file->Delete();
	file->SetFileNameFullPath( GetBitmapFileName( window->GetName().c_str() ) );
	file->Delete();
}

std::string Core::Widgets::WorkingAreaStorage::GetWorkingAreaFolder()
{
	return m_WorkingAreaFolder;
}

void Core::Widgets::WorkingAreaStorage::SaveAll()
{
	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( m_WorkingAreaFolder );

	BaseWindowFactories::Pointer factories;
	factories = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = factories->GetWindowsList( WIDGET_TYPE_WORKING_AREA );

	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		Core::WindowConfig config;
		if ( ! factories->GetWindowConfig( *it, config ) )
		{
			continue;
		}

		std::string srcBitmap = config.GetBitmapFilename();
		std::string name = config.GetCaption();

		WorkingAreaFactory* workingAreaFactory;
		workingAreaFactory = dynamic_cast<WorkingAreaFactory*> ( 
			factories->FindFactory( *it ).GetPointer() );

		if ( workingAreaFactory )
		{
			std::string configFilename = GetConfigFileName( name );
			blTagMap::Pointer properties = workingAreaFactory->GetProperties();

			wxImage img;
			img.LoadFile( srcBitmap );

			if ( img.IsOk()  )
			{
				blTag::Pointer tag = properties->FindTagByName( "Icon" );
				std::string dstBitmap;
				if ( tag.IsNotNull() )
				{
					dstBitmap = tag->GetValueAsString();
				}
				else
				{
					// If no Icon is found, always use ".png" extension
					dstBitmap = itksys::SystemTools::GetFilenameWithoutExtension( srcBitmap ) + ".png";
				}
				properties->AddTag( "Icon", dstBitmap );

				img.SaveFile( 
					itksys::SystemTools::GetFilenamePath( configFilename.c_str() )
					+ Core::IO::SlashChar + dstBitmap );
			}


			blXMLTagMapWriter::Pointer writer = blXMLTagMapWriter::New();
			writer->SetFilename( configFilename.c_str() );
			writer->SetInput( properties );
			writer->Update();

		}
	}

}

std::string Core::Widgets::WorkingAreaStorage::GetBitmapFileNameFromConfigFile( 
	const std::string &configFileName )
{
	std::string path = itksys::SystemTools::GetFilenamePath( configFileName );
	std::string name = itksys::SystemTools::GetFilenameWithoutLastExtension( configFileName );
	return path + Core::IO::SlashChar + name + ".xpm";
}

std::string Core::Widgets::WorkingAreaStorage::GetDefaultWorkingAreaFactoryName()
{
	return m_DefaultWorkingArea;
}
