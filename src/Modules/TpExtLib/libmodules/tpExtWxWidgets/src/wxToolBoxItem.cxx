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

#include "wxToolBoxItem.h"
#include "wxToolBoxItemSelectedEvent.h"
#include <wx/tglbtn.h>

#define wxID_CaptionButton (10 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxToolBoxItem, wxPanel)
	EVT_TOGGLEBUTTON(wxID_CaptionButton, wxToolBoxItem::OnButtonToggled)
END_EVENT_TABLE()

//!
wxToolBoxItem::wxToolBoxItem(wxWindow* parent, wxWindowID id, wxWindow* widget, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	m_widget = widget;
	this->SetAutoLayout(true);

	// Create caption m_button
	this->m_button = new wxToggleButton(this, wxID_CaptionButton, caption, wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
	this->m_button->SetMaxSize(wxSize(-1, 32));

	// Layout
	this->m_internalLayout = new wxBoxSizer(wxVERTICAL);

	// Add the m_widget
	if(this->m_widget != NULL)
	{
		this->m_widget->SetAutoLayout(true);
		this->m_widget->SetMinSize(wxSize(this->GetMinSize().GetWidth(), -1));
		this->m_widget->SetMaxSize(wxSize(this->GetMinSize().GetWidth(), -1));
		this->m_widget->Reparent(this);
		this->m_widget->Hide();
	}
	this->m_internalLayout->Add(this->m_button, 0, wxEXPAND);
	if(this->m_widget != NULL)
	{
		this->m_internalLayout->Add(this->m_widget, 1, wxEXPAND);
		this->m_widget->Enable( false );
	}
	this->m_internalLayout->SetSizeHints(this);
	this->SetSizer(this->m_internalLayout);
}

//!
wxToolBoxItem::~wxToolBoxItem(void)
{
}

//!
wxWindow* wxToolBoxItem::GetWidget(void) const
{
	return this->m_widget;
}

//!
wxString wxToolBoxItem::GetCaption(void) const
{
	return this->m_button->GetLabel();
}

//!
void wxToolBoxItem::SetWidget(wxWindow* widget)
{
	// Clear layout and reparent widgets
	this->m_internalLayout->Clear(false);
	this->m_widget->Reparent(NULL);
	this->m_widget = widget;
	this->m_widget->SetAutoLayout(true);
	this->m_widget->SetMinSize(wxSize(this->GetMinSize().GetWidth(), -1));
	this->m_widget->SetMaxSize(wxSize(this->GetMinSize().GetWidth(), -1));
	this->m_widget->Reparent(this);

	// Add the m_widget
	this->m_internalLayout->Add(this->m_button, 0, wxEXPAND);
	this->m_internalLayout->Add(this->m_widget, 1, wxEXPAND);
	this->m_internalLayout->SetSizeHints(this);

	this->m_widget->Enable( false );
}

//!
void wxToolBoxItem::SetCaption(wxString& caption)
{
	this->m_button->SetLabel(caption);
}
	
//!
void wxToolBoxItem::Toggle(void)
{
	bool bButtonValue;
	if ( ( this->m_button->GetValue() + 1 ) % 2 == 0 )
	{
		bButtonValue = false;
	}
	else
	{
		bButtonValue = true;
	}

	this->m_button->SetValue( bButtonValue );

	wxCommandEvent event( 
		wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
		this->m_button->GetId( ) );
	event.SetInt( bButtonValue );
	event.SetEventObject(this->m_button);
	this->m_button->GetEventHandler()->ProcessEvent(event);
}

//!
void wxToolBoxItem::Collapse(void)
{
	this->m_button->SetValue(false);
	this->m_widget->Hide();
	this->m_widget->InvalidateBestSize();
	this->InvalidateBestSize();
	this->m_internalLayout->SetSizeHints(this);
}

//!
void wxToolBoxItem::Expand(void)
{
	this->m_button->SetValue(true);
	this->m_widget->Show();
	this->m_widget->InvalidateBestSize();
	this->InvalidateBestSize();
	this->m_internalLayout->SetSizeHints(this);
}

//!
void wxToolBoxItem::UpdateObserversOfToolBoxItem(void)
{
	// Create a Selected item event and configure it
	wxToolBoxItemSelectedEvent event(this->GetId());
    event.SetEventObject(this);
	event.item = this;

	// Cast the event
	this->GetEventHandler()->ProcessEvent(event);
}

bool wxToolBoxItem::IsCollapsed(void) const
{
	return !this->m_button->GetValue();
}

bool wxToolBoxItem::IsExpanded(void) const
{
	return this->m_button->GetValue();
}

/**
When user toggles the m_button, an EVT_TBOX_ITEM_SELECTED is cast.
*/
void wxToolBoxItem::OnButtonToggled(wxCommandEvent& event)
{
	if(event.IsChecked())
	{
		this->m_widget->Show();
	}
	else
		this->m_widget->Hide();
	this->m_internalLayout->SetSizeHints(this);

	UpdateObserversOfToolBoxItem();
	event.Skip(false);
}




