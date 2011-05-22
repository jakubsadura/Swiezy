// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "blwxTreeCtrl.h"

#include "blwxAppTreeCtrl.h"

IMPLEMENT_APP_CONSOLE(blwxAppTreeCtrl)


BEGIN_EVENT_TABLE(blwxAppTreeCtrl, wxApp)
END_EVENT_TABLE()

/**
 */
bool blwxAppTreeCtrl::OnInit( )
{
	wxApp::OnInit( );

	CreateMainWindow( );

	ShowMainWindow( );

	return true;
}

/**
 */
void blwxAppTreeCtrl::CreateMainWindow( )
{
	m_mainWindow = new wxFrame(NULL, wxID_ANY, wxT("blwxAppTreeCtrl test"));
}

/**
 */
void blwxAppTreeCtrl::ShowMainWindow( )
{
	// Add all windows into the main window and show it
	m_mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer( wxVERTICAL );

	wxTreeCtrl* tree;
	tree = new blwxTreeCtrl(
		m_mainWindow, 
		wxID_ANY, 
		wxDefaultPosition, 
		wxDefaultSize, 
		wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER|wxTR_EDIT_LABELS);
	layout->Add( tree, 1, wxEXPAND );


	wxTreeItemId root = tree->AddRoot( "Root" );
	wxTreeItemId child = tree->AppendItem( root, "Child1" );
	tree->SetState( child, 1 );
	tree->AppendItem( child, "Child1.1" );
	tree->AppendItem( root, "Child2" );
	tree->AppendItem( root, "Child3" );
	tree->AppendItem( root, "Child4" );
	tree->Expand(root);


	m_mainWindow->SetSizer(layout);
	m_mainWindow->SetSize( 300, 200 );
	m_mainWindow->Show(true);
}

