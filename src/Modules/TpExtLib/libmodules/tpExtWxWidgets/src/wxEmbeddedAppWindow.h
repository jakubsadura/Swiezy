/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxEmbeddedAppWindow_H
#define _wxEmbeddedAppWindow_H

#include "TpExtLibWxWidgetsWin32Header.h"

/**
Embed an external application into a child wxWidgets window

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 7 Jan 2011
*/
class TPEXTLIBWXWIDGETS_EXPORT wxEmbeddedAppWindow : public wxPanel 
{
	/**
	Launch a command line app using std::system
	*/
	class LaunchAppThread : public wxThread
	{
	public:
		LaunchAppThread( const std::string command );

		// thread execution starts here
		virtual void *Entry();

	public:
		//!
		std::string m_Command;
	};

	/**
	Find a window with a specified name
	*/
	class EmbedWindowAppThread : public wxThread
	{
	public:
		//! Punt handle here
		EmbedWindowAppThread( 
			const std::string &windowName, 
			wxEmbeddedAppWindow* parent,
			wxWindow* &embededWindow );

		// thread execution starts here
		virtual void *Entry();

	public:
		//!
		std::string m_WindowName;
		//!
		wxWindow* &m_EmbededWindow;
		//!
		wxEmbeddedAppWindow* m_Parent;
	};

public:
	wxEmbeddedAppWindow(wxWindow* parent, int id, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, long style=0);

	~wxEmbeddedAppWindow( );

	//!
	std::string GetAppFilePath() const;
	void SetAppFilePath(std::string val);

	//!
	std::string GetWindowName() const;
	void SetWindowName(std::string val);

	//!
	void LaunchApp( );

private:
	void set_properties();
	void do_layout();

	//!
	bool FindProcessID( );

	//!
	void KillProcess( );

	//!
	std::string GetFilenameName(const std::string& filename);

protected:
	//!
	wxWindow* m_EmbededWindow;
	//! C:\\Program Files (x86)\\Taverna Workbench 2.2.0\\taverna.exe
	std::string m_AppFilePath;
	//! "Taverna Workbench 2.2.0"
	std::string m_WindowName;
	//! Launch application
	LaunchAppThread* m_LaunchAppThread;
	//! Wait for the window to appear and put it as a child of this wxPanel
	EmbedWindowAppThread* m_EmbedWindowAppThread;
	//! App process ID
	long m_ProcessID;
};

#endif // _wxEmbeddedAppWindow_H
