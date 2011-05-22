// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "dynAppWxAGUI.h"

#include "ModuleDescriptionParser.h"
#include "ModuleFactory.h"
#include "CISTIBToolkit.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

IMPLEMENT_APP_CONSOLE(tpExtAppAGUI)


BEGIN_EVENT_TABLE(tpExtAppAGUI, wxApp)
END_EVENT_TABLE()

/**
 */
bool tpExtAppAGUI::OnInit( )
{
	wxApp::OnInit( );

	CreateMainWindow( );

	ShowMainWindow( );

	ReadXML( );

	BuildGUI( );

	return true;
}

/**
 */
void tpExtAppAGUI::CreateMainWindow( )
{
	m_mainWindow = new wxFrame(NULL, wxID_ANY, wxT("tpExtAppAGUI test"));
}

/**
 */
void tpExtAppAGUI::ShowMainWindow( )
{
	// Add all windows into the main window and show it
	m_mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer( wxVERTICAL );

	m_mainWindow->SetSizer(layout);
	m_mainWindow->SetSize( 300, 200 );
	m_mainWindow->Show(true);
}

int tpExtAppAGUI::ReadXML()
{
	
	std::string searchPath = std::string( CISTIB_TOOLKIT_BUILD_FOLDER ) + "/bin/debug/commandlineplugins/debug/";
	ModuleFactory factory;
	factory.SetSearchPaths( searchPath );
	factory.Scan();

	std::vector<std::string> moduleNames = factory.GetModuleNames();
	if ( moduleNames.size() == 0 )
	{
		std::cerr << argv[0] << ": Cannot find modules in " << searchPath << std::endl;
		perror(argv[0]);
		return EXIT_FAILURE;
	}

	//m_Module = factory.GetModuleDescription( moduleNames[ 0 ] );
	m_Module = factory.GetModuleDescription( "Simple region growing" );

	return EXIT_SUCCESS;
}

void tpExtAppAGUI::BuildGUI()
{
	dynWxAGUIBuilder builder;
	builder.SetModule( &m_Module );
	builder.SetParentWindow( m_mainWindow );
	builder.Update();

	builder.GetPanel()->GetUpdater()->UpdateWidget( );

	m_mainWindow->GetSizer( )->Add( builder.GetPanel(), 0, wxEXPAND );
}
