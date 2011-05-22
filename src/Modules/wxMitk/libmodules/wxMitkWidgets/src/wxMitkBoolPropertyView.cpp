/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wxMitkBoolPropertyView.h>
#include <wxUnicode.h>

using namespace mitk;

wxMitkBoolPropertyView::wxMitkBoolPropertyView( const mitk::BoolProperty* property, wxWindow* parent, const char* name )
	: wxCheckBox( parent, wxID_Checked, wxT(""), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE),
PropertyView( property ),
m_BoolProperty(property)
{
	this->SetName(_U(name));
	this->SetValue( false );
	this->PropertyChanged();
}

wxMitkBoolPropertyView::~wxMitkBoolPropertyView()
{
}

void wxMitkBoolPropertyView::PropertyChanged()
{
	if ( m_Property )
		this->SetValue( m_BoolProperty->GetValue() );
}

void wxMitkBoolPropertyView::PropertyRemoved()
{
	m_Property = NULL;
	m_BoolProperty = NULL;
	// display "no certain value"
	this->Set3StateValue(wxCHK_UNDETERMINED);
}

