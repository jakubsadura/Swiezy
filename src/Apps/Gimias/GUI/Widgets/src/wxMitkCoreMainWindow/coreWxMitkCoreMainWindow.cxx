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

#include "coreWxMitkCoreMainWindow.h"
#include "coreMainMenu.h"
#include "coreMainContextMenu.h"
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
#include "coreProfileWizard.h"
#include "coreLogFileViewer.h"
#include "coreMissingDataEntityFieldsWizard.h"
#include "coreStatusBar.h"
#include "coreBaseWindowFactories.h"
#include "coreWorkflowGUIBuilder.h"
#include "coreDataEntityInformation.h"
#include "coreWorkflowManager.h"
#include "coreWorkflowEditorWidget.h"
#include "coreWorkflowManagerWidget.h"
#include "coreDynProcessingWidgetFactory.h"
#include "coreImportConfigurationWizard.h"
#include "coreUserRegistrationDialog.h"
#include "corePreferencesDialog.h"

#include "wxID.h"

#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/dnd.h>
#include <wx/utils.h>
#include <wx/clipbrd.h>
#include <wx/progdlg.h>

#include "wxMemoryUsageIndicator.h"
#include "wxUnicode.h"

#include <stdio.h>

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

#define wxID_WorkflowEditor wxID( "wxID_WorkflowEditor" )
#define wxID_WorkflowManager wxID( "wxID_WorkflowManager" )


BEGIN_EVENT_TABLE(wxMitkCoreMainWindow, wxFrame)
	EVT_CLOSE	(wxMitkCoreMainWindow::OnClose)
	EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_TabContainer, wxMitkCoreMainWindow::OnPageChanged)
	EVT_MOUSE_EVENTS		(wxMitkCoreMainWindow::OnWxMouseEvent)
END_EVENT_TABLE();


#define wxID_ProgressDialog (1 + wxID_HIGHEST)

/**
\brief Info frame defined inside wxWIDGETS but is not public

\author Xavi Planes
\date 05 Dec 2008
\ingroup gmWidgets
*/
class WXDLLEXPORT wxInfoFrame : public wxFrame
{
public:
	wxInfoFrame(wxWindow *parent, const wxString& message);

private:
	DECLARE_NO_COPY_CLASS(wxInfoFrame)
};


#if wxUSE_DRAG_AND_DROP
/**
\brief Drop target for Data entity list (i.e. user drags a file from 
explorer unto window and adds the file to entity list)

\author Xavi Planes
\date 05 Dec 2008
\ingroup gmWidgets
*/
class wxDataEntityListDropTarget : public wxFileDropTarget
{
public:
	wxDataEntityListDropTarget(wxMitkCoreMainWindow& mainWindow) : m_mainWindow(mainWindow) {}
	~wxDataEntityListDropTarget(){}
	virtual bool OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                         const wxArrayString& files)
    {

		std::vector<std::string> pathFilenames;
		for ( unsigned i = 0 ; i < files.size() ; i++ )
		{
			pathFilenames.push_back( _U(files[ i ]) );
		}

		m_mainWindow.GetMainMenu( )->LoadDataEntity( pathFilenames );

		return true;
    }
    wxMitkCoreMainWindow& m_mainWindow;
};
#endif


/** 
Constructor for the class wxMitkCoreMainWindow. It implements a Main Form for the application,
and defines all menus, toolbars, color schemas and font schemas, to provide the application a
friendly-user interface.

Also it will react and dispatch the signals to the other widgets it will contain.
\param parent Parent widget that contains the main window. If there is no parent, call it with NULL
\param name Name for the main window widget (for internal purposes)
\param f Some flags for style spec
*/
wxMitkCoreMainWindow::wxMitkCoreMainWindow(
	wxWindow* parent, 
	wxWindowID id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name) : Core::Widgets::BaseMainWindow(parent, id,title, pos, size, style, name)
{
	this->SetAutoLayout(true);
}


void wxMitkCoreMainWindow::Initialize()
{
	//// Main initialization ////
	try
	{

		// Build and attach the menu bar
		CreateMainMenu();

		// Create context menu
		CreateContextMenu( );

		// Populate the containers and widgets by current profile
		CreateWidgets();
		
		// Status bar
		CreateStatusBar( );

		// Create processing dialog
		CreateProcessingDialog();

		// Configure state holder
		AddStateHolderObserver( );


		// Set a caption and icon to the title bar
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		coreAssertMacro(settings.IsNotNull() && "The Settings manager has to be initialized");
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull() && "The Graphical interface has to be initialized");
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull() && "The Style manager has to be initialized");
		style->ConfigureMainWindow(this);

		std::string filename = settings->GetCoreResourceForFile("Icon32.png");
		wxIcon pix(_U(filename), wxBITMAP_TYPE_PNG);
		this->SetIcon(pix);
		style->ConfigurePageContainer(this->m_TabContainer);

#if wxUSE_DRAG_AND_DROP
		// Drag & Drop
		SetDropTarget( new wxDataEntityListDropTarget( *this ) );
#endif

		m_ProgressDialog = NULL;

		WorkingAreaStorage::Pointer workingAreaStorage = WorkingAreaStorage::New();
		workingAreaStorage->ScanDirectory( settings->GetProjectHomePath() );
		graphicalIface->SetWorkingAreaStorage( workingAreaStorage );

	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::wxMitkCoreMainWindow);
}


/** Destructor for the class wxMitkCoreMainWindow */
wxMitkCoreMainWindow::~wxMitkCoreMainWindow()
{
	// Clean memory allocated by plugins
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->CleanRegisteredWidgets();

	// Save plugins tab pages layout
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	Core::Widgets::PluginTab* page;
	for ( size_t i = 0 ; i < m_TabContainer->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PluginTab*> ( m_TabContainer->GetPage( i ) );
		switch ( settings->GetPerspective() )
		{
		case Core::Runtime::PERSPECTIVE_PLUGIN:
			settings->SetLayoutConfiguration(
				page->GetCaption( ).c_str(),
				page->GetLayoutConfiguration() );
			break;
		case Core::Runtime::PERSPECTIVE_WORKFLOW:
			// Nothing
			break;
		}
	}

	// Removes all pushed event handlers (from plugins also)
	// except the handler of this window
	wxEvtHandler* handler = PopEventHandler( );
	wxEvtHandler* handlerPrev = handler;
	while ( handler )
	{
		handlerPrev = handler;
		handler = PopEventHandler( );
	}
	PushEventHandler( handlerPrev );

	if ( m_ContextMenu )
	{
		delete m_ContextMenu;
	}
}


/** 
Creates the widgets for the graphical interface, attaches the connection events
and configures the interface elements in order to hide that widgets out of the
current profile schema.
*/
void wxMitkCoreMainWindow::CreateWidgets(void)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());

		// Create a label to show when the tab page container is empty.
		this->m_EmptyLabel = new wxStaticText(this, wxID_ANY, wxT("\n\n    <No plugin has been loaded into the application. Please choose Edit Profile in the Edit menu.>"));
		style->ConfigureNoPluginsLoadedLabel(this->m_EmptyLabel);

		// Creates the main tab page container.
		this->m_TabContainer = new wxAuiNotebook(
			this, wxID_TabContainer, wxDefaultPosition, wxDefaultSize, 
			wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );

		wxFont font = *wxNORMAL_FONT;
		font.SetPointSize( 16 );
		m_TabContainer->SetFont( font );

		style->ConfigurePageContainer(this->m_TabContainer);
		this->m_TabContainer->Hide();

		wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
		vlayout->Add(this->m_TabContainer, 1, wxEXPAND);

		//vlayout->Add(hlayout, 0, wxEXPAND);
		this->SetSizer(vlayout);

		// Force relayout the window
		wxSizeEvent event(this->GetSize(), this->GetId());
		event.SetEventObject(this);
		this->GetEventHandler()->ProcessEvent(event);

		PluginTabFactory::RegisterDefaultWidgets();

		m_WorkflowEditorWidget = new WorkflowEditorWidget( this , wxID_WorkflowEditor );
		m_WorkflowEditorWidget->Hide();

		m_WorkflowManagerWidget = new WorkflowManagerWidget( this , wxID_WorkflowManager );
		m_WorkflowManagerWidget->Hide();

		// Create preferences dialog
		m_PreferencesDialog = new PreferencesDialog( this, wxID_ANY, "Preferences" );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(wxMitkCoreMainWindow::CreateWidgets)
}

/** 
*/
void wxMitkCoreMainWindow::CreateMainMenu(void)
{
	try
	{
		// Create the menu bar and attach it
		this->SetMenuBar( new MainMenu( ) );

		PushEventHandler( GetMenuBar()->GetEventHandler() );

	}
	coreCatchExceptionsAddTraceAndThrowMacro(wxMitkCoreMainWindow::CreateMainMenu)
}
	


/** 
Add the tab page widget given by a PluginTab. 
This method is called by the GraphicalInterface when FrontEndPluginManager 
tells it to load a plugin object. 
It retrieves the GUI part of the plugin, held by a PluginTab, and inserts 
it as a new Tab page in the
tab page control of the main window
\sa Core::FrontEndPlugin::FrontEndPlugin, Core::Widgets::PluginTab
*/
bool wxMitkCoreMainWindow::AttachPluginGUI(Core::Widgets::PluginTab* page)
{
	bool attached = false;
	// Adds the Tab
	coreAssertMacro(page != NULL && "The tab page holding the GUI of the plugin cannot be NULL");
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(graphicalIface.IsNotNull() && "The Graphical interface has to be initialized");
	Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
	coreAssertMacro(style.IsNotNull() && "The Style manager has to be initialized");
	try
	{
		page->Reparent(this->m_TabContainer);
		attached = this->m_TabContainer->AddPage(page, _U(page->GetCaption().c_str()), false);
		if(!attached)
		{
			// If the plugin could not be loaded, display error
			Core::Exceptions::OnAttachPluginToGUIException eNoAtt("wxMitkCoreMainWindow::AttachPluginGUI");
			throw eNoAtt;
		}

		style->ConfigurePageContainer(page);
		this->m_EmptyLabel->Hide();
		this->m_TabContainer->Show();
		
		// Disable the tab if it's not the active one
		wxWindow* currentPage;
		currentPage = m_TabContainer->GetPage( m_TabContainer->GetSelection( ) );
		if ( currentPage != page )
		{
			page->SetActiveTab( false );
		}

		// Force relayout the window
		wxSizeEvent event(this->GetSize(), this->GetId());
		event.SetEventObject(this);
		this->GetEventHandler()->ProcessEvent(event);
	}
	coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro(wxMitkCoreMainWindow::AttachPluginGUI);
	return attached;
}


/** 
ARemoves the tab page widget given by a PluginTab. 
This method is called by the GraphicalInterface when FrontEndPluginManager tells it to unload a plugin object. 
It retrieves the GUI part of the plugin, held by a PluginTab, and removes it from the tab page control of 
the main window
\sa Core::FrontEndPlugin::FrontEndPlugin, Core::Widgets::PluginTab
*/
void wxMitkCoreMainWindow::DetachPluginGUI(Core::Widgets::PluginTab* page)
{
	// Adds the Tab
	coreAssertMacro(page != NULL && "The tab page holding the GUI of the plugin cannot be NULL");
	try
	{
		unsigned int i = 0;
		bool found = false;
		while(i < this->m_TabContainer->GetPageCount() && !found)
		{
			found = this->m_TabContainer->GetPage(i) == page;
			if(found)
				this->m_TabContainer->RemovePage(i);
		}
	}
	coreCatchExceptionsCastToNewTypeAndThrowMacro(Core::Exceptions::OnDetachPluginFromGUIException, 
		wxMitkCoreMainWindow::DetachPluginGUI);
}


Core::Widgets::PluginTab* wxMitkCoreMainWindow::GetCurrentPluginTab(void)
{
	if ( m_TabContainer->GetPageCount() == 0 )
	{
		return NULL;
	}

	wxWindow* currentPage;
	currentPage = m_TabContainer->GetPage( m_TabContainer->GetSelection( ) );
	return dynamic_cast<Core::Widgets::PluginTab*> (currentPage);
}

Core::Widgets::PluginTab* Core::Widgets::wxMitkCoreMainWindow::GetLastPluginTab( void )
{
	wxWindow* page;
	int count = m_TabContainer->GetPageCount();
	if ( count == 0 )
	{
		return NULL;
	}
	page = m_TabContainer->GetPage( count - 1 );
	Core::Widgets::PluginTab* pluginTab = dynamic_cast<Core::Widgets::PluginTab*> (page);
	return pluginTab;
}

/** 
Implementation of Close window handler to kill all instances of the application
when the close is issued to the main window. It also prompts for a confirmation.
*/
void wxMitkCoreMainWindow::CloseWindow(void)
{
	std::string message;
	DialogResult question = this->AskQuestionToUserYesOrNo(
		"The application will exit now and\n"
                "all unsaved work will be lost.\n"
                "Exit now?",
		"Exit application?"
		);
	
	if ( question == DialogResult_YES)
	{
		message = "Closing Application now...";
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		graphicalIface->LogMessage(message);

        Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
        coreAssertMacro(settings.IsNotNull());
        settings->SetFirstTimeStart(false);
		Core::Runtime::Environment::Pointer runtime = Core::Runtime::Kernel::GetApplicationRuntime();
		runtime->ExitApplication();
	}
}

/** 
Displays the log file browser file
*/
void wxMitkCoreMainWindow::ShowLogFileBrowser(void)
{
	Core::Widgets::LogFileViewer* logView = new Core::Widgets::LogFileViewer(this);
	logView->SetSize(600, 600);
	logView->ShowModal();
	logView->Destroy();
}

/** 
Displays the Profile Manager file
*/
void wxMitkCoreMainWindow::ShowProfileManager(void)
{
	try
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		coreAssertMacro(settings.IsNotNull());

		Core::Widgets::ProfileWizard* wizard = new Core::Widgets::ProfileWizard(this);
		bool success;
		if(settings->IsFirstTimeStart())
		{
			success = wizard->RunWizard(wizard->GetStartPage());
		}
		else
		{
			success = wizard->RunWizard(wizard->GetSecondPage());
		}

		if( success )
		{
			wizard->UpdateData( );
			wizard->Destroy();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ShowProfileManager);
}

void Core::Widgets::wxMitkCoreMainWindow::ShowImportConfigurationWizard( void )
{
	try
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();

		Core::Widgets::ImportConfigurationWizard* wizard;
		wizard = new Core::Widgets::ImportConfigurationWizard(this);
		if ( !wizard->ScanPreviousConfigurations( ) )
		{
			wizard->Destroy();
			return;
		}

		wizard->RunWizard( wizard->GetStartPage() );

		wizard->Destroy();
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ShowImportConfigurationWizard);
}

/** 
Displays the About screen.
*/
void wxMitkCoreMainWindow::ShowAboutScreen(void)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		graphicalIface->ShowAboutScreen();
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::showLogViewer);
}


//!
void wxMitkCoreMainWindow::OnClose(wxCloseEvent& event)
{
	try
	{
		this->CloseWindow();
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::OnClose)
}

/**
Prompts a modal dialog to the user, displaying a question message and 
asking confirmation for YES or NO.
*/
wxMitkCoreMainWindow::DialogResult 
wxMitkCoreMainWindow::AskQuestionToUserYesOrNo(
	const std::string& message, const std::string& caption)
{
	try
	{
		wxMessageDialog* question = new wxMessageDialog(this, _U(message.c_str()),
								_U(caption.c_str()),
			wxYES_NO | wxICON_QUESTION | wxSTAY_ON_TOP);
		switch(question->ShowModal())
		{
		case wxID_YES: { return DialogResult_YES; }
			case wxID_NO: { return DialogResult_NO; }
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::AskQuestionToUserYesOrNo);
	return DialogResult_ERROR;

}

/**
Prompts a modal dialog to the user, displaying a question message and 
asking for OK or CANCEL.
*/
wxMitkCoreMainWindow::DialogResult 
wxMitkCoreMainWindow::AskQuestionToUserOkOrCancel(
	const std::string& message, const std::string& caption)
{
	try
	{
		wxMessageDialog* question = new wxMessageDialog(this, _U(message.c_str()),
								_U(caption.c_str()),
			wxOK | wxCANCEL | wxICON_QUESTION | wxSTAY_ON_TOP);
		switch(question->ShowModal())
		{
			case wxID_OK: { return DialogResult_OK; }
			case wxID_CANCEL: { return DialogResult_CANCEL; }
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		wxMitkCoreMainWindow::AskQuestionToUserYesOrNo);

	return DialogResult_ERROR;

}


/** Reports a warning message to the Information pannel and displays a dialog*/
void wxMitkCoreMainWindow::ReportWarning(
	const std::string& message, 
	bool showAlsoDialog)
{
	try
	{
		//if(this->m_MacroPannel != NULL) 
		//	this->m_MacroPannel->ReportWarning(message);
		
		if(showAlsoDialog)
		{
			wxMessageDialog* warning = new wxMessageDialog(this, _U(message.c_str()),
								       wxT("Warning"),
				wxOK | wxICON_WARNING | wxSTAY_ON_TOP);

			warning->ShowModal();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ReportWarning);
}


/** Reports an error message to the Information pannel and displays a dialog */
void wxMitkCoreMainWindow::ReportError(const std::string& message, bool showAlsoDialog)
{
	try
	{
		//if(this->m_MacroPannel != NULL) 
		//	this->m_MacroPannel->ReportError(message);
		if(showAlsoDialog)
		{
			wxMessageDialog* error = new wxMessageDialog(this, _U(message.c_str()),
								     wxT("Error"),
				wxOK | wxICON_ERROR | wxSTAY_ON_TOP);

			error->ShowModal();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ReportError);
}


/** Reports an information message to the Information pannel and displays a dialog */
void wxMitkCoreMainWindow::ReportMessage(const std::string& message, bool showAlsoDialog)
{
	try
	{
		//if(this->m_MacroPannel != NULL) 
		//	this->m_MacroPannel->ReportMessage(message);

		if(showAlsoDialog)
		{
			wxMessageDialog* dialog = new wxMessageDialog(this, _U(message.c_str()),
								      wxT("Info"),
				wxOK | wxICON_INFORMATION | wxSTAY_ON_TOP);

			dialog->ShowModal();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ReportMessage);
}

/** Writes a raw text message to the Information pannel. This call is 
used for writing script commands (macros) 
to the macro pannel, displayed verbatim */
void wxMitkCoreMainWindow::WriteVerbatimToMacroPanel(const std::string& message)
{
	try
	{
		//if(this->m_MacroPannel != NULL) 
		//	this->m_MacroPannel->WriteVerbatim(message);
	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::ReportMessage);
}

void Core::Widgets::wxMitkCoreMainWindow::OnPageChanged( wxAuiNotebookEvent& event )
{

	// First disable all Plugins tab to avoid having to plugins tab active 
	// at the same moment
	Core::Widgets::PluginTab* page;
	for ( size_t i = 0 ; i < m_TabContainer->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PluginTab*> ( m_TabContainer->GetPage( i ) );
		page->SetActiveTab( false );
	}

	// Second, enable the active one
	for ( size_t i = 0 ; i < m_TabContainer->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PluginTab*> ( m_TabContainer->GetPage( i ) );
		if ( m_TabContainer->GetSelection( ) == i )
		{
			page->SetActiveTab( true );
			if ( GetMainMenu() )
			{
				GetMainMenu()->SetPluginTab( page );
			}
		}
	}

}

void Core::Widgets::wxMitkCoreMainWindow::CreateContextMenu( )
{
	MainContextMenu* contextMenu = new MainContextMenu( );
	m_ContextMenu = contextMenu;
}

void wxMitkCoreMainWindow::OnWxMouseEvent(wxMouseEvent& event)
{
	try
	{
		if( event.GetButton() == wxMOUSE_BTN_RIGHT )
		{
			if( event.RightDown() && !event.ShiftDown() )
			{
				m_bShowContextMenu = true;
			}
			else
			{
				if ( m_bShowContextMenu &&
					 m_TabContainer->GetRect().Contains( event.GetPosition() ) )
				{
					PopupMenu( m_ContextMenu );
				}
			}

		}

		if ( event.Dragging( ) || event.Moving( ) )
		{
			m_bShowContextMenu = false;
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(wxMitkCoreMainWindow::OnWxMouseEvent)
}

void wxMitkCoreMainWindow::CreateProcessingDialog()
{
	m_processingDialog = new wxInfoFrame( this, wxT("Processing... Please wait") );
	m_processingDialog->Show( false );
}

void wxMitkCoreMainWindow::AddStateHolderObserver()
{
	Core::Runtime::AppStateHolderType::Pointer appStateHolder;
	appStateHolder = Core::Runtime::Kernel::GetApplicationRuntime( )->GetAppStateHolder( );
	appStateHolder->AddObserver( 
		this, 
		&wxMitkCoreMainWindow::OnAppStateChanged, 
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
}


void wxMitkCoreMainWindow::ShowProcessingDialog( bool show )
{
	if ( show )
	{
		// Show hour glass icon
		SetCursor( wxCursor( wxCURSOR_WAIT ) );

		m_processingDialog->Show( true );
		// We need to refresh the dialog
		m_processingDialog->Update( );
	}
	else
	{
		// Show hour glass icon
		SetCursor( wxNullCursor );

		m_processingDialog->Show( false );
	}
}

/**
*/
void wxMitkCoreMainWindow::OnAppStateChanged( )
{
	try
	{
		Core::Runtime::AppStateHolderType::Pointer appStateHolder;
		appStateHolder = Core::Runtime::Kernel::GetApplicationRuntime( )->GetAppStateHolder( );

		Core::ProgressTicketHolderType::Pointer progressTicketHolder;
		progressTicketHolder = Core::Runtime::Kernel::GetApplicationRuntime( )->GetProgressTicketHolder( );

		switch( appStateHolder->GetSubject( ) )
		{
		case Core::Runtime::APP_STATE_PROCESSING:
			if ( progressTicketHolder.IsNull( ) )
			{
				ShowProcessingDialog( true );
			}
			else
			{
				progressTicketHolder->AddObserver(
					this,
					&Core::Widgets::wxMitkCoreMainWindow::OnProgressTicketChanged );
				OnProgressTicketChanged( );
			}
			break;
		case Core::Runtime::APP_STATE_IDLE:
			// hide the processing dialog
			ShowProcessingDialog( false );
			// Hide the window m_ProgressDialog
			OnProgressTicketChanged( );
			break;
		default:
			break;
		}
	}
	coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro(
		wxMitkCoreMainWindow::OnAppStateChanged)
}

void Core::Widgets::wxMitkCoreMainWindow::OnProgressTicketChanged()
{
	// Get progress ticket
	Core::ProgressTicketHolderType::Pointer progressTicketHolder;
	progressTicketHolder = Core::Runtime::Kernel::GetApplicationRuntime( )->GetProgressTicketHolder( );

	if ( progressTicketHolder.IsNotNull() )
	{
		Core::ProgressTicket::Pointer progressTicket;
		progressTicket = progressTicketHolder->GetSubject( );

		if ( m_ProgressDialog == NULL )
		{
			m_ProgressDialog = new wxProgressDialog( 
								_U(progressTicket->GetProcessorName( )),
				wxT(""),
				100,
				this,
				wxPD_SMOOTH | wxPD_CAN_ABORT | wxPD_AUTO_HIDE );
			// This is the correct size for cardiac segmentation plugin
			m_ProgressDialog->SetSize( 250, 160 );
		}
		else
		{
			m_ProgressDialog->SetTitle(_U(progressTicket->GetProcessorName( )));
		}

		// Update the progress dialog
		bool bContinueProcessing;
		m_ProgressDialog->Center();
		bContinueProcessing = m_ProgressDialog->Update( 
			int ( progressTicket->GetProgress( ) * 100 ), 
			_U(progressTicket->GetStatus( ) ));
		m_ProgressDialog->Show( true );
		m_ProgressDialog->Update( );
		if ( !bContinueProcessing )
		{
			progressTicket->SetAbortProcessing( true );
			DisconnectProgressTicketObserver( );
			progressTicketHolder->NotifyObservers( );
		}
	}
	else
	{
		DisconnectProgressTicketObserver( );
		if ( m_ProgressDialog != NULL )
		{
			m_ProgressDialog->Resume( );
			m_ProgressDialog->Show( false );
		}
	}

}

void Core::Widgets::wxMitkCoreMainWindow::DisconnectProgressTicketObserver()
{
	// Disconnect the observer
	Core::ProgressTicketHolderType::Pointer progressTicketHolder;
	progressTicketHolder = Core::Runtime::Kernel::GetApplicationRuntime( )->GetProgressTicketHolder( );
	if ( progressTicketHolder.IsNotNull() )
	{
		progressTicketHolder->RemoveObserver(
			this,
			&Core::Widgets::wxMitkCoreMainWindow::OnProgressTicketChanged );
	}

}

wxStatusBar* Core::Widgets::wxMitkCoreMainWindow::OnCreateStatusBar( 
	int number, long style, wxWindowID id, const wxString& name )
{
	return new Core::Widgets::StatusBar( this );
}

MainMenu* Core::Widgets::wxMitkCoreMainWindow::GetMainMenu()
{
	return dynamic_cast<MainMenu*> ( GetMenuBar() );
}

void Core::Widgets::wxMitkCoreMainWindow::CreatePluginTab( 
	const std::string &caption )
{
	Core::Widgets::PluginTab* pluginTab;
	pluginTab = Core::Widgets::PluginTabFactory::Build( caption );
	AttachPluginGUI( pluginTab );
}


void Core::Widgets::wxMitkCoreMainWindow::RegisterWindow( 
	Core::BaseWindow *baseWindow,
	Core::WindowConfig config /*= Core::WindowConfig( )*/ )
{
	if ( !config.GetProcessingTool() )
	{
		GetLastPluginTab()->AddWindow( baseWindow, config );
	}
}

void Core::Widgets::wxMitkCoreMainWindow::RegisterWindow( 
	const std::string &factoryName,
	Core::WindowConfig config /*= Core::WindowConfig( )*/ )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<Core::Widgets::PluginTab*> list;
	list = GetAllPluginTabs( );
	std::list<Core::Widgets::PluginTab*>::iterator it;
	for ( it = list.begin() ; it != list.end( ) ; it++ )
	{
		try
		{
			PluginTabFactory::AddWindow( factoryName, (*it) );
		}
		coreCatchExceptionsReportAndNoThrowMacro(RegisterWindow)

	}
}

std::list<Core::Widgets::PluginTab*> Core::Widgets::wxMitkCoreMainWindow::GetAllPluginTabs()
{
	std::list<Core::Widgets::PluginTab*> list;

	Core::Widgets::PluginTab* page;
	for ( size_t i = 0 ; i < m_TabContainer->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PluginTab*> ( m_TabContainer->GetPage( i ) );
		list.push_back( page );
	}	

	return list;
}

void Core::Widgets::wxMitkCoreMainWindow::AttachWorkflow( Workflow::Pointer workflow )
{
	if ( workflow.IsNull() )
	{
		return;
	}

	try
	{
		wxAuiNotebook* notebook;
		notebook = Core::Widgets::WorkflowGUIBuilder::Build( workflow );
		notebook->SetId( wxID_TabContainer );
		m_EmptyLabel->Hide();
		GetSizer()->Replace( m_TabContainer, notebook );
		GetSizer( )->Layout();

		m_TabContainer->Destroy( );
		m_TabContainer = notebook;

		// Update menus
		wxAuiNotebookEvent event = wxAuiNotebookEvent();
		OnPageChanged( event );
	}
	coreCatchExceptionsReportAndNoThrowMacro( wxMitkCoreMainWindow::AttachWorkflow )
}

void Core::Widgets::wxMitkCoreMainWindow::ShowWorkflowEditor( bool show /*= true */ )
{
	Core::Workflow::Pointer workflow;
	workflow = Core::Runtime::Kernel::GetWorkflowManager()->GetActiveWorkflow( );
	m_WorkflowEditorWidget->SetWorkflow( workflow );

	m_WorkflowEditorWidget->Show( show );
}

void Core::Widgets::wxMitkCoreMainWindow::ShowWorkflowManager( bool show /*= true */ )
{
	m_WorkflowManagerWidget->Show( show );
}

void Core::Widgets::wxMitkCoreMainWindow::RestartApplication()
{
	std::string message;
	message = \
		"The application needs to restart in order.\n" \
		"to apply the new configuration.\n" \
		"All unsaved data will be lost if you proceed.\n" \
		"Make sure to save all your data before\n" \
		"continuing.\n" \
		"Exit now?";
	wxMessageDialog* question = new wxMessageDialog(this, _U(message.c_str()),
		_U("Exit application?"),
		wxYES_NO | wxICON_QUESTION | wxSTAY_ON_TOP);
	if(question->ShowModal() == wxID_YES)
	{
		Core::Runtime::Kernel::GetGraphicalInterface()->LogMessage("Restarting application now...");
		Core::Runtime::Kernel::GetApplicationRuntime()->RestartApplication();
	}

}


void Core::Widgets::wxMitkCoreMainWindow::UpdateLayout()
{

	std::list<Core::Widgets::PluginTab*> list;
	list = GetAllPluginTabs( );
	std::list<Core::Widgets::PluginTab*>::iterator it;
	for ( it = list.begin() ; it != list.end( ) ; it++ )
	{
		// Backup default layout
		(*it)->BackupLayoutConfiguration();

		// Configure plugin tab layout
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string strConfig;
		settings->GetLayoutConfiguration( (*it)->GetCaption( ).c_str(), strConfig );
		(*it)->UpdateLayoutConfiguration( strConfig );
	}
}

void Core::Widgets::wxMitkCoreMainWindow::OnGraphicalInterfaceInitialized()
{

}

//!
void Core::Widgets::wxMitkCoreMainWindow::RegisterModule( ModuleDescription *module )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

	// Special patch to add "Slicer" to slicer command line plugins
	std::string caption = module->GetTitle();
	if ( module->GetAcknowledgements().find( "Insight Software Consortium" )  != std::string::npos )
	{
		caption = module->GetTitle() + " (ITK)";
	}
	else if ( module->GetAcknowledgements().find( "NAMIC" ) != std::string::npos ||
		 module->GetDocumentationURL().find( "slicer" ) != std::string::npos )
	{
		caption = module->GetTitle() + " (3D Slicer)";
	}
	else if ( module->GetDynamicLibrary().find( "vtk" ) == 0 )
	{
		caption = module->GetTitle() + " (VTK)";
	}
	else if ( module->GetDynamicLibrary().find( "mitk" ) == 0 )
	{
		caption = module->GetTitle() + " (MITK)";
	}

	// Search the factory using the title
	Core::BaseWindowFactory::Pointer factory;
	std::list<std::string> windows;
	windows = graphicalIface->GetBaseWindowFactory( )->GetWindowsList( WIDGET_TYPE_MAX, caption );
	if ( !windows.empty() )
	{
		Core::BaseWindowFactory::Pointer factory;
		factory = graphicalIface->GetBaseWindowFactory( )->FindFactory( *windows.begin() );
		factory->SetModule( *module );
	}
	else if ( !module->GetParameterGroups( ).empty() )
	{
		graphicalIface->RegisterFactory( 
			DynProcessingWidgetFactory::NewBase( *module ),
			WindowConfig( ).Category( module->GetCategory() )
			.ProcessingTool().ProcessorObservers().Caption( caption ) );
	}
	else
	{
		Core::Runtime::Kernel::GetGraphicalInterface()->LogMessage("Module " + module->GetTitle() + " not loaded");
	}


}

bool Core::Widgets::wxMitkCoreMainWindow::ShowMissingDataEntityFieldsWizard( 
	Core::DataEntity::Pointer dataEntity )
{
	bool result = false;
	try
	{
		MissingDataEntityFieldsWizard* wizard;
		wizard = new MissingDataEntityFieldsWizard(this, dataEntity);
		wxWizardPage* startPage = wizard->GetStartPage();
		if(startPage != NULL)
		{
			result = wizard->RunWizard(startPage);
		}

		if(result)
		{
			// If it was not canceled, apply the user choices
			dataEntity->GetMetadata( )->SetModality(wizard->GetSelectedModality());
		}
		wizard->Destroy();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		MainMenu::ShowMissingDataEntityFieldsWizard);
	return result;
}

void Core::Widgets::wxMitkCoreMainWindow::UpdateActiveWorkflowLayout()
{
	Core::Widgets::PluginTab* page;
	for ( size_t i = 0 ; i < m_TabContainer->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PluginTab*> ( m_TabContainer->GetPage( i ) );
		Core::Workflow::Pointer workflow;
		workflow = Core::Runtime::Kernel::GetWorkflowManager()->GetActiveWorkflow( );
		if ( workflow.IsNotNull() )
		{
			Core::WorkflowStep::Pointer step;
			step = workflow->GetStep( page->GetCaption() );
			if ( step.IsNotNull() )
			{
				page->GetProperties( step->GetProperties() );
			}
		}
	}
}

void Core::Widgets::wxMitkCoreMainWindow::ShowRegistrationForm( void )
{
	UserRegistrationDialog dialog( this, wxID_ANY, "User Registration" );
	dialog.Center();
	dialog.ShowModal();
}

void Core::Widgets::wxMitkCoreMainWindow::ShowPreferencesdialog( void )
{
	m_PreferencesDialog->Center();
	m_PreferencesDialog->ShowModal( );
}
