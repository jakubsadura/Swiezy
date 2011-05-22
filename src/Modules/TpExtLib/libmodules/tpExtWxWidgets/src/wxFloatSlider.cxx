/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxFloatSlider.h"

wxFloatSlider::wxFloatSlider( 
	wxWindow* parent, wxWindowID id, float value , float minValue, 
	float maxValue, float step, const wxPoint& point /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= wxSL_HORIZONTAL*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, const wxString& name /*= "slider"*/ ) 
	: wxSlider( parent, id, value/step, minValue/step, maxValue/step, point, size, style, validator, name )
{
	m_Step = step;
}

float wxFloatSlider::GetValue()
{
	return wxSlider::GetValue() * m_Step;
}

void wxFloatSlider::SetValue( float value )
{
	wxSlider::SetValue( value / m_Step );
}

float wxFloatSlider::GetStep() const
{
	return m_Step;
}

void wxFloatSlider::SetStep( float val )
{
	m_Step = val;
}
