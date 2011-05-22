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

#include "coreKernel.h"
#include "coreAssert.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreSettings.h"
#include "coreException.h"
#include "coreWorkflowManager.h"

using namespace Core::Runtime;

class WXDLLEXPORT wxInfoFrame : public wxFrame
{
public:
	wxInfoFrame(wxWindow *parent, const wxString& message);

private:
	DECLARE_NO_COPY_CLASS(wxInfoFrame)
};


//!
wxMitkGraphicalInterface::wxMitkGraphicalInterface(void) : m_MainWindow(NULL), m_SplashScreen(NULL), isRunning(false)
{
}

void wxMitkGraphicalInterface::Start( )
{
	
	try
	{
		m_BaseWindowFactory = Core::BaseWindowFactories::New( );

		// Create the Style Manager
		CreateStyleManager();

		// Create the Main Window (it will configure itself)
		CreateMainWindow( );

		// Create splash screen
		CreateSpashScreen();

		// Create plugin manager
		CreatePluginManager();

		Core::Runtime::Environment::Pointer runtime = Core::Runtime::Kernel::GetApplicationRuntime();
		if ( !runtime->OnAppExit( ) )
		{

			// Show Main Window
			ShowMainWindow();

			OnGraphicalInterfaceInitialized( );

			// Hide splash screen
			HideSplashScreen( );
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(wxMitkGraphicalInterface::Start);
}

//!
wxMitkGraphicalInterface::~wxMitkGraphicalInterface()
{
	this->m_FrontEndPluginManager = NULL;
	this->m_StyleManager = NULL;
}



/** Shows the graphical interface to the user and forces its repaint */
void wxMitkGraphicalInterface::Show(bool show)
{
	if(this->m_MainWindow != NULL) 
	{
		this->m_MainWindow->Show(show);

		// Unload the splash screen
		if(this->m_SplashScreen != NULL)
		{
			this->m_SplashScreen->Close(true);
			this->m_SplashScreen = NULL;
		}
	}
}

/** 
Displays \a message in the splash window list of messages, if it is present.
*/
void wxMitkGraphicalInterface::LogMessage(const std::string& message)
{
	try
	{
		if(this->m_SplashScreen != NULL 
			&& this->m_SplashScreen->GetBehaviour() == Core::Widgets::SplashScreen::Behaviour_SPLASH)
		{
			this->m_SplashScreen->AppendToStartupMessages(message);
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(wxMitkGraphicalInterface::LogMessage);
}

/** 
Prompts the Exception to the Logger (superclass does it), and also displays it in the splash window
list of messages of the Splash Screen, if it is present.
*/
void wxMitkGraphicalInterface::LogException(Core::Exceptions::Exception& e)
{
	try
	{
		this->LogMessage(e.what());
		this->ReportMessage(e.what());
	}
	coreCatchExceptionsAddTraceAndThrowMacro(wxMitkGraphicalInterface::LogException);
}

/** Returns true if the application has initialized the graphical interface,
* so it is not running in console mode only
*/
bool wxMitkGraphicalInterface::IsRunning(void)
{
	return this->isRunning;
}


/** Displays the About Screen of the application */
void wxMitkGraphicalInterface::ShowAboutScreen(void)
{
	try
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		coreAssertMacro(settings.IsNotNull() && "The Settings manager must be running");

		// Create the Splash screen
		std::string filename(settings->GetCoreResourceForFile("Splash.jpg"));
		wxBitmap image( wxString::FromUTF8( filename.c_str() ), wxBITMAP_TYPE_JPEG);

		Core::Widgets::SplashScreen* splashScreen = new Core::Widgets::SplashScreen(image, Core::Widgets::SplashScreen::Behaviour_ABOUT);
		splashScreen->Show();
	}
	coreCatchExceptionsCastToNewTypeAndNoThrowMacro(Core::Exceptions::SplashWindowBitmapIsNotSetException, wxMitkGraphicalInterface::wxMitkGraphicalInterface);

}

//!
void wxMitkGraphicalInterface::ShowLogFileBrowser(void)
{
	this->GetMainWindow()->ShowLogFileBrowser();
}

//!
void wxMitkGraphicalInterface::ShowProfileManager(void)
{
	this->GetMainWindow()->ShowProfileManager();
}


/** 
Sends a warning message to be displayed to the user at the information panel, and optionally as a dialog. 
Also formats accordingly the widget that originates the warning.
This means that the appearance - e.g. the colour - of the the widget is changed, to give visual feedback
to the application user.

\param showAlsoDialog set to true by default. It tells the GUI to display a dialog also.
\param widget The widget that originates the warning. If is set to NULL, the function behaves essentially like ReportMessage function.
\param message The text to report to the user
\sa ReportMessage, ReportError, m_StyleManager
*/
void wxMitkGraphicalInterface::ReportWarning(const std::string& message, bool showAlsoDialog, wxWindow* widget)
{
	if(this->m_StyleManager.IsNotNull() && widget != NULL)
		this->m_StyleManager->SetWarningFormat(widget);
	if(this->m_MainWindow != NULL) 
		this->m_MainWindow->ReportWarning(message, showAlsoDialog);
}

/** 
Sends a error message to be displayed to the user at the information panel, and optionally as a dialog.
that originates the error.
This means that the appearance - e.g. the colour - of the the widget is changed, to give visual feedback
to the application user.

\param showAlsoDialog set to true by default. It tells the GUI to display a dialog also.
\param widget The widget that originates the error. If is set to NULL, the function behaves essentially like ReportMessage function.
\param message The text to report to the user
\sa ReportMessage, ReportWarning, m_StyleManager
*/
void wxMitkGraphicalInterface::ReportError(const std::string& message, bool showAlsoDialog, wxWindow* widget)
{
	if(this->m_StyleManager.IsNotNull() && widget != NULL)
		this->m_StyleManager->SetErrorFormat(widget);
	
	if(this->m_MainWindow != NULL) 
		this->m_MainWindow->ReportError(message, showAlsoDialog);
}

/** 
Displays a message to be displayed to the user at the information panel, and optionally as a dialog.
\param showAlsoDialog set to true by default. It tells the GUI to display a dialog also.
\param message The text to report to the user
\sa ReportError, ReportWarning, m_StyleManager
*/
void wxMitkGraphicalInterface::ReportMessage(const std::string& message, bool showAlsoDialog)
{
	if(this->m_MainWindow != NULL) 
		this->m_MainWindow->ReportMessage(message, showAlsoDialog);
}

/** 
Restores the format of a given widget, undoing the look&feel alterations performed by 
ReportError and ReportWarning.
\sa ReportError, ReportWarning, m_StyleManager
*/
void wxMitkGraphicalInterface::ClearErrorFormat(wxWindow* widget)
{
	if(this->m_StyleManager.IsNotNull() && widget != NULL)
		this->m_StyleManager->SetCurrentFormat(widget);
}

/**
The following two lines allow to set the pointer to NULL when the Splash screen is unloaded,
so the Graphical interface can release the SplashScreen at will. This function should not be used
by programmers, because it exists only for handling an special behaviour of closing wxSplashScreen.
(And we dont want to make the graphical interface listen to wxWidget events)
*/
void wxMitkGraphicalInterface::SplashScreenWasUnloaded(void)
{
	this->m_SplashScreen = NULL;
}

/**
 */
Core::Widgets::BaseMainWindow* Core::Runtime::wxMitkGraphicalInterface::GetMainWindow() const
{
	return m_MainWindow;
}

/**
 */
Core::Runtime::StyleManager::Pointer Core::Runtime::wxMitkGraphicalInterface::GetStyleManager() const
{
	return m_StyleManager;
}

/**
 */
FrontEndPluginManager::Pointer Core::Runtime::wxMitkGraphicalInterface::GetFrontEndPluginManager() const
{
	return m_FrontEndPluginManager;
}

void Core::Runtime::wxMitkGraphicalInterface::CreateSpashScreen()
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	#if !defined(coreNoLoadSplashScreen) && defined(NDEBUG)
		try
		{
			// Create the Splash screen
			std::string filename(settings->GetCoreResourceForFile("Splash.jpg"));
			wxBitmap image( wxString::FromUTF8( filename.c_str() ), wxBITMAP_TYPE_JPEG);

			this->m_SplashScreen = new Core::Widgets::SplashScreen(
				image, 
				Core::Widgets::SplashScreen::Behaviour_SPLASH,
				m_MainWindow );
			this->m_SplashScreen->Show();

		}
		coreCatchExceptionsCastToNewTypeAndNoThrowMacro(Core::Exceptions::SplashWindowBitmapIsNotSetException, wxMitkGraphicalInterface::wxMitkGraphicalInterface);
	#endif
}

void Core::Runtime::wxMitkGraphicalInterface::CreatePluginManager()
{
	std::string message;
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	this->m_FrontEndPluginManager = FrontEndPluginManager::New();

	if ( settings->GetShowRegistrationForm() )
	{
		// Kill the splash screen
		if(this->m_SplashScreen != NULL)
			this->m_SplashScreen->Close(true);
		this->m_SplashScreen = NULL;

		GetMainWindow()->ShowRegistrationForm( );
	}

	// If is the first time start, do not build widgets and run the profile wizard instead
	if(settings->IsFirstTimeStart())
	{
		message = "Starting for first time...\nRunning the Import configuration";
		this->LogMessage(message);
		
		// Kill the splash screen
		if(this->m_SplashScreen != NULL)
			this->m_SplashScreen->Close(true);
		this->m_SplashScreen = NULL;

		// Display the Profile Wizard for profile configuration
		GetMainWindow()->ShowImportConfigurationWizard();
	}

	if ( settings->IsFirstTimeStart() )
	{
		message = "Running the Profile Manager";
		this->LogMessage(message);

		this->ShowProfileManager();
	}
	else if ( settings->GetPerspective() == PERSPECTIVE_PLUGIN )
	{
		// If it is not the first time start, load the plugins into the GUI
		this->m_FrontEndPluginManager->CreatePluginsMatchingProfile(
			Core::Runtime::Kernel::GetApplicationSettings()->GetCurrentUserProfile());

		// Configure plugin tab layout after adding all windows
		// by all plugins
		GetMainWindow()->UpdateLayout( );
	}
	else if ( settings->GetPerspective() == PERSPECTIVE_WORKFLOW )
	{
		// Set active workflow
		//Core::Runtime::Kernel::GetWorkflowManager()->SetActiveWorkflow( 
		//	settings->GetActiveWorkflow() );
		Core::Workflow::Pointer workflow;
		workflow = Core::Runtime::Kernel::GetWorkflowManager()->GetActiveWorkflow( );
		if ( workflow.IsNotNull() )
		{
			m_FrontEndPluginManager->LoadPlugins( workflow->GetPluginNamesList() );
		}
		GetMainWindow()->AttachWorkflow( workflow );

	}

}

void Core::Runtime::wxMitkGraphicalInterface::CreateMainWindow(  )
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	std::string title = settings->GetApplicationTitle();
	m_MainWindow = dynamic_cast<Core::Widgets::BaseMainWindow*> ( wxTheApp->GetTopWindow() );
	m_MainWindow->SetTitle(  wxString::FromUTF8( title.c_str() ) );
	m_MainWindow->Initialize();
}

void Core::Runtime::wxMitkGraphicalInterface::ShowMainWindow()
{
	this->m_MainWindow->Maximize();
	this->m_MainWindow->Show();
}

void Core::Runtime::wxMitkGraphicalInterface::HideSplashScreen()
{
	if ( this->m_SplashScreen )
	{
		this->m_SplashScreen->Hide( );
	}
}

void Core::Runtime::wxMitkGraphicalInterface::OnGraphicalInterfaceInitialized()
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();

	// Save settings
	settings->SaveSettings();

	// Mark it as already initialized and running
	this->isRunning = true;
	std::string message;
	message = "Graphical interface was initialized";
	this->LogMessage(message);

	m_MainWindow->OnGraphicalInterfaceInitialized( );
}

void Core::Runtime::wxMitkGraphicalInterface::CreateStyleManager()
{
	this->m_StyleManager = Core::Runtime::StyleManager::New();
	coreAssertMacro(this->m_StyleManager.IsNotNull() && "The Style manager must be running");
}

void Core::Runtime::wxMitkGraphicalInterface::CreatePluginTab( 
	const std::string &caption )
{
	Core::Runtime::PERSPECTIVE_TYPE perspective;
	perspective = Core::Runtime::Kernel::GetApplicationSettings()->GetPerspective( );

	if ( perspective == PERSPECTIVE_PLUGIN )
	{
		GetMainWindow( )->CreatePluginTab( caption );
	}
}

Core::BaseWindowFactories::Pointer 
Core::Runtime::wxMitkGraphicalInterface::GetBaseWindowFactory() const
{
	return m_BaseWindowFactory;
}

void Core::Runtime::wxMitkGraphicalInterface::RegisterFactory( 
	BaseWindowFactory::Pointer factory, WindowConfig config /*= WindowConfig( ) */ )
{
	if ( GetFrontEndPluginManager().IsNotNull() )
	{
		std::string pluginName = GetFrontEndPluginManager()->GetCurrentPluginName();
		config.PluginName( pluginName );
	}

	GetBaseWindowFactory( )->RegisterFactory( factory, config );

	GetMainWindow( )->RegisterWindow( factory->GetName(), config );
}

Core::BaseWorkingAreaStorage::Pointer Core::Runtime::wxMitkGraphicalInterface::GetWorkingAreaStorage() const
{
	return m_BaseWorkingAreaStorage;
}

void Core::Runtime::wxMitkGraphicalInterface::SetWorkingAreaStorage( BaseWorkingAreaStorage* val )
{
	m_BaseWorkingAreaStorage = val;
}
