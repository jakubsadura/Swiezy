/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wxMitkStringPropertyOnDemandEdit.h>
#include <wx/textdlg.h>
#include <wxUnicode.h>
#include <wx/sizer.h>

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkStringPropertyOnDemandEdit, wxFrame)
EVT_LEFT_DOWN(wxMitkStringPropertyOnDemandEdit::onToolButtonClicked)
END_EVENT_TABLE()

wxMitkStringPropertyOnDemandEdit::wxMitkStringPropertyOnDemandEdit( mitk::StringProperty* property, wxWindow* parent, const char* name )
: wxFrame( parent, -1, wxT("")),
PropertyEditor( property ),
m_StringProperty(property)
{
	this->SetName(_U(name));
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	m_label = new wxStaticText(this, -1, wxT(""));
	sizer->Add(m_label, wxSizerFlags() .Expand());

	m_toolbutton = new wxStaticText(this, -1, wxT("..."));
	sizer->Add(m_toolbutton, wxSizerFlags() .Border(wxLEFT, 3));

	PropertyChanged();
}

wxMitkStringPropertyOnDemandEdit::~wxMitkStringPropertyOnDemandEdit()
{
}

void wxMitkStringPropertyOnDemandEdit::PropertyChanged()
{
	if ( m_Property )
		m_label->SetLabel( _U(m_StringProperty->GetValue()) );
}

void wxMitkStringPropertyOnDemandEdit::PropertyRemoved()
{
	m_Property = NULL;
	m_StringProperty = NULL;
	m_label->SetLabel(wxT("n/a"));
}

void wxMitkStringPropertyOnDemandEdit::onToolButtonClicked(wxMouseEvent& event)
{
	wxString newText = wxGetTextFromUser(wxT("Enter value"), wxT("New value"), m_label->GetLabel());

	if (newText != wxT(""))
	{
		BeginModifyProperty();  // deregister from events
		m_StringProperty->SetValue(_U(newText));
		m_label->SetLabel(newText);
		EndModifyProperty();  // again register for events
	}
}
