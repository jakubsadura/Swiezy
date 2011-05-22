/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include <wx/wupdlock.h>

#include "coreMainMenu.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"
#include "coreLogger.h"
#include "coreAssert.h"
#include "coreReportExceptionMacros.h"
#include "coreBaseExceptions.h"
#include "coreSplashScreen.h"
#include "coreStyleManager.h"
#include "coreLogFileViewer.h"
#include "coreMacroPannelWidget.h"
#include "coreDirectory.h"
#include "coreDataEntityHelper.h"
#include "coreLoggerHelper.h"
#include "coreWxDataObjectDataEntity.h"
#include "coreMultiRenderWindow.h"
#include "coreStatusBar.h"
#include "coreDataEntityReader.h"
#include "coreDataEntityWriter.h"
#include "coreWorkflowManager.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreDataTreeHelper.h"
#include "coreUserRegistrationDialog.h"

#include "coreSessionWriter.h"
#include "coreSessionReader.h"

#include "coreMovieToolbar.h"
#include "coreProcessingToolboxWidget.h"
#include "coreSelectionToolboxWidget.h"

#include "coreDirectory.h"
#include "coreVTKProcessor.h"
#include "vtkImageCast.h"

#include "wxID.h"

#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/dnd.h>
#include <wx/utils.h>
#include <wx/clipbrd.h>
#include <wx/progdlg.h>

#include "wxMemoryUsageIndicator.h"
#include "wxUnicode.h"

#include "CGNSFileWriterDialog.h"
#include "coreCgnsFileWriter.h"
#include "coreVTKImageDataHolder.h"

#include <stdio.h>

#include "itksys/SystemTools.hxx"

// Only under Windows
#ifdef WIN32
	#include "coreFuraPopupWidget.h"
#endif

/** Definitions for handling shared libraries in Windows */
#if (defined(_WIN32) || defined(WIN32)) && !defined(SWIG_WRAPPING)
#	undef MAIN_WINDOW_EXPORT
#	define MAIN_WINDOW_EXPORT __declspec(dllexport)
#else
/* unix needs nothing */
#	undef  MAIN_WINDOW_EXPORT
#	define MAIN_WINDOW_EXPORT
#endif

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(MainMenu, wxMenuBar)
	EVT_MENU	(wxID_OpenDataMenuItem, MainMenu::OnMenuOpenFile)	
	EVT_MENU	(wxID_OpenDirectoryMenuItem, MainMenu::OnMenuOpenDirectory)	
	EVT_MENU	(wxID_SaveDataMenuItem, MainMenu::OnMenuSaveFile)
	EVT_MENU	(wxID_SaveImageMenuItem, MainMenu::OnMenuSaveImage)
	EVT_MENU	(wxID_OpenSessionMenuItem, MainMenu::OnMenuOpenSession)
	EVT_MENU	(wxID_SaveSessionMenuItem, MainMenu::OnMenuSaveSession)
	EVT_MENU	(wxID_ExitMenuItem, MainMenu::OnMenuExit)

	EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, MainMenu::OnMRUFile)

	EVT_MENU	(wxID_ImportConfigurationMenuItem, MainMenu::OnMenuImportConfiguration)
	EVT_MENU	(wxID_ShowLogMenuItem, MainMenu::OnMenuShowLog)
	EVT_MENU	(wxID_Preferences, MainMenu::OnMenuPreferences)
	
	EVT_MENU	(wxID_ReportBug, MainMenu::OnMenuReportBug)
	EVT_MENU	(wxID_UserRegistration, MainMenu::OnMenuUserRegistration)
	EVT_MENU	(wxID_DevelopersSite, MainMenu::OnMenuDevelopersSite)
	EVT_MENU	(wxID_FrameworkVideos, MainMenu::OnMenuFrameworkVideos)
	
	EVT_MENU	(wxID_ShowAboutMenuItem, MainMenu::OnMenuShowAbout)
	
	EVT_MENU	(wxID_CopyMenuItem, MainMenu::OnMenuCopy)	
	EVT_MENU	(wxID_PasteMenuItem, MainMenu::OnMenuPaste)	
	
	EVT_MENU	(wxID_WindowResetLayoutMenuItem, MainMenu::OnMenuWindowResetLayout)	
	EVT_MENU	(wxID_WindowMaximizeWorkingAreaMenuItem, MainMenu::OnMenuWindowMaximizeWorkingAreaMenuItem)	

	EVT_MENU	(wxID_WorkflowManagerMenuItem,MainMenu::OnMenuItemWorkflowManager)
	EVT_MENU	(wxID_PerspectivePluginMenuItem,MainMenu::OnMenuItemPerspectivePlugin)
	EVT_MENU	(wxID_PerspectiveWorkflowMenuItem,MainMenu::OnMenuItemPerspectiveWorkflow)

	EVT_MENU	(wxID_ShowAllToolbarsMenuItem, MainMenu::OnMenuShowAllToolbars)	
	EVT_MENU	(wxID_HideAllToolbarsMenuItem, MainMenu::OnMenuHideAllToolbars)	

END_EVENT_TABLE();

// non recursive search
wxMenuItem *FindChildItem( wxMenu* menu, const wxString& text)
{
	wxString label = wxMenuItem::GetLabelFromText(text);

	wxMenuItemList items = menu->GetMenuItems( );
	wxMenuItem *item = (wxMenuItem *)NULL;
	for ( wxMenuItemList::compatibility_iterator node = items.GetFirst();
		node;
		node = node->GetNext() )
	{
		wxMenuItem *item = node->GetData();
		if ( !item->IsSeparator() )
		{
			if ( item->GetLabel() == label )
				return item;
		}
	}

	return item;
}



MainMenu::MainMenu() : wxMenuBar()
{
	CreateMainMenu( );

	Initialize();
}


MainMenu::~MainMenu()
{
	// Save history file list
	std::vector<std::string> strList;
	for ( unsigned i = 0 ; i < m_FileHistory->GetCount() ; i++ )
	{
		strList.push_back( _U(m_FileHistory->GetHistoryFile( i )) );
	}
	Core::Runtime::Kernel::GetApplicationSettings()->SetMRUList( strList );

	if ( m_FileHistory )
		delete m_FileHistory;
}

/** 
*/
void MainMenu::CreateMainMenu(void)
{
	try
	{
		wxMenu* menu;

		// The file menu
		menu = new wxMenu();
		this->Append(menu, wxT("&File"));
		menu->Append(wxID_OpenDataMenuItem, wxT("&Open data file\tCtrl+O"), 
			wxT("Open a data file, and load it as a new data entity to work with"), wxITEM_NORMAL);
		menu->Append(wxID_OpenDirectoryMenuItem, wxT("Open &directory\tCtrl+D"), 
			wxT("Open a directory (DICOM)"), wxITEM_NORMAL);

		menu->Append(wxID_SaveDataMenuItem, wxT("&Save selected data\tCtrl+S"), 
			wxT("Save to a file the data entity that is currently being selected at the Processing Browser"), wxITEM_NORMAL);
		menu->Append(wxID_SaveImageMenuItem, wxT("&Save image of current 3D view"), 
			wxT("Save an Image of the data entity that is currently rendered in the 3D view"), wxITEM_NORMAL);
		menu->AppendSeparator();

		menu->Append(wxID_OpenSessionMenuItem, wxT("&Open session"), 
			wxT("Read all data saved in session"), wxITEM_NORMAL);
		menu->Append(wxID_SaveSessionMenuItem, wxT("&Save current session"), 
			wxT("Save all data to session"), wxITEM_NORMAL);

		menu->AppendSeparator();
		menu->Append(wxID_WorkflowManagerMenuItem, wxT("Workflow Manager"), wxT("Workflow Manager"));
		wxMenu *subMenu = new wxMenu();
		subMenu->Append(wxID_PerspectivePluginMenuItem, wxT("Plugin"), wxT("Plugin"), wxITEM_RADIO);
		subMenu->Append(wxID_PerspectiveWorkflowMenuItem, wxT("Workflow"), wxT("Workflow"), wxITEM_RADIO);
		menu->Append(wxID_PerspectiveMenuItem, wxT("&Perspective"), subMenu );


		menu->AppendSeparator();
		menu->Append(wxID_ExitMenuItem, wxT("E&xit"), wxT("Exits the application"), wxITEM_NORMAL);

		// Create file history
		m_FileHistory = new wxFileHistory;
		m_FileHistory->UseMenu( menu );

		// Fill history file list
		std::vector<std::string> strList;
		strList = Core::Runtime::Kernel::GetApplicationSettings()->GetMRUList( );
		// Start with the last
		for ( int i = int( strList.size() ) - 1 ; i >= 0  ; i-- )
		{
			if ( FileExists(strList[i]) )
				m_FileHistory->AddFileToHistory( _U(strList[ i ]) );
		}

		
		// Edit menu
		menu = new wxMenu();
		this->Append(menu, wxT("&Edit"));
		menu->Append(wxID_ImportConfigurationMenuItem, wxT("Import Configuration"), 
			wxT("Import configuration from previous versions"), wxITEM_NORMAL);
		menu->Append(wxID_ShowLogMenuItem, wxT("View &Log file"), 
			wxT("Shows the log file of the application"), wxITEM_NORMAL);
		menu->Append(wxID_CopyMenuItem, wxT("&Copy timepoint\tCtrl+C"), 
			wxT("Copy the current timepoint of the selected data"), wxITEM_NORMAL);
		menu->Append(wxID_PasteMenuItem, wxT("&Paste timepoint\tCtrl+V"), 
			wxT("Paste to the selected data timepoint"), wxITEM_NORMAL);
		menu->Append(wxID_Preferences, wxT("Preferences"), 
			wxT("Preferences"), wxITEM_NORMAL);

		//View menu
		menu = new wxMenu();
		this->Append(menu, wxT("&View"));
		menu->Append(wxID_WindowResetLayoutMenuItem, wxT("Reset layout"), 
			wxT("Reset layout"));
		menu->Append(wxID_WindowMaximizeWorkingAreaMenuItem, wxT("Maximize working area\tF11"), 
			wxT("Maximize working area"));

		subMenu = new wxMenu();
		menu->AppendSeparator();
		menu->Append(wxID_ANY, wxT("Toolbars"), subMenu);
		subMenu->Append(wxID_ShowAllToolbarsMenuItem, wxT("Show all"), 
			wxT("Show all toolbars"));
		subMenu->Append(wxID_HideAllToolbarsMenuItem, wxT("Hide all"), 
			wxT("Hide all toolbars"));

		// Tools menu
		menu = new wxMenu();
		this->Append(menu, wxT("&Tools"));

		menu = new wxMenu();
		this->Append(menu, wxT("&Selection"));


		// Help menu
		menu = new wxMenu();
		this->Append(menu, wxT("&Help"));
		menu->Append(wxID_UserRegistration, wxT("&User registration"), wxT("User registration"), wxITEM_NORMAL);
		menu->Append(wxID_ReportBug, wxT("&Report Bug or Enhancement"), wxT("Report Bug or Enhancement"), wxITEM_NORMAL);
		menu->AppendSeparator();
		menu->Append(wxID_DevelopersSite, wxT("&Developer resources"), wxT("Developer resources"), wxITEM_NORMAL);
		menu->Append(wxID_FrameworkVideos, wxT("&Watch Videos"), wxT("Watch Videos"), wxITEM_NORMAL);
		menu->AppendSeparator();
		menu->Append(wxID_ShowAboutMenuItem, wxT("&About"), wxT("About this program"), wxITEM_NORMAL);
		

	}
	coreCatchExceptionsAddTraceAndThrowMacro(MainMenu::CreateMainMenu)
}

void MainMenu::Initialize()
{
	//// Main initialization ////
	try
	{

		BaseWindowFactories::Pointer baseWindowFactory;
		baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
		baseWindowFactory->GetFactoriesHolder()->AddObserver(
			this,
			&MainMenu::UpdateRegisteredWindowsMenuItems );
        
		// Update menu items from settings
		Core::Runtime::PERSPECTIVE_TYPE type;
		type = Core::Runtime::Kernel::GetApplicationSettings()->GetPerspective( );
		switch( type )
		{
		case Core::Runtime::PERSPECTIVE_PLUGIN:this->Check( wxID_PerspectivePluginMenuItem, true );break;
		case Core::Runtime::PERSPECTIVE_WORKFLOW:this->Check( wxID_PerspectiveWorkflowMenuItem, true );break;
		}

        SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);

	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::MainMenu);
}

//!
void MainMenu::OnMenuOpenFile(wxCommandEvent& event)
{
	try
	{	
		// Open file dialog
		std::string filetypes = Core::IO::DataEntityReader::GetFileFilterTypesForRead();
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath;
		wxFileDialog* openFileDialog = 
			new wxFileDialog(this, wxT("Open a data file"), wxT(""), wxT(""), wxT(""), 
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxMULTIPLE );
		openFileDialog->SetWildcard(_U(filetypes));

		// Default value
		openFileDialog->SetDirectory(_U(settings->GetCurrentDataPath().c_str()));

		if(openFileDialog->ShowModal() == wxID_OK)
		{
			wxArrayString wxFilenames;
			openFileDialog->GetFilenames( wxFilenames );
			dataPath = _U(openFileDialog->GetDirectory());

			std::vector<std::string> completePath;
			for ( unsigned i = 0 ; i < wxFilenames.size() ; i++ )
			{
				completePath.push_back( dataPath + Core::IO::SlashChar + 
					_U(wxFilenames[ i ]) );
			}

			wxTheApp->Yield(true);
			LoadDataEntity( completePath );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuOpenFile)
}

void Core::Widgets::MainMenu::OnMenuOpenDirectory( wxCommandEvent& event )
{
	std::string dirPath(event.GetString().mb_str());
	
	wxDirDialog* openDirectoryDialog = new wxDirDialog( 
		this, wxT("Open directory"), wxT(""));

	try
	{	
		if(dirPath.compare("")==0)
		{

			Core::Runtime::Settings::Pointer settings;
			settings = Core::Runtime::Kernel::GetApplicationSettings();
			std::string dataPath = settings->GetCurrentDataPath();

			openDirectoryDialog->SetPath(_U(dataPath));
			if(openDirectoryDialog->ShowModal() == wxID_OK)
			{
				dirPath = openDirectoryDialog->GetPath().mb_str(wxConvUTF8);
				if(dirPath.compare("") == 0)
				{
					throw Core::Exceptions::Exception( 
						"OnMenuOpenDirectory",
						"Provide a valid directory path" );
				}
			}
		}
		else //directory specified by the caller..thus we have to check that it exists!
		{
			//check if dirpath exists
			Core::IO::Directory::Pointer dirToOpen = Core::IO::Directory::New();
			dirToOpen->SetDirNameFullPath(dirPath);
			if(!dirToOpen->Exists())
			{
				throw Core::Exceptions::Exception( 
					"OnMenuOpenDirectory",
					"Provide a valid directory path" );
			}
		}

		wxTheApp->Yield(true);
		std::vector<std::string> completePath;
		completePath.push_back( dirPath );
		LoadDataEntity( completePath );

	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuOpenDirectory)

	openDirectoryDialog->Destroy();
}

//!
void MainMenu::OnMenuSaveFile(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		Core::DataContainer::Pointer container = Core::Runtime::Kernel::GetDataContainer();
		coreAssertMacro(container.IsNotNull());
		Core::DataEntity::Pointer dataEntity = 
			container->GetDataEntityList( )->GetSelectedDataEntityHolder()->GetSubject();
		
		if(dataEntity.IsNull())
		{
			throw Exceptions::Exception("OnMenuSaveFile",
				"First you have to select a data entity from the Processing browser");
		}

		bool isCGNS = false;
		blTag::Pointer tag = dataEntity->GetMetadata()->FindTagByName( "IsCGNS" );
		if ( tag.IsNotNull() )
		{
			tag->GetValue( isCGNS );
		}

		if ( isCGNS )
		{
			Core::IO::CGNSFileWriter::GenerateMetadata( dataEntity );

			//Ask the user the scalars and vectors to write.
			CGNSFileWriterDialog* writerDialog = new CGNSFileWriterDialog( );
			writerDialog->SetProperties( dataEntity->GetMetadata().GetPointer() );

			if ( writerDialog->ShowModal() == wxID_OK )
			{
				writerDialog->GetProperties( dataEntity->GetMetadata().GetPointer() );
			}
			else 
			{
				return;
			}
		}

		std::string filetypes = Core::IO::DataEntityWriter::GetFileFilterTypesForWrite(dataEntity->GetType());
		wxFileDialog* saveFileDialog = new wxFileDialog(this, wxT("Save data to file"), 
								wxT(""), wxT(""), wxT(""), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		saveFileDialog->SetDirectory(_U(settings->GetLastSavePath()));
		saveFileDialog->SetWildcard(_U(filetypes));
		saveFileDialog->SetFilename( wxString::FromAscii(dataEntity->GetMetadata( )->GetName().c_str()) );
		if(saveFileDialog->ShowModal() == wxID_OK)
		{
			std::string fileName = _U(saveFileDialog->GetFilename());
			std::string dataPath = _U(saveFileDialog->GetDirectory());
			std::string fileNameWithoutExtension;
			fileNameWithoutExtension.append( fileName,0, fileName.find_last_of("."));
			if ( fileName == "" )
			{
				throw Exceptions::Exception("OnMenuSaveFile", 
					"Provide a valid file name for saving a data file");
				return;
			}

			// If DICOM and FLOAT or DOUBLE -> Convert to INT
			std::string ext = itksys::SystemTools::GetFilenameLastExtension( fileName );
			std::string extension = itksys::SystemTools::LowerCase(ext);
			if ( extension == ".dcm" )
			{
				Core::vtkImageDataPtr image;
				bool worked = dataEntity->GetProcessingData( image );
				if( worked && image != NULL && 
					( image->GetScalarType() == VTK_FLOAT || image->GetScalarType() == VTK_DOUBLE ) )
				{
					std::string message = "Float or double DICOM data is not supported. Do you want to convert it to int?";
					std::string caption = "DICOM data";
					wxMessageDialog* question = new wxMessageDialog(GetParent(), _U(message.c_str()),
						_U(caption.c_str()),
						wxYES_NO | wxICON_QUESTION | wxSTAY_ON_TOP);
					switch(question->ShowModal())
					{
					case wxID_YES: 
						{
						VTKProcessor<vtkImageCast>::Pointer processor;
						processor = VTKProcessor<vtkImageCast>::New();
						processor->GetFilter( )->SetOutputScalarTypeToInt( );
						processor->SetInputDataEntity( 0, dataEntity );
						processor->GetOutputPort( 0 )->SetReuseOutput( true );
						processor->Update( );
						dataEntity = processor->GetOutputDataEntity( 0 );
						}
						break;
					case wxID_NO: 
						return; 
						break;
					}

				}
			}

			// If the data entity has more than one time step or has at least one child, create a folder
			// even if the user doesn't ask for it
			if ( dataEntity->GetNumberOfTimeSteps()>1)
			{
				//TODO: check if the folder already exists
				dataPath = dataPath + Core::IO::SlashChar + fileNameWithoutExtension;
				int overwriteExistingFolder = 1;
				if (boost::filesystem::exists(dataPath))
				{
					wxMessageDialog* overwriteExistingFolderDialog =
						new wxMessageDialog(this, wxT("Overwrite existing foder?"),wxT("Ovewrite"),	wxYES_NO /*| wxCANCEL*/);
				
					switch(overwriteExistingFolderDialog->ShowModal())
					{
					case wxID_YES:{ 
						boost::filesystem::create_directory(dataPath);
						//clean the screen before saving
						wxTheApp->Yield();
						Core::DataEntityHelper::SaveDataEntity( 
							dataPath + Core::IO::SlashChar + fileName, 	
							dataEntity, 
							GetCurrentPluginTab()->GetRenderingTreeManager().GetPointer() );
						break;}

					case wxID_NO:  
						break;
					}	// end switch
				} 
				else  // else if (boost::filesystem::exists(dataPath))
				{
					boost::filesystem::create_directory(dataPath);
						//clean the screen before saving
					wxTheApp->Yield();
					Core::DataEntityHelper::SaveDataEntity( 
						dataPath + Core::IO::SlashChar + fileName, 	
						dataEntity,
						GetCurrentPluginTab()->GetRenderingTreeManager().GetPointer() );
				} // end if (boost::filesystem::exists(dataPath))

			} //end ( dataEntity->GetNumberOfTimeSteps()>1)

			else
				Core::DataEntityHelper::SaveDataEntity( 
					dataPath + Core::IO::SlashChar + fileName, 
					dataEntity,
					GetCurrentPluginTab()->GetRenderingTreeManager().GetPointer() );

			settings->SetLastSavePath( dataPath );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuSaveFile)

}

//!
void MainMenu::OnMenuSaveImage(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		//coreAssertMacro(PluginTab.IsNotNull);
		std::string filetypes = ".png";
		wxFileDialog* saveFileDialog = new wxFileDialog(this, wxT("Save Image of 3D view"), 
				wxT(""), wxT(""), wxT(""), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			saveFileDialog->SetDirectory(_U(settings->GetCurrentDataPath()));
			saveFileDialog->SetWildcard(_U(filetypes));
		if(saveFileDialog->ShowModal() == wxID_OK)
			{
				std::string fileName = _U(saveFileDialog->GetFilename());
				std::string dataPath = _U(saveFileDialog->GetDirectory());
				if(fileName.compare("") != 0)
				{
					bool saved = false;
			
					try
					{	
						const std::string& completeFileName = dataPath + Core::IO::SlashChar + fileName;
						GetCurrentMultiRenderWindow()->SaveImage3DWindow( completeFileName );
						saved = true;
					}
					coreCatchExceptionsReportAndNoThrowMacro(
						MainMenu::OnMenuSaveImage);

					if(!saved)
						throw Exceptions::Exception(
							"OnMenuSaveImage", 
							"The Image could not be saved or either the application \n" \
							"was not able to find a suitable writer for that format.");
				}
				else
					throw Exceptions::Exception("OnMenuSaveImage",
						"Provide a valid file name for opening a data file");
			}
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuSaveImage)
}


//!
void MainMenu::OnMenuExit(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		graphicalIface->GetMainWindow()->CloseWindow();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuExit)
}

//!
void MainMenu::OnMenuShowLog(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		graphicalIface->GetMainWindow()->ShowLogFileBrowser();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuShowLog)
}

void Core::Widgets::MainMenu::OnMenuImportConfiguration( wxCommandEvent& event )
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		graphicalIface->GetMainWindow()->ShowImportConfigurationWizard();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuImportConfiguration)
}

//!
void MainMenu::OnMenuShowAbout(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		graphicalIface->GetMainWindow()->ShowAboutScreen();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMenuShowAbout)
}


//!
void MainMenu::OnMRUFile(wxCommandEvent& event)
{
	try
	{	
		wxString f(m_FileHistory->GetHistoryFile(event.GetId() - wxID_FILE1));

		std::vector<std::string> pathFilenames;
		pathFilenames.push_back( _U(f) );

		LoadDataEntity( pathFilenames );
	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::OnMRUFile)
}

void MainMenu::LoadDataEntity( 
	std::vector<std::string> pathFilenames )
{
	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_PROCESSING );

	// First process all the pending messages
	wxTheApp->Yield(true);

	wxWindowUpdateLocker noUpdates( this );

	try
	{	

		// PreLoad data entity
		blTagMap::Pointer tagMap;
		tagMap = Core::DataEntityHelper::PreLoadDataEntityVector( pathFilenames );

		ShowReaderHeaderDialog( tagMap );


		// Load data entity
		std::vector<Core::DataEntity::Pointer> dataEntityVector;
		dataEntityVector = Core::DataEntityHelper::LoadDataEntityVector( pathFilenames, tagMap );

		for ( unsigned i = 0 ; i < dataEntityVector.size() ; i++ )
		{
			OnLoadSingleDataEntity( 
				dataEntityVector[ i ] );
		}

		// Show message
		Core::LoggerHelper::ShowMessage(
			"File imported successfully", 
			Core::LoggerHelper::MESSAGE_TYPE_INFO, false);

		// Add files to history
		m_FileHistory->AddFileToHistory( _U( pathFilenames[ 0 ] ) );

		// Set last opened path
		Core::IO::File::Pointer file = Core::IO::File::New( );
		file->SetFileNameFullPath( pathFilenames[ 0 ] );
		Core::Runtime::Kernel::GetApplicationSettings()->SetLastOpenedPath(
			file->GetPathToFile() );

	}
	coreCatchExceptionsReportAndNoThrowMacro(MainMenu::LoadDataEntity);

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );

}

void Core::Widgets::MainMenu::OnMenuCut( wxCommandEvent& event )
{

}

void Core::Widgets::MainMenu::OnMenuCopy( wxCommandEvent& event )
{
	try
	{

		if (wxTheClipboard->Open())
		{

			Core::DataEntity::Pointer selectedDataEntity;
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			selectedDataEntity = dataContainer->GetDataEntityList( )->GetSelectedDataEntity( );
			
			Core::DataEntity::Pointer newDataEntity;

			if ( selectedDataEntity.IsNotNull( ) )
			{
				// Build a new data entity using the current time step
				boost::any	processingData;
				processingData = selectedDataEntity->GetProcessingData( GetCurrentPluginTimeStep( ) );

				newDataEntity = Core::DataEntity::New( selectedDataEntity->GetType() );
				newDataEntity->GetMetadata()->SetName( "Copy" );
				newDataEntity->AddTimeStep( processingData );

				// This data objects are held by the clipboard, 
				// so do not delete them in the app.
				wxDataObjectDataEntity* data = new wxDataObjectDataEntity( );
				data->SetDataEntity( newDataEntity );
				wxTheClipboard->SetData( data );
			}

			wxTheClipboard->Close();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( OnMenuCopy );

}

void Core::Widgets::MainMenu::OnMenuPaste( wxCommandEvent& event )
{

	try
	{

		if (wxTheClipboard->Open())
		{
			if ( wxTheClipboard->IsSupported( wxDataFormat( Core::wxDataObjectDataEntity::GetFormatId( ) ) ) )
			{
				wxDataObjectDataEntity data;
				bool bRes = wxTheClipboard->GetData( data );
				if ( bRes )
				{
					// Get selected data entity
					Core::DataEntity::Pointer selectedDataEntity;
					Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
					selectedDataEntity = dataContainer->GetDataEntityList( )->GetSelectedDataEntity( );

					// Set the time step to the selected data entity
					if ( selectedDataEntity.IsNotNull( ) && 
						selectedDataEntity->GetType( ) == data.GetDataEntity( )->GetType( ) )
					{
						Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
						gIface = Core::Runtime::Kernel::GetGraphicalInterface();

						Core::DataEntityImpl::Pointer impl;
						impl = selectedDataEntity->GetTimeStep( GetCurrentPluginTimeStep( ) );
						impl->DeepCopy( data.GetDataEntity( )->GetProcessingData( 0 ) );

						// Update the rendering tree with the new data
						selectedDataEntity->Modified();

						GetCurrentMultiRenderWindow()->RequestUpdateAll();
					}
				}
			}  
			wxTheClipboard->Close();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( OnMenuPaste );

}


void Core::Widgets::MainMenu::OnMenuWindowResetLayout( wxCommandEvent& event )
{
	try
	{
		GetCurrentPluginTab()->ResetLayout();
	}
	coreCatchExceptionsReportAndNoThrowMacro( "OnMenuWindowResetLayout" )
}

void Core::Widgets::MainMenu::UpdateMenus()
{
	UpdateViewMenuItems();
	UpdateRegisteredWindowsMenuItems();

}

void Core::Widgets::MainMenu::OnMenuWindowMaximizeWorkingAreaMenuItem( wxCommandEvent& event )
{
	// If the focus is inside a text control -> Don't do anything
	wxWindow *window = wxWindow::FindFocus();
	wxTextCtrl *textControl = dynamic_cast<wxTextCtrl *> ( window );
	if ( textControl != NULL )
	{
		return;
	}

	GetCurrentPluginTab()->MaximizeWindow( wxID_WorkingArea );
}

Core::Widgets::RenderWindowBase* Core::Widgets::MainMenu::GetCurrentMultiRenderWindow()
{
	if ( GetCurrentPluginTab() == NULL )
	{
		return NULL;
	}
	return GetCurrentPluginTab()->GetWorkingAreaManager( )->GetActiveMultiRenderWindow( );
}

blTagMap::Pointer Core::Widgets::MainMenu::GetCurrentMultiRenderWindowState()
{
	if ( GetCurrentPluginTab() == NULL )
	{
		return NULL;
	}
	return GetCurrentPluginTab()->GetWorkingAreaManager( )->GetActiveWindowMetadata( );
}

void Core::Widgets::MainMenu::OnLoadSingleDataEntity(
	Core::DataEntity::Pointer dataEntity )
{
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityHolder::Pointer selectedDataEntity;
	selectedDataEntity = dataContainer->GetDataEntityList( )->GetSelectedDataEntityHolder();

	if ( dataEntity->IsROI()  && dataEntity->GetFather().IsNull() )
	{
		if ( selectedDataEntity.IsNotNull( ) && 
			selectedDataEntity->GetSubject().IsNotNull( ) && 
			selectedDataEntity->GetSubject()->GetType( ) == ImageTypeId )
		{
			dataEntity->SetFather( selectedDataEntity->GetSubject() );
		}
		else
		{
			std::string errorMessage = "You must select an image to open a Region of Interest";
			throw Core::Exceptions::Exception( 
				"wxMitkCoreMainWindow::LoadDataEntity", 
				errorMessage.c_str() );
		}
	}

	// Add to data list and rendering tree
	RenderingTreeManager::Pointer renderingTreeManager;
	renderingTreeManager = GetCurrentPluginTab( )->GetRenderingTreeManager();
	Core::DataEntityHolder::Pointer holder = Core::DataEntityHolder::New( );
	holder->SetSubject( dataEntity );
	RenderingTree::Pointer renderingTree;
	renderingTree = renderingTreeManager->GetActiveTree( );
	Core::DataTreeHelper::PublishOutput( 
		holder, 
		renderingTree,
		true,
		true);

}

/**
*/
bool MainMenu::FileExists( std::string strFilename )
{
	if ( strFilename.empty( ) )
	{
		return false;
	}

	if (FILE * file = fopen(strFilename.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	return false;
}


int Core::Widgets::MainMenu::GetCurrentPluginTimeStep()
{
	if ( GetCurrentPluginTab( ) == NULL )
	{
		return 0;
	}

	MovieToolbar* movieToolbar;
	GetCurrentPluginTab( )->GetWidget( wxID_MovieToolbar, movieToolbar );

	Core::IntHolderType::Pointer timeStep;
	timeStep = movieToolbar->GetCurrentTimeStep();
	return timeStep->GetSubject( );
}

Core::Widgets::PluginTab* Core::Widgets::MainMenu::GetCurrentPluginTab( void )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	return graphicalIface->GetMainWindow()->GetCurrentPluginTab();
}

void Core::Widgets::MainMenu::SetPluginTab( Core::Widgets::PluginTab* val )
{

	// Add observer to current state of plugin tab
	if ( GetCurrentPluginTab( ) )
	{
		m_PluginTabConnection.disconnect( );

		m_PluginTabConnection = GetCurrentPluginTab( )->GetWindowsMapHolder()->AddObserver( 
			this,
			&Core::Widgets::MainMenu::UpdateMenus );

		if ( GetCurrentPluginTab( )->GetWorkingAreaManager( ) )
		{
			m_CurrentMultiRenderWindowConnection.disconnect();
			Core::Widgets::RenderWindowHolderType::Pointer holder;
			holder = GetCurrentPluginTab( )->GetWorkingAreaManager( )->GetActiveMultiRenderWindowHolder();
			m_CurrentMultiRenderWindowConnection = 
				holder->AddObserver1( 
				this, 
				&Core::Widgets::MainMenu::SetActiveMultiRenderWindow );
		}

	}

	UpdateViewMenuItems( );
	UpdateRegisteredWindowsMenuItems( );
}

Core::BaseProcessor::Pointer Core::Widgets::MainMenu::GetProcessor()
{
	return NULL;
}

void Core::Widgets::MainMenu::UpdateRegisteredWindowsMenuItems( )
{
	UpdateRegisteredWindowsMenu( 
		"Tools",
		WIDGET_TYPE_PROCESSING_TOOL, 
		wxCommandEventHandler(MainMenu::OnProcessingToolsMenu) );

	UpdateRegisteredWindowsMenu( 
		"Selection",
		WIDGET_TYPE_SELECTION_TOOL, 
		wxCommandEventHandler(MainMenu::OnSelectionToolsMenu ) );
}

void Core::Widgets::MainMenu::UpdateRegisteredWindowsMenu(
	wxString menuName,
	WIDGET_TYPE state,
	wxObjectEventFunction func )
{
	int menuId = FindMenu( menuName );
	if ( menuId == wxNOT_FOUND )
	{
		return;
	}
	wxMenu* toolsMenu = GetMenu( menuId );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( state );

	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		Core::WindowConfig config;
		if ( ! baseWindowFactory->GetWindowConfig( *it, config ) )
		{
			continue;
		}

		// Get category menu or the main menu
		wxMenu* categoryMenu;
		if ( !config.GetCategory().empty() )
		{
			// Find category, or create it
			wxMenuItem *menuItem = FindChildItem( toolsMenu, config.GetCategory() );
			if ( !menuItem )
			{
				menuItem = toolsMenu->Append(
					wxNewId(), wxT(config.GetCategory()), 
					new wxMenu( ), wxT(config.GetCategory()) );
			}
			categoryMenu = menuItem->GetSubMenu();
		}
		else
		{
			categoryMenu = toolsMenu;
		}

		// Find item, or create it
		int windowMenuId = categoryMenu->FindItem( config.GetCaption() );
		if ( windowMenuId == wxNOT_FOUND )
		{
			wxWindowID id = wxNewId();
			categoryMenu->Append(
				id, 
				config.GetCaption(), 
				config.GetCaption(), 
				wxITEM_NORMAL);

			// Connect handler
			Connect(
				id,
				wxEVT_COMMAND_MENU_SELECTED,
				func
				);
		}
	}
}

void Core::Widgets::MainMenu::UpdateViewMenuItems()
{
	// Get view menu
	int menuId = FindMenu( "View" );
	if ( menuId == wxNOT_FOUND || GetCurrentPluginTab( ) == NULL )
	{
		return;
	}
	wxMenu* viewMenu = GetMenu( menuId );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	PluginTab::WindowMapType windowsMap;
	windowsMap = GetCurrentPluginTab( )->GetAllWindows();

	// Add toolbar menu
	PluginTab::WindowMapType::iterator it;
	for ( it = windowsMap.begin() ; it != windowsMap.end() ; it++ )
	{
		Core::BaseWindow* baseWindow = dynamic_cast<Core::BaseWindow*> ( it->second );
		std::string factoryName = baseWindow->GetFactoryName();
		Core::WindowConfig config;
		if ( !baseWindowFactory->GetWindowConfig( factoryName, config ) )
		{
			continue;
		}

		// Don't add command panel windows
		if ( config.GetCommandPanel( ) )
		{
			continue;
		}

		// Add category menu
		wxMenu* categoryMenu;
		if ( !config.GetCategory().empty() )
		{
			// Find category, or create it
			wxMenuItem *menuItem = FindChildItem( viewMenu, config.GetCategory() );
			if ( !menuItem )
			{
				menuItem = viewMenu->Append(
					wxNewId(), wxT(config.GetCategory()), 
					new wxMenu( ), wxT(config.GetCategory()) );
			}
			categoryMenu = menuItem->GetSubMenu();
		}
		else
		{
			categoryMenu = viewMenu;
		}

		// Find item, or create it
		wxWindowID windowMenuId = categoryMenu->FindItem( it->second->GetName() );
		if ( windowMenuId == wxNOT_FOUND )
		{
			windowMenuId = wxNewId();
			categoryMenu->Append(
				windowMenuId, 
				it->second->GetName(), 
				it->second->GetName(), 
				wxITEM_CHECK);

			// Connect handler
			Connect(
				windowMenuId,
				wxEVT_COMMAND_MENU_SELECTED,
				wxCommandEventHandler(MainMenu::OnAutomatedMenu)
				);
		}

		this->Check( windowMenuId, GetCurrentPluginTab( )->IsWindowShown( it->first ) );
	}

}

void Core::Widgets::MainMenu::OnAutomatedMenu( wxCommandEvent& event )
{
	wxMenuItem* item = FindItem( event.GetId() );
	GetCurrentPluginTab()->ShowWindow( item->GetItemLabel(), event.IsChecked() );
}

void Core::Widgets::MainMenu::OnProcessingToolsMenu( wxCommandEvent& event )
{
	if ( !GetCurrentPluginTab() )
	{
		return;
	}

	wxMenuItem* item = FindItem( event.GetId() );
	try
	{
		// open the tools panel widget 
		ProcessingToolboxWidget* processingToolboxWidget;
		GetCurrentPluginTab()->GetWidget( wxID_ProcessingToolboxWidget, processingToolboxWidget );
		processingToolboxWidget->SetToolByName( item->GetItemLabel().c_str() );
		GetCurrentPluginTab()->ShowWindow( wxID_ProcessingToolboxWidget );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "OnProcessingToolsMenu" )

}

void Core::Widgets::MainMenu::OnSelectionToolsMenu( wxCommandEvent& event )
{
	if ( GetCurrentPluginTab() == NULL )
	{
		return;
	}

	wxMenuItem* item = FindItem( event.GetId() );
	try
	{
		SelectionToolboxWidget* selectionToolboxWidget;
		GetCurrentPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );
		if ( selectionToolboxWidget )
		{
			selectionToolboxWidget->SetToolByName( item->GetItemLabel().c_str() );
			GetCurrentPluginTab()->ShowWindow( wxID_SelectionToolboxWidget );
			selectionToolboxWidget->Start();
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro( "OnProcessingToolsMenu" )
}


void Core::Widgets::MainMenu::SetActiveMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* multiRenderWindow )
{
	// Add observer of the current state of multi render window to update the menu
	if ( multiRenderWindow != NULL )
	{
		m_MultiRenderWindowStateConnection.disconnect();

		m_MultiRenderWindowStateConnection = 
			multiRenderWindow->GetMetadataHolder( )->AddObserver( 
			this,
			&Core::Widgets::MainMenu::UpdateMenus );

		// Update all the views
		multiRenderWindow->GetMetadataHolder( )->NotifyObservers( );
	}

}

void Core::Widgets::MainMenu::OnMenuItemWorkflowManager( wxCommandEvent& event )
{
	Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->ShowWorkflowManager( true );
}

void Core::Widgets::MainMenu::OnMenuItemPerspectivePlugin( wxCommandEvent& event )
{
	Core::Runtime::Kernel::GetApplicationSettings()->SetPerspective( 
		Core::Runtime::PERSPECTIVE_PLUGIN );
	Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->RestartApplication( );
}

void Core::Widgets::MainMenu::OnMenuItemPerspectiveWorkflow( wxCommandEvent& event )
{
	Core::Runtime::Kernel::GetApplicationSettings()->SetPerspective( 
		Core::Runtime::PERSPECTIVE_WORKFLOW );
	Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->RestartApplication( );
}

wxFileHistory * Core::Widgets::MainMenu::GetFileHistory() const
{
	return m_FileHistory;
}

void Core::Widgets::MainMenu::ShowReaderHeaderDialog( blTagMap::Pointer tagMap )
{
	// Get registered windows list for reader headers
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_READER_HEADER_WINDOW );

	// Get DataEntity and Extension tags
	Core::DataEntityType type = UnknownTypeId;
	blTag::Pointer tag = tagMap->FindTagByName( "DataEntityType" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( type );
	}

	std::string ext;
	tag = tagMap->FindTagByName( "Extension" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( ext );
	}

	// Create list of windows
	wxWizard* wizard = NULL;
	wxWizardPageSimple* firstWizardPage = NULL;
	std::list<BaseWindow*> baseWindowList;

	// Create a dialog for this file and show to the user
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin() ; it != windowsList.end() ; it++ )
	{
		// Get reader factory
		BaseWindowFactory::Pointer factory = baseWindowFactory->FindFactory( *it );
		Core::BaseWindowReaderFactory* readerFactory;
		readerFactory = dynamic_cast<Core::BaseWindowReaderFactory*> ( factory.GetPointer() );
		if ( readerFactory == NULL )
		{
			throw Core::Exceptions::Exception( 
				"MainMenu::ShowReaderHeaderDialog", 
				"BaseWindowReaderFactory is NULL" );
		}

		// Check if the data entity type is valid for this window
		// and create the wizard pages
		if ( readerFactory->CheckType( type, ext ) )
		{
			if ( wizard == NULL )
			{
				wxString title = "File Header Options";
				wizard = new wxWizard( 
					Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( ), wxID_ANY, title );
			}

			wxWizardPageSimple* wizardPage = new wxWizardPageSimple( wizard );
			wizard->GetPageAreaSizer()->Add( wizardPage );
			if ( firstWizardPage == NULL )
			{
				firstWizardPage = wizardPage;
			}

			factory->SetParent( wizardPage );
			Core::BaseWindow* baseWindow = factory->CreateWindow();

			wxWindow* window = dynamic_cast<wxWindow*> ( baseWindow );
			wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
			sizer->Add( window, 1, wxEXPAND | wxALL, 5 );
			wizardPage->SetSizer( sizer );

			baseWindowList.push_back( baseWindow );
		}
	}

	// Add windows to wizard
	if ( wizard )
	{
		// Add pages and set properties
		std::list<BaseWindow*>::iterator it;
		for ( it = baseWindowList.begin() ; it != baseWindowList.end() ; it++ )
		{
			(*it)->SetProperties( tagMap );
		}

		// Run wizard
		bool result = wizard->RunWizard( firstWizardPage );

		// Get properties
		if ( result )
		{
			for ( it = baseWindowList.begin() ; it != baseWindowList.end() ; it++ )
			{
				(*it)->GetProperties( tagMap );
			}
		}

		wizard->Destroy();

		if ( !result )
		{
			throw Core::Exceptions::Exception( 
				"MainMenu::ShowReaderHeaderDialog", 
				"File import canceled" );
		}
	}

}

void Core::Widgets::MainMenu::OnMenuShowAllToolbars( wxCommandEvent& event )
{
	ShowAllWindows( "Toolbars", true );
}

void Core::Widgets::MainMenu::OnMenuHideAllToolbars( wxCommandEvent& event )
{
	ShowAllWindows( "Toolbars", false );
}

void Core::Widgets::MainMenu::ShowAllWindows( const std::string &category, bool show )
{
	wxWindowUpdateLocker lock( GetCurrentPluginTab( ) );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	PluginTab::WindowMapType windowsMap;
	windowsMap = GetCurrentPluginTab( )->GetAllWindows();

	// Add toolbar menu
	PluginTab::WindowMapType::iterator it;
	for ( it = windowsMap.begin() ; it != windowsMap.end() ; it++ )
	{
		Core::BaseWindow* baseWindow = dynamic_cast<Core::BaseWindow*> ( it->second );
		std::string factoryName = baseWindow->GetFactoryName();
		Core::WindowConfig config;
		if ( !baseWindowFactory->GetWindowConfig( factoryName, config ) )
		{
			continue;
		}

		if ( config.GetCategory() == category )
		{
			GetCurrentPluginTab()->ShowWindow( config.GetCaption(), show );
		}
	}
}

void Core::Widgets::MainMenu::OnMenuUserRegistration( wxCommandEvent& event )
{
	UserRegistrationDialog dialog( this, wxID_ANY, "User Registration" );
	dialog.Center();
	dialog.ShowModal();
}

void Core::Widgets::MainMenu::OnMenuReportBug( wxCommandEvent& event )
{
	wxLaunchDefaultBrowser( "http://sourceforge.net/apps/mantisbt/gimias/bug_report_page.php" );
}

void Core::Widgets::MainMenu::OnMenuDevelopersSite( wxCommandEvent& event )
{
	wxLaunchDefaultBrowser( "http://www.gimias.org/index.php?option=com_content&view=article&id=11&Itemid=20" );
}

void Core::Widgets::MainMenu::OnMenuFrameworkVideos( wxCommandEvent& event )
{
	wxLaunchDefaultBrowser( "http://www.gimias.org/index.php?option=com_content&view=article&id=105&Itemid=61" );
}

void Core::Widgets::MainMenu::OnMenuPreferences( wxCommandEvent& event )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	graphicalIface->GetMainWindow()->ShowPreferencesdialog();
}

void MainMenu::OnMenuSaveSession(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());

		// get file path for session
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetDataSourcePath();
		std::string filetypes = ".xml";
		wxFileDialog* saveFileDialog = new wxFileDialog(this, wxT("Save data to file"), 
			wxT(""), wxT(""), wxT(""), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if(saveFileDialog == NULL)
			return;
		saveFileDialog->SetDirectory(_U(settings->GetCurrentDataPath()));
		saveFileDialog->SetWildcard(_U(filetypes));
		saveFileDialog->SetFilename( "Session" );
		if(saveFileDialog->ShowModal() == wxID_OK)
		{
			std::string sessionFilepath = _U(saveFileDialog->GetPath());

			//write session
			wxBusyInfo info(wxT("Saving session, please wait..."), this);
			Core::IO::SessionWriter::Pointer sessionWriter = Core::IO::SessionWriter::New();
			sessionWriter->SetFilepath(sessionFilepath);
			sessionWriter->SetTreeManager( GetCurrentPluginTab()->GetRenderingTreeManager() );
			sessionWriter->Update();
		}
		delete saveFileDialog;
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		MainMenu::OnMenuSaveSession)
}

void MainMenu::OnMenuOpenSession(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());

		// get file path for session
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetDataSourcePath();
		std::string filetypes = "Session files(*.xml)|*.xml|All files(*.*)|*.*";
		wxFileDialog* readFileDialog = new wxFileDialog(this, wxT("Open file"), 
			wxT(""), wxT(""), wxT(""),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if(readFileDialog == NULL)
			return;
		
		readFileDialog->SetDirectory(_U(settings->GetCurrentDataPath()));
		readFileDialog->SetWildcard(_U(filetypes));
		if(readFileDialog->ShowModal() == wxID_OK)
		{
			std::string sessionFilepath = _U(readFileDialog->GetPath());
			
			//write session
			wxBusyInfo info(wxT("Opening session, please wait..."), this);
			Core::IO::SessionReader::Pointer sessionReader = Core::IO::SessionReader::New();
			sessionReader->SetFilepath(sessionFilepath);
			sessionReader->Update();
		}
		delete readFileDialog;
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		MainMenu::OnMenuReadSession)
}
