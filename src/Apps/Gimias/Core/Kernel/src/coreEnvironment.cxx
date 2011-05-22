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

#include "coreDirectory.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreEnvironment.h"
#include "coreAssert.h"
#include "coreLogger.h"
#include "coreKernel.h"

using namespace Core::Runtime;

// Variable (global within the library scope) for holding the command-line arguments
wxChar** av;

// Flag (global within the file scope) that states if the application must restart when RestartIfNeeded is called.
bool mustRestartOnExit = false;

#if defined(WIN32) || defined(_WINDOWS_)
	#include <process.h>
#else
	#include <unistd.h>
#endif

/**
Creates a new process running the application again (restarting it) and kills the current process
(exits this instance of the application)
This routine is called at exit, because the "onexit" signal, by the "atexit" call (ANSI).
*/
void RestartIfNeeded(void)
{
	if(mustRestartOnExit && av != NULL)
	{
		// Restart again GIMIAS
		wxExecute( av );

		// Exit this process
		exit( 0 );
	}
}

// ------------------------------------------------------------------------------------------------

//!
Environment::Environment()
{
	this->exit = false;
	m_AppStateHolder = AppStateHolderType::New( );
	m_AppStateHolder->SetName( "State Holder" );
	m_AppStateHolder->SetSubject( APP_STATE_IDLE );
}

//!
Environment::~Environment(void)
{
}

/**
Sets the RestartIfNeeded function as the function to call when the application quits.
\param Since the Core adopted wxWidgets as GUI, argc and mode are deprecated and not used. argv still have to be provided
*/
void Environment::InitApplication(int& argc, wxChar ** argv, Core::Runtime::AppRunMode mode)
{
	atexit(RestartIfNeeded);
	av = argv;
}

/** 
Returns true if the application is running.
*/
bool Environment::IsAppRunning(void) const
{
	return wxTheApp != NULL && wxTheApp->IsMainLoopRunning();
}

/** 
Returns true if the application is exiting.
*/
bool Environment::OnAppExit(void) const
{
	coreAssertMacro(wxTheApp != NULL);
	return this->exit;
}

/** 
Exits the application main loop (see header file).
*/
void Environment::ExitApplication(void)
{
	coreAssertMacro(wxTheApp != NULL);

	// If we're on exit, do not try exiting again
	if(this->OnAppExit())
		return;
	this->exit = true;

	// by default, the program will exit the
	// main loop (and so, usually, terminate) when the last top-level
	// program window is deleted
	wxTheApp->SetExitOnFrameDelete(true);

	// Destroy the main window (wxMitkCoreMainWindow) and the child windows
	// will be destroyed automatically
	wxWindow* mainWindow = wxTheApp->GetTopWindow();
	if(mainWindow != NULL)
	{
		mainWindow->Show( false );
		mainWindow->Destroy();
	}
}

/** 
Restarts the application main loop.
*/
void Environment::RestartApplication(void)
{
	mustRestartOnExit = true;
	ExitApplication();
}

/** 
Start the application main loop.
*/
void Environment::Exec(void)
{
	coreAssertMacro(wxTheApp != NULL);
	try
	{
		// Run the main event loop
		while(!this->OnAppExit() && !wxTheApp->IsMainLoopRunning())
		{
			try
			{
				wxTheApp->MainLoop();
			}
			coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro(Environment::Exec)
		}

		// Dispatch destroy event for main window when only the profile window is shown
		wxTheApp->ProcessIdle( );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Environment::Exec);
}

AppStateHolderType::Pointer Environment::GetAppStateHolder() const
{
	return m_AppStateHolder;
}

void Environment::SetAppState( 
	APP_STATE val, 
	Core::ProgressTicketHolderType::Pointer progressTicket )
{
	m_ProgressTicketHolder = progressTicket;
	m_AppStateHolder->SetSubject( val );
}

Core::ProgressTicketHolderType::Pointer Core::Runtime::Environment::GetProgressTicketHolder() const
{
	return m_ProgressTicketHolder;
}


