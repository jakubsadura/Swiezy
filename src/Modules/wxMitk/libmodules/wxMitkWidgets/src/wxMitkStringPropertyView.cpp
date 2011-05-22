/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wxMitkStringPropertyView.h>
#include <wxUnicode.h>

using namespace mitk;

wxMitkStringPropertyView::wxMitkStringPropertyView( const mitk::StringProperty* property, wxWindow* parent, const char* name )
	: wxStaticText( parent, -1, wxT("")),
PropertyView( property ),
m_StringProperty(property)
{
	this->SetName(_U(name));
	PropertyChanged();
}

wxMitkStringPropertyView::~wxMitkStringPropertyView()
{
}

void wxMitkStringPropertyView::PropertyChanged()
{
	if ( m_Property )
		SetLabel( _U(m_StringProperty->GetValue()) );
}

void wxMitkStringPropertyView::PropertyRemoved()
{
	m_Property = NULL;
	m_StringProperty = NULL;
	SetLabel(wxT("n/a"));
}

