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

#include "coreStyleManager.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreSettings.h"
#include "coreDirectory.h"

#include <wx/sysopt.h>

using namespace Core::Runtime;

/** Constructor for the class StyleManager */
StyleManager::StyleManager(void)
{
	try
	{
		// Initialize all image handler types for all available image formats (jpeg, png...)
		wxInitAllImageHandlers();
		
		// Disable the gradient background for the tab page control
		//wxSystemOptions::SetOption(wxT("msw.notebook.themed-background"), 0);

	}
	coreCatchExceptionsReportAndNoThrowMacro(StyleManager::StyleManager);
}

/** Destructor for the class StyleManager */
StyleManager::~StyleManager(void)
{
}

/** Formats a given widget in order to display a warning */
void StyleManager::SetWarningFormat(wxWindow* widget)
{
}

/** Formats a given widget in order to display an error */
void StyleManager::SetErrorFormat(wxWindow* widget)
{
}

/** Formats a given widget in order to display its standard look & feel */
void StyleManager::SetCurrentFormat(wxWindow* widget)
{
}

/** Returns the front color used for displaying normal text */
wxColour StyleManager::GetBasicTextColor(void) const 
{
	return wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
}

/** Returns the front color used for displaying warnings */
wxColour StyleManager::GetWarningTextColor(void) const 
{ 
	return wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
}

/** Returns the front color used for displaying errors */
wxColour StyleManager::GetErrorTextColor(void) const 
{ 
	return wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
}


/** Automatically applies the style to the Splash screen log textbox */
void StyleManager::ConfigureSplashScreenLog(wxTextCtrl* label)
{
}


/** 
Automatically applies the style to the text that is displayed in 
the main window when no plugin was loaded 
*/
void StyleManager::ConfigureNoPluginsLoadedLabel(wxStaticText* label)
{
}


/** Automatically applies the style to the main window */
void StyleManager::ConfigureMainWindow(wxFrame* frame)
{
}

/** Automatically applies the style to the Command Pannel */
void StyleManager::ConfigureCommandPanel(wxWindow* commandPannel)
{
}

/** Automatically applies the style to the Command Pannel */
void StyleManager::ConfigureProgressPanel(wxWindow* progressPannel)
{
}

/** Automatically applies the style to the items inserted in the Command Pannel */
void StyleManager::ConfigureCommandPanelItem(wxWindow* item)
{
}


/** Automatically applies the style to the Working Area pannel */
void StyleManager::ConfigureWorkingArea(wxWindow* workingArea)
{
}

/** Automatically applies the style to the Page Container */
void StyleManager::ConfigurePageContainer(wxWindow* container)
{
}

/** Automatically applies the style to the left button toolbar */
void StyleManager::ConfigureLeftToolbar(wxWindow* container)
{
}

/** Automatically applies the style to the Acquire input controls */
void StyleManager::ConfigureAcquireInputControl(wxWindow* control)
{
}

/** Automatically applies the style to the buttons of the left button toolbar */
void StyleManager::ConfigureButtonForLeftToolbar(wxControl* button)
{
}

void StyleManager::ConfigureConsoleWindow(wxTextCtrl* text)
{
}

void StyleManager::ConfigureProcessingDialog(wxWindow* window)
{
}



