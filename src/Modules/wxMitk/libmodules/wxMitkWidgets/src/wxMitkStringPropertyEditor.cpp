/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wxMitkStringPropertyEditor.h>
#include <wxUnicode.h>

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkStringPropertyEditor, wxTextCtrl)
EVT_CHAR(wxMitkStringPropertyEditor::onTextChanged)
END_EVENT_TABLE()

wxMitkStringPropertyEditor::wxMitkStringPropertyEditor( mitk::StringProperty* property, wxWindow* parent, const char* name )
: wxTextCtrl( parent, -1),
PropertyEditor( property ),
m_StringProperty(property)
{
	this->SetName(_U(name));
	PropertyChanged();
}

wxMitkStringPropertyEditor::~wxMitkStringPropertyEditor()
{
}

void wxMitkStringPropertyEditor::PropertyChanged()
{
	if ( m_Property )
		SetValue( _U(m_StringProperty->GetValue() ));
}

void wxMitkStringPropertyEditor::PropertyRemoved()
{
	m_Property = NULL;
	m_StringProperty = NULL;
	SetValue(wxT("n/a"));
}

void wxMitkStringPropertyEditor::onTextChanged(wxKeyEvent& event)
{
	BeginModifyProperty();  // deregister from events
	m_StringProperty->SetValue(_U(this->GetValue()));
	EndModifyProperty();  // again register for events
	event.Skip();
}

