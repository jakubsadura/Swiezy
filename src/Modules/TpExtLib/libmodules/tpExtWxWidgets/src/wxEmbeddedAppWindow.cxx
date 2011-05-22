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

#include "wxEmbeddedAppWindow.h"

#include <wx/wupdlock.h>
#include <wx/process.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#if defined(WIN32)
	#include <tlhelp32.h>
	HWND FindWindowMSW( const std::string &name )
	{
		return FindWindow( NULL, name.c_str() ); 
	}
#endif //WIN32


wxEmbeddedAppWindow::wxEmbeddedAppWindow( 
	wxWindow* parent, int id, const wxPoint& pos/*=wxDefaultPosition*/, 
	const wxSize& size/*=wxDefaultSize*/, long style/*=0*/ ) 
	: wxPanel( parent, id, pos, size, style)
{
	m_EmbededWindow = NULL;
	m_ProcessID = -1;
}

wxEmbeddedAppWindow::~wxEmbeddedAppWindow()
{
	KillProcess( );
}

std::string wxEmbeddedAppWindow::GetAppFilePath() const
{
	return m_AppFilePath;
}

void wxEmbeddedAppWindow::SetAppFilePath( std::string val )
{
	m_AppFilePath = val;
}

std::string wxEmbeddedAppWindow::GetWindowName() const
{
	return m_WindowName;
}

void wxEmbeddedAppWindow::SetWindowName( std::string val )
{
	m_WindowName = val;
}

void wxEmbeddedAppWindow::LaunchApp()
{
	if ( m_EmbededWindow )
	{
		return;
	}

	if ( FindProcessID( ) )
	{
		std::string filename = GetFilenameName( m_AppFilePath.c_str() );
		wxString message = wxString::Format( "Application %s is running. ", filename.c_str() );
		message += "Do you want to close it?";
		if ( ::wxMessageBox( message, "Close application", wxYES_NO ) == wxYES )
		{
			KillProcess( );
		}
	}


	// Execute command
	m_LaunchAppThread = new LaunchAppThread( m_AppFilePath );
	if ( m_LaunchAppThread->Create() != wxTHREAD_NO_ERROR ) wxLogError(wxT("Can't create thread!"));
	m_LaunchAppThread->Run();

	// Restore focus
	GetParent( )->SetFocus();

	m_EmbedWindowAppThread = new EmbedWindowAppThread( m_WindowName, this, m_EmbededWindow );
	if ( m_EmbedWindowAppThread->Create() != wxTHREAD_NO_ERROR ) wxLogError(wxT("Can't create thread!"));
	m_EmbedWindowAppThread->Run();
}

void wxEmbeddedAppWindow::do_layout()
{
	// Add to sizer
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	sizer_1->Add( m_EmbededWindow, wxSizerFlags().Expand().Proportion(1) );
	SetSizer(sizer_1);
	sizer_1->Fit(this);

	// Send a resize event to update the size of m_EmbededWindow
	wxSizeEvent resEvent(GetParent()->GetBestSize(), GetParent()->GetId());
	resEvent.SetEventObject(GetParent());
	GetParent()->GetEventHandler()->ProcessEvent(resEvent);

	FitInside( );
}

void wxEmbeddedAppWindow::set_properties()
{

}

bool wxEmbeddedAppWindow::FindProcessID( )
{
	std::string filename = GetFilenameName( m_AppFilePath.c_str() );

#if defined(WIN32)

	PROCESSENTRY32 entry;
	entry.dwFlags = sizeof( PROCESSENTRY32 );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if ( Process32First( snapshot, &entry ) == TRUE ) 
	{
		while ( Process32Next( snapshot, &entry ) == TRUE ) 
		{
			if ( stricmp( entry.szExeFile, filename.c_str() ) == 0 ) 
			{
				m_ProcessID = entry.th32ProcessID;
			}
		}
	}

	CloseHandle( snapshot );

#endif //WIN32

	return m_ProcessID != -1;
}

std::string wxEmbeddedAppWindow::GetFilenameName( const std::string& filename )
{
#if defined(WIN32)
	std::string::size_type slash_pos = filename.find_last_of("/\\");
#else
	std::string::size_type slash_pos = filename.find_last_of("/");
#endif
	if(slash_pos != std::string::npos)
	{
		return filename.substr(slash_pos + 1);
	}
	else
	{
		return filename;
	}
}

void wxEmbeddedAppWindow::KillProcess()
{
#if defined(WIN32)
	if ( m_EmbededWindow )
	{
		HWND hWnd = (HWND) m_EmbededWindow->GetHWND();

		// Ask window nicely to close
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	}

	if ( m_ProcessID != -1 )
	{
		HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, m_ProcessID);
		if ( hProcess )
		{
			::TerminateProcess(hProcess, 1);
			while ( WaitForSingleObject(hProcess, 100)!=WAIT_OBJECT_0 ){}
			CloseHandle( hProcess );
		}
	}
#endif //WIN32
}

wxEmbeddedAppWindow::LaunchAppThread::LaunchAppThread( const std::string command )
{
	m_Command = command;
}

void * wxEmbeddedAppWindow::LaunchAppThread::Entry()
{
	std::string command;
#if defined(WIN32)
	if ( m_Command.find( " " ) != std::string::npos )
	{
		command = "\"" + m_Command + "\"";
	}
	else
	{
		command = m_Command;
	}
#else
	command = m_Command;
#endif 
	int res = system( command.c_str() );
	return NULL;
}

wxEmbeddedAppWindow::EmbedWindowAppThread::EmbedWindowAppThread( 
	const std::string &windowName, wxEmbeddedAppWindow* parent, wxWindow* &embededWindow )
	: m_EmbededWindow( embededWindow )
{
	m_WindowName = windowName;
	m_Parent = parent;
}

void * wxEmbeddedAppWindow::EmbedWindowAppThread::Entry()
{
	// Find window handler
	long hWnd = 0;
	while ( hWnd == 0 ) 
	{ 
#if defined(WIN32)
		hWnd = (long) FindWindowMSW( m_WindowName );
		Sleep( 10 );
#endif
	}

	// Wrap wxWindow and reparent to parent
	m_EmbededWindow = new wxWindow();
	m_EmbededWindow->SetId( wxID_ANY );
#if defined(WIN32)
	m_EmbededWindow->AssociateHandle((WXHWND)hWnd);
#endif
	m_EmbededWindow->SetSize( 20, 20 );
	m_EmbededWindow->Reparent( m_Parent );

	// Remove sizer effect
	long wxWindowLong = m_EmbededWindow->GetWindowStyle( );
	wxWindowLong = wxWindowLong | wxBORDER_NONE & ~wxMINIMIZE_BOX & ~wxMAXIMIZE_BOX & ~wxCAPTION & ~wxRESIZE_BORDER;
	m_EmbededWindow->SetWindowStyle( wxWindowLong );

	// Remove min, max, close and caption bar
#if defined(WIN32)
	long windowLong = GetWindowLong(HWND(hWnd), GWL_STYLE);
	windowLong = windowLong & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_CAPTION & ~WS_THICKFRAME;
	SetWindowLong( HWND(hWnd), GWL_STYLE, windowLong );
#endif //WIN32

	m_Parent->do_layout();

	return NULL;
}
