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
#include "coreMainApp.h"
#include "coreLogger.h"
#include "coreException.h"
#include "coreSettings.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataEntityBuildersRegistration.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityHelper.h"
#include "coreDataTreeHelper.h"
#include "corePluginTab.h"
#include "coreSessionReader.h"

#include <wx/cmdline.h>
#include <iostream>
#include <stdio.h>
#include <string>
#ifdef _MSC_VER
	#include <new.h>
	
#endif //not MSVC

using namespace Core;

IMPLEMENT_APP_CONSOLE(MainApp)

/**
The startup initialization of the Kernel and other main objects
*/

//! this specify the exception handler of the memory allocation failure when _set_new_mode(1) (only on MSVC)
//seems that the function cannot be included in a namespace.. I don't know why!
int CISTIB_memory_failure(size_t size)
{
	throw std::bad_alloc( );
	return 0;
}

// constants for the command line options names
namespace CmdLineOption
{

	const char * const FILENAME = "filename";
	const char * const SESSION = "session";

} // namespace CmdLineOption

void MainApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	wxApp::OnInitCmdLine(parser);

	parser.AddOption("", CmdLineOption::FILENAME, "Load filename");
	parser.AddOption("", CmdLineOption::SESSION, "Load session");
}


bool MainApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxString filename;
	if ( parser.Found(CmdLineOption::FILENAME, &filename) )
	{
		Core::DataEntity::Pointer dataEntity;
		dataEntity = Core::DataEntityHelper::LoadDataEntity( filename.c_str() );

		Core::DataEntityHolder::Pointer holder = Core::DataEntityHolder::New( );
		holder->SetSubject( dataEntity );
		RenderingTree::Pointer renderingTree;

		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		Core::Widgets::PluginTab* tab;
		tab = graphicalIface->GetMainWindow()->GetCurrentPluginTab();
		if ( tab )
		{
			Core::DataTreeHelper::PublishOutput( 
				holder, 
				tab->GetRenderingTreeManager()->GetActiveTree() );
		}
	}else if ( parser.Found(CmdLineOption::SESSION, &filename) )
	{
		Core::IO::SessionReader::Pointer sessionReader = Core::IO::SessionReader::New();
		sessionReader->SetFilepath(filename.c_str());
		sessionReader->Update();
	}

	return wxApp::OnCmdLineParsed(parser);
}

bool MainApp::OnInit(void)
{
	Core::Runtime::Environment::Pointer runtime;
	Core::Runtime::Logger::Pointer logger;

	try
	{
		//old_mode = _set_new_mode(1);
		//old_handler = _set_new_handler(CISTIB_memory_failure );
		//I don't use these returns, since I am not planning to restore the original values...
#ifdef _MSC_VER
			_set_new_mode(1);
			_set_new_handler(CISTIB_memory_failure );	
#endif //not MSVC

		// Create main window
		wxWindow *mainWindow;
		mainWindow = new Core::Widgets::wxMitkCoreMainWindow(NULL, wxID_ANY, wxT(""));
		this->SetTopWindow( mainWindow );

		Core::Runtime::Kernel::Initialize( this, Core::Runtime::Graphical, "");

		std::cout << Core::Runtime::Kernel::GetApplicationSettings()->GetApplicationTitleAndVersion() << 
			" is starting..." << std::endl;

		wxApp::OnInit();
	}
	catch(Core::Exceptions::Exception& e)
	{
		logger = Core::Runtime::Kernel::GetLogManager();
		std::cerr << e.what() << std::endl;
		std::cerr << "---  Stack Trace --- \n" << e.GetStackTrace() << std::endl;
		if(logger.IsNotNull()) 
			logger->LogException(e);
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
		return false;
	}
	catch(...)
	{ 
		// Catch any other unhandled exception to gain in stability 
		std::cerr << "Unhandled Exception caught" << std::endl;
		std::cerr << "The application exited because an unexpected error while initializing. Contact the support" << std::endl;
		std::cerr << " team in order to solve this problem." << std::endl << std::endl;
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
		return false;
	}
	
	return true;
}


//! this is executed upon startup, like 'main()' in non-wxWidgets programs, and holds the application main loop
int MainApp::OnRun(void)
{
	int errorCode = 0;
	Core::Runtime::Environment::Pointer runtime;
	Core::Runtime::Logger::Pointer logger;
	std::string message;

	try
	{
		logger = Core::Runtime::Kernel::GetLogManager();
		runtime = Core::Runtime::Kernel::GetApplicationRuntime();
		runtime->Exec();
	}
	catch(Core::Exceptions::Exception& e)
	{
		// Report an error code 1 (exit by unhandled exception caught on runtime)
		errorCode = 1;
		std::cerr << e.what() << std::endl;
		std::cerr << "---  Stack Trace --- \n" << e.GetStackTrace() << std::endl;
		if(logger.IsNotNull()) 
			logger->LogException(e);
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
	}
	catch(std::exception& e)
	{ 
		// Catch any other unhandled std::exception to gain in stability 
		// Report an error code 1 (exit by unhandled exception caught on runtime)
		errorCode = 1;
		std::cerr << "Std Unhandled Exception caught: " << std::endl;
		std::cerr << e.what() << std::endl << std::endl;
		std::cerr << "The application exited because an unexpected error while in runtime. Contact the support" << std::endl;
		std::cerr << " team in order to solve this problem." << std::endl << std::endl;
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
	}
	catch(...)
	{ 
		// Catch any other unhandled exception to gain in stability 
		// Report an error code 2 (exit by unknown unhandled exception caught on runtime)
		errorCode = 2;
		std::cerr << "Unhandled Exception caught" << std::endl;
		std::cerr << "The application exited because an unexpected error while in runtime. Contact the support" << std::endl;
		std::cerr << " team in order to solve this problem." << std::endl << std::endl;
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
	}

	return errorCode;
}

/**
On exit application is called when closing the main frame of the application. Here all objects must be
unloaded to prevent memmory leaks.
*/
int MainApp::OnExit(void)
{
	int errorCode = 0;
	try 
	{ 
		// Cleanup allocation of all the members of static objects
		Core::Runtime::Kernel::Terminate(); 

		// Remove DataStorage
		wxApp::OnExit( );
	} 
	catch(std::exception& e)
	{ 
		// Catch any other unhandled std::exception to gain in stability 
		// Report an error code 1 (exit by unhandled exception caught on runtime)
		errorCode = 1;
		std::cerr << "Std Unhandled Exception caught: " << std::endl;
		std::cerr << e.what() << std::endl << std::endl;
		std::cerr << "The application exited because an unexpected error while in runtime. Contact the support" << std::endl;
		std::cerr << " team in order to solve this problem." << std::endl << std::endl;
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
	}
	catch(...)
	{ 
		// Catch any other unhandled exception to gain in stability 
		// Report an error code 2 (exit by unknown unhandled exception caught on runtime)
		errorCode = 2;
		std::cerr << "Unhandled Exception caught" << std::endl;
		std::cerr << "The application exited because an unexpected error while in runtime. Contact the support" << std::endl;
		std::cerr << " team in order to solve this problem." << std::endl << std::endl;
		std::cerr << "Press a key to exit..." << std::endl;
		getchar();
	}

	return errorCode == 0;
}



void MainApp::HandleEvent( 
	wxEvtHandler *handler, wxEventFunction func, wxEvent& event ) const
{
	try
	{
		wxApp::HandleEvent( handler, func, event );
	}
	coreCatchExceptionsReportAndNoThrowMacro( MainApp::HandleEvent );
}
