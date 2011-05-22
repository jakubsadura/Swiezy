// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "blwxAppEmbeddedWindow.h"
#include "wxEmbeddedAppWindow.h"

IMPLEMENT_APP_CONSOLE(blwxAppEmbeddedWindow)

BEGIN_EVENT_TABLE(blwxAppEmbeddedWindow, wxApp)
END_EVENT_TABLE()

bool blwxAppEmbeddedWindow::OnInit( )
{
	wxApp::OnInit( );

	m_mainWindow = new wxFrame(NULL, wxID_ANY, wxT("blwxAppEmbeddedWindow test"));

	// Add all windows into the main window and show it
	m_mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer( wxVERTICAL );

	m_mainWindow->SetSizer(layout);
	m_mainWindow->SetSize( 300, 200 );
	m_mainWindow->Show(true);

	wxEmbeddedAppWindow *embedWin = new wxEmbeddedAppWindow( m_mainWindow, wxID_ANY );
	layout->Add( embedWin, wxSizerFlags().Expand().Proportion(1) );

	embedWin->SetAppFilePath( "notepad.exe" );
	embedWin->SetWindowName( "Untitled - Notepad" );
	embedWin->LaunchApp( );

	return true;
}
