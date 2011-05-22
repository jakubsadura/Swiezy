// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "wxMitkAppRenderingTree.h"
#include "wxMitkTestRenderingTreeWidget.h"
#include "CISTIBToolkit.h"

IMPLEMENT_APP_CONSOLE(wxMitkAppRenderingTree)

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkAppRenderingTree, wxMitkApp)
END_EVENT_TABLE()

const int MAX_WINDOWS = 1;

/**
 */
bool wxMitkAppRenderingTree::OnInit( )
{
	mitk::wxMitkApp::OnInit( );

	CreateMainWindow( );

	ShowMainWindow( );

	return true;
}

/**
 */
void wxMitkAppRenderingTree::CreateMainWindow( )
{
	m_mainWindow = new wxFrame(NULL, wxID_ANY, wxT("wxMitk test"));
}

/**
 */
void wxMitkAppRenderingTree::ShowMainWindow( )
{
	// Add all windows into the main window and show it
	m_mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer( wxVERTICAL );
	mitk::wxMitkTestRenderingTreeWidget *widget;

	wxMitkTestRenderingTreeWidgetParams params;
	params.m_inputType = wxMitkTestRenderingTreeWidgetParams::INPUT_TYPE_IMAGE;
	params.m_bEnableMultiRenderWindow = true;
	params.m_bEnableVolumeRendering = false;
	params.m_layoutConfiguration = Default2x2;
	params.m_pointInteractorType = wxMitkTestRenderingTreeWidgetParams::POINT_INTERACTOR_TYPE_NONE;
	params.m_strDataPath = "";//CISTIB_TOOLKIT_FOLDER;

	for ( int i = 0 ; i < MAX_WINDOWS ; i++ )
	{
		widget = new mitk::wxMitkTestRenderingTreeWidget( m_mainWindow, wxID_ANY );
		widget->Initialize( params );
		layout->Add( widget, 1, wxEXPAND );
	}

	m_mainWindow->SetSizer(layout);
	m_mainWindow->SetSize( 1024, 800 );
	m_mainWindow->Show(true);

	// Test showing labels
	//widget->TestTextOutput();
}

