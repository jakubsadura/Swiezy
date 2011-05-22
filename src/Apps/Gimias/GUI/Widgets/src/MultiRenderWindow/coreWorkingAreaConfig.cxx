/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkingAreaConfig.h"
#include "coreWorkingAreaManagerWidget.h"
#include "coreRenderWindowConfig.h"
#include "wxWidgetStackControl.h"

#include "WorkingAreaConfig.xpm"

#define wxID_BTN_SWITCH wxID( "wxID_BTN_SWITCH" ) 

BEGIN_EVENT_TABLE(Core::Widgets::WorkingAreaConfig, wxPanel)
    EVT_BUTTON(wxID_ANY, Core::Widgets::WorkingAreaConfig::OnBtnSwitch)
END_EVENT_TABLE();


Core::Widgets::WorkingAreaConfig::WorkingAreaConfig( 
	wxWindow* parent, int id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/)
	: wxPanel( parent, id, pos, size, style )
{
	SetBitmap( workingareaconfig_xpm );

	m_btnSwitch = new wxButton(this, wxID_BTN_SWITCH, wxT(""));
	m_StackControl = new wxWidgetStackControl(this, wxID_ANY);

	m_WorkingAreaManagerWidget = new WorkingAreaManagerWidget( this, wxID_ANY );
	m_MultiRenderWindowConfig = new RenderWindowConfig( this, wxID_ANY );

	m_StackControl->Add( m_WorkingAreaManagerWidget );
	m_StackControl->Add( m_MultiRenderWindowConfig );
	
	m_StackControl->Raise(m_MultiRenderWindowConfig);
	m_btnSwitch->SetLabel( wxT( "Workgin Area Manager >>>" ) );

	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(m_btnSwitch, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(m_StackControl, 1, wxEXPAND | wxALL, 4);

	SetSizer(vlayout);
	vlayout->Fit(this);
}

void Core::Widgets::WorkingAreaConfig::SetMultiRenderWindow( RenderWindowBase* multiRenderWindow )
{
	m_MultiRenderWindowConfig->SetMultiRenderWindow( multiRenderWindow );
	m_WorkingAreaManagerWidget->SetMultiRenderWindow( multiRenderWindow );
}

Core::BaseProcessor::Pointer Core::Widgets::WorkingAreaConfig::GetProcessor()
{
	return NULL;
}

void Core::Widgets::WorkingAreaConfig::OnBtnSwitch( wxCommandEvent &event )
{
	if ( m_StackControl->GetWidgetOnTop( ) == m_WorkingAreaManagerWidget )
	{
		m_StackControl->Raise(m_MultiRenderWindowConfig);
		m_btnSwitch->SetLabel( wxT( "Working Area Manager >>>" ) );
	}
	else
	{
		m_StackControl->Raise(m_WorkingAreaManagerWidget);
		m_btnSwitch->SetLabel( wxT( "<<< Configure Working Area" ) );
	}
}

void Core::Widgets::WorkingAreaConfig::SetPluginTab( Core::Widgets::PluginTab* val )
{
	BaseWindow::SetPluginTab( val );
	m_MultiRenderWindowConfig->SetPluginTab( GetPluginTab() );
	m_WorkingAreaManagerWidget->SetPluginTab( GetPluginTab() );
}
