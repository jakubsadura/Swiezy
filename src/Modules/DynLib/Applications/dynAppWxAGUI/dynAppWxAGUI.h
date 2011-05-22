/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wx/app.h"

#include "dynWxAGUIBuilder.h"

/**
\brief Application to create Automatic GUI
\ingroup tpExtWxWidgets
\author Xavi Planes
\date 7 July 2010
*/
class tpExtAppAGUI : public wxApp
{
public:
	//! Called before OnRun(), this is a good place to do initialization
	bool OnInit( );

private:
	//! Create the layout of all windows and show main window
	void ShowMainWindow( );

	//! Create main window
	void CreateMainWindow( );

	//!
	int ReadXML( );

	//!
	void BuildGUI( );

private:
	
	//! Man window
	wxFrame* m_mainWindow;

	//!
	ModuleDescription m_Module;

	DECLARE_EVENT_TABLE();
};

DECLARE_APP(tpExtAppAGUI)

