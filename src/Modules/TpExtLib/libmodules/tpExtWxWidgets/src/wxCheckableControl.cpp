/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/tooltip.h"

#include "wxCheckableControl.h"

BEGIN_EVENT_TABLE(wxCheckableControl, wxPanel)
  EVT_CHECKBOX			(wxID_ANY, wxCheckableControl::OnCheckBox)
END_EVENT_TABLE()


wxCheckableControl::wxCheckableControl(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
          : wxPanel(parent, id, pos, size, style)
{
	m_Sizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer( m_Sizer );

	m_CheckBox = new wxCheckBox(this, wxID_ANY, wxT(""));
	GetSizer()->Insert( 0, m_CheckBox, wxSizerFlags( ).Proportion( 1 ).Align( wxALIGN_CENTER_VERTICAL ) );
}

wxCheckableControl::~wxCheckableControl()
{

}

void wxCheckableControl::SetCheckBox( wxCheckBox* val, wxSizerFlags& flags )
{
	GetSizer()->Detach( 0 );

	if ( val != m_CheckBox )
	{
		m_CheckBox->Destroy( );
		m_CheckBox = val;
	}

	m_CheckBox->Reparent( this );
	GetSizer()->Insert( 0, val, flags );

	UpdateWidget();
}

void wxCheckableControl::AddWindow( wxWindow* win, wxSizerFlags& flags )
{
	GetSizer()->Add( win, flags );
	win->Reparent( this );
	wxToolTip* toolTip = win->GetToolTip( );
	if ( toolTip )
	{
		m_CheckBox->SetToolTip( toolTip->GetTip() );
	}
}

void wxCheckableControl::OnCheckBox(wxCommandEvent& event)
{
	UpdateWidget();
	event.Skip();
}

void wxCheckableControl::SetValue( bool val )
{
	m_CheckBox->SetValue( val );
	UpdateWidget( );
}

bool wxCheckableControl::GetValue() const
{
	return m_CheckBox->GetValue( );
}

void wxCheckableControl::UpdateWidget()
{
	wxSizerItemList list = GetSizer()->GetChildren();
	wxSizerItemList::iterator it;
	for ( it = list.begin( ) ; it != list.end( ) ; it++ )
	{
		if ( (*it)->GetWindow( ) == m_CheckBox )
		{
			continue;
		}

		(*it)->GetWindow( )->Enable( m_CheckBox->GetValue() );
	}
}

void wxCheckableControl::SetLabel( const wxString& label )
{
	wxPanel::SetLabel( label );
	m_CheckBox->SetLabel( label );
}

wxCheckBox* wxCheckableControl::GetCheckBox()
{
	return m_CheckBox;
}
