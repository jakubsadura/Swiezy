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

#include "coreSplashScreen.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreAssert.h"
#include "coreSettings.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"

using namespace Core::Widgets;


/** base Constructor for the class SplashScreen 
\param bitmap requires a bitmap for display as background. Background image should be 600 x 385 (default size of splash window)
\param type defines the behaviour of the splashscreen as follows:
<ul>
<li> Splash: Behaves as a splash screen for the startup time of the application
<li> About: Behaves as a static window for showing up the "About Dialog" of the application
</ul>
*/
SplashScreen::SplashScreen(const wxBitmap& bitmap, Behaviour type, wxWindow* parent) 
: wxSplashScreen(bitmap, wxSPLASH_NO_TIMEOUT | wxSPLASH_CENTRE_ON_SCREEN, 0, parent, wxID_ANY, 
				wxDefaultPosition, wxSize(600, 385))
{ 
	try
	{
		// Set the SplashScreen type
		this->m_Behaviour = type;
		if(type == Behaviour_SPLASH)
			this->SetTimeout(3000);

		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());
		
		// Initialize the Message label
		std::string copyright = std::string( " \xA9 " );
		std::string versionStr = 
			"Build " + settings->GetKernelVersion() + "\n" + 
			settings->GetCompanyName() + copyright + settings->GetYear() + "\n";

		// Initialize the Message label
		this->m_LblMessage = new wxTextCtrl(
			this, wxID_ANY, wxT(""), 
			wxPoint(190, 130), wxSize(400, 240), wxVSCROLL | wxTE_MULTILINE | wxTE_RICH);
		style->ConfigureSplashScreenLog(this->m_LblMessage);
		this->AppendToStartupMessages(versionStr);

		if(this->m_Behaviour == Behaviour_SPLASH) 
			this->AppendToStartupMessages("Starting up");
	}
	coreCatchExceptionsAddTraceAndThrowMacro(SplashScreen::SplashScreen);
}

/**
\todo [BUGFIX] When closing, it must set to NULL the pointer that GraphicalInterface holds, so it can control
when to release the memmory of the SplashScreen at will. The eventhandler OnCloseWindow(wxCloseEvent& event)
Is not working properly, so we have to do it at the Destructor.
*/
SplashScreen::~SplashScreen()
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	if(graphicalIface.IsNotNull())
		graphicalIface->SplashScreenWasUnloaded();
}



/** Appends a string to the message list displayed in the Splash Screen
Its better to use this method instead of message()
*/
void SplashScreen::AppendToStartupMessages(const std::string& message)
{
	this->m_LblMessage->AppendText( wxString::FromUTF8( message.c_str() ) );
	this->m_LblMessage->AppendText(wxT("\n"));

	// If is an about screen, do not scroll bottom. Is is a splash screen, scroll bottom it every time
	// a message is written
	if(this->m_Behaviour == Behaviour_SPLASH) 
	{
		m_LblMessage->ShowPosition( m_LblMessage->GetLastPosition() - m_LblMessage->GetNumberOfLines() );
		m_LblMessage->ScrollLines(-1); 
	}

	this->Update();
}

//!
void SplashScreen::OnCloseWindow(wxCloseEvent& event)
{
	while(this->timeout > clock())
	{}
	event.Skip(true);
}

//!
void SplashScreen::SetTimeout(long milis)
{
	this->timeout = clock() + milis;
}

/**
 */
Core::Widgets::SplashScreen::Behaviour Core::Widgets::SplashScreen::GetBehaviour() const
{
	return m_Behaviour;
}
