/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "corePreferencesDialog.h"
#include "corePreferencesPage.h"

using namespace Core::Widgets;

Core::Widgets::PreferencesDialog::PreferencesDialog( 
	wxWindow* parent, int id, const wxString& title, 
	const wxPoint& pos/*=wxDefaultPosition*/, const wxSize& size/*=wxDefaultSize*/, 
	long style/*=wxDEFAULT_DIALOG_STYLE*/ )
	: corePreferencesDialogUI(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxTHICK_FRAME)
{
	m_PreferencesWindow = NULL;

	// Add observer to registered windows
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->GetFactoriesHolder()->AddObserver(
		this,
		&PreferencesDialog::UpdateTreebook );
}

void Core::Widgets::PreferencesDialog::UpdateTreebook()
{
	if ( m_PreferencesWindow == NULL )
	{
		return;
	}

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_PREFERENCES );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		std::string factoryName = *it;

		WindowConfig config;
		baseWindowFactory->GetWindowConfig( factoryName, config );
		
		if ( !m_PreferencesWindow->FindWindowByName( config.GetCaption() ) )
		{
			BaseWindow* baseWindow = baseWindowFactory->CreateWindow( factoryName, m_PreferencesWindow );
			m_PreferencesWindow->AddPage( 
				dynamic_cast<wxWindow*> (baseWindow),
				wxT( config.GetCaption() ) );

		}
	}

}

void Core::Widgets::PreferencesDialog::OnOK( wxCommandEvent &event )
{
	Core::Widgets::PreferencesPage* page;
	for ( size_t i = 0 ; i < m_PreferencesWindow->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PreferencesPage*> ( m_PreferencesWindow->GetPage( i ) );
		if ( page )
		{
			page->UpdateData();
		}
	}

	event.Skip();
}

void Core::Widgets::PreferencesDialog::OnCancel( wxCommandEvent &event )
{
	event.Skip();
}

int Core::Widgets::PreferencesDialog::ShowModal()
{
	// Create wxTreebook
	if ( m_PreferencesWindow == NULL )
	{
		m_PreferencesWindow = new wxTreebook( this, wxID( "PreferencesDialogTreebook" ) );

		UpdateTreebook( );

		GetSizer()->Insert( 0, m_PreferencesWindow, 
			wxSizerFlags( ).Proportion( 1 ).Expand().Border(wxALL, 5) );
		FitInside();
	}

	Core::Widgets::PreferencesPage* page;
	for ( size_t i = 0 ; i < m_PreferencesWindow->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PreferencesPage*> ( m_PreferencesWindow->GetPage( i ) );
		if ( page )
		{
			page->UpdateWidget();
		}
	}

	return corePreferencesDialogUI::ShowModal();
}

void Core::Widgets::PreferencesDialog::OnApply( wxCommandEvent &event )
{
	Core::Widgets::PreferencesPage* page;
	for ( size_t i = 0 ; i < m_PreferencesWindow->GetPageCount( ) ; i++ )
	{
		page = dynamic_cast<Core::Widgets::PreferencesPage*> ( m_PreferencesWindow->GetPage( i ) );
		if ( page )
		{
			page->UpdateData();
		}
	}

	event.Skip();
}

PreferencesDialog::~PreferencesDialog(void)
{

}

