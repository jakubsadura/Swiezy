/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "DicomMenuEventHandler.h"
#include "DicomWorkingAreaPanelWidget.h"
#include "DicomPluginWidgetCollective.h"

#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMainMenu.h"
#include "coreSettings.h"
#include "corePluginTab.h"

#include "blMitkUnicode.h"

using namespace DicomPlugin;

// Event the widget
BEGIN_EVENT_TABLE(DicomMenuEventHandler, wxEvtHandler)
	EVT_MENU	(wxID_OpenDataMenuItem, DicomMenuEventHandler::OnMenuOpenDicomFile)	
	EVT_MENU	(wxID_OpenDirectoryMenuItem, DicomMenuEventHandler::OnMenuOpenDirectory)	
	EVT_MENU	(wxID_ShowPACSWidget, DicomMenuEventHandler::OnMenuShowPACSWidget)	
	EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, DicomMenuEventHandler::OnMRUFile)
END_EVENT_TABLE()

DicomMenuEventHandler::DicomMenuEventHandler( WorkingAreaPanelWidget* workingArea ) : wxEvtHandler()
{
	m_WorkingArea = workingArea;
}

void DicomMenuEventHandler::OnMRUFile( wxCommandEvent& event )
{

	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	wxMenuBar* menuBar = gIface->GetMainWindow()->GetMenuBar( );
	Core::Widgets::MainMenu* mainMenu = dynamic_cast<Core::Widgets::MainMenu*> ( menuBar );
	wxString f(mainMenu->GetFileHistory()->GetHistoryFile(event.GetId() - wxID_FILE1));

	Open( f.c_str( ) );
}

void DicomPlugin::DicomMenuEventHandler::OnMenuOpenDirectory( wxCommandEvent& event )
{
	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	wxDirDialog* openDirectoryDialog = new wxDirDialog( 
		m_WorkingArea, wxT("Open Dicom directory"), wxT(""));

	try
	{
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetCurrentDataPath();

		openDirectoryDialog->SetPath(_U(dataPath));
		if(openDirectoryDialog->ShowModal() == wxID_OK)
		{
			std::string dirPath(openDirectoryDialog->GetPath().mb_str(wxConvUTF8));
			if(dirPath.compare("") == 0)
			{
				throw Core::Exceptions::Exception( 
					"OnMenuOpenDirectory",
					"Provide a valid directory path" );
			}

			wxTheApp->Yield(true);

			Open(dirPath);

			AddFileToHistory( dirPath );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomMenuEventHandler::OpenDirectory)

	openDirectoryDialog->Destroy();
}

void DicomPlugin::DicomMenuEventHandler::OnMenuOpenDicomFile(wxCommandEvent& event)
{
	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	wxFileDialog* openFileDialog = new wxFileDialog
		(
		m_WorkingArea,
		wxT("Open a DICOMDIR/DICOM file"),
		wxT(""), wxT(""), wxT(""),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
		);

	try
	{	
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());
		std::string filetypes = "DICOMDIR Files |DICOMDIR|DICOM Files (dcm)|*.dcm|All files (*.*)|*.*";
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetLastOpenedPath();

		//process all the pending messages
		wxTheApp->Yield(true);

		openFileDialog->SetDirectory(_U(dataPath));
		openFileDialog->SetWildcard(_U(filetypes));
		if(openFileDialog->ShowModal() == wxID_OK)
		{
			std::string filePath(openFileDialog->GetPath().mb_str(wxConvUTF8));
			if(filePath.compare("") == 0)
			{
				throw Core::Exceptions::Exception( 
					"OnMenuOpenDirectory",
					"Provide a valid file name for opening a data file" );
			}

			wxTheApp->Yield(true);

			Open(filePath);

			AddFileToHistory( filePath );

		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomMenuEventHandler::OpenDcmFile)

	openFileDialog->Destroy();
}

void DicomPlugin::DicomMenuEventHandler::AddFileToHistory( const std::string &filename )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	wxMenuBar* menuBar = gIface->GetMainWindow()->GetMenuBar( );
	Core::Widgets::MainMenu* mainMenu = dynamic_cast<Core::Widgets::MainMenu*> ( menuBar );
	mainMenu->GetFileHistory()->AddFileToHistory( _U( filename ) );

}

void DicomPlugin::DicomMenuEventHandler::Open( const std::string &filename )
{
	wxTheApp->Yield(true);
	wxBusyInfo info(wxT("Loading DICOM data, please wait..."), m_WorkingArea);
	DataSetProcessor::Pointer processor = 
		dynamic_cast<DataSetProcessor*> ( m_WorkingArea->GetProcessor( ).GetPointer() );
	processor->SetPath( filename.c_str() );
	processor->Update();

	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	Core::IO::File::Pointer file = Core::IO::File::New( );
	file->SetFileNameFullPath( filename );
	Core::Runtime::Kernel::GetApplicationSettings()->SetLastOpenedPath(
		file->GetPathToFile() );

}

void DicomPlugin::DicomMenuEventHandler::OnMenuShowPACSWidget( wxCommandEvent& event )
{
	m_WorkingArea->GetPluginTab()->ShowWindow( wxID_ConnectToPacsDialogWidget );
}
