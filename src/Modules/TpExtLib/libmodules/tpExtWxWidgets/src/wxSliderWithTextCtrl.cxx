/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxSliderWithTextCtrl.h"
#include <cmath>
#include <limits>
#include <sstream>

BEGIN_EVENT_TABLE(wxSliderWithTextCtrl, wxPanel)
  EVT_TEXT(wxID_ANY, wxSliderWithTextCtrl::OnTextCtrl)
  EVT_COMMAND_SCROLL(wxID_ANY, wxSliderWithTextCtrl::OnSlider)
END_EVENT_TABLE()


//! Retains maximal precision.
std::string NumberToText(double _x, const int _maxNrOfDecimals = 3)
{
	std::stringstream ss;
	ss.precision(std::numeric_limits<double>::digits10 + 2);
	ss << std::fixed  << _x;

	//used to find the first significant digit
	std::stringstream scientific_ss;
	scientific_ss.precision(std::numeric_limits<double>::digits10 + 2);
	scientific_ss << std::scientific <<_x;

	std::string result = ss.str();
	size_t n = result.find('.');
	// find the first significant digit
	bool found = false;
	int lastSignificantZero = n + 1;

	for( size_t m = n+1; m < result.length() && !found; m++)
	{
		if( result[m] == '0' )
		{
			lastSignificantZero = m;
		}
		else
			found = true;
	}

	const int nrDecimals = result.length() - n - 1;
	if( n != std::string::npos && /*nrDecimals*/ lastSignificantZero > _maxNrOfDecimals && found )
	{
		//result = result.substr(0, n + 1 + _maxNrOfDecimals);
		result = result.substr(0, n + 1 + lastSignificantZero);
	}	
	else
	{
		if ( _maxNrOfDecimals == 0 )
		{
			result = result.substr(0, n );
		}
		else
		{
			result = result.substr(0, n + 1 +_maxNrOfDecimals);
		}
	}

	return result;
}

wxSliderWithTextCtrl::wxSliderWithTextCtrl( 
	wxWindow *parent, const wxWindowID id, 
	float value , float minValue, 
	float maxValue, float step, const wxPoint& point /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= wxSL_HORIZONTAL*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, 
	const wxString& name /*= "slider"*/ ) : wxPanel( parent, id, point, size, style )
{
	wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer( sizer );

	m_Slider = new wxFloatSlider(this, wxID_ANY, value, minValue, maxValue, step);
	m_TextCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_TextCtrl->SetSize( 50, 20 );
	m_TextCtrl->SetMaxSize( wxSize( 50, 20 ) );
	m_TextCtrl->SetMinSize( wxSize( 50, 20 ) );
	GetSizer()->Add( m_Slider, wxSizerFlags().Expand().Proportion(1).Align( wxALIGN_CENTER_VERTICAL ) );
	GetSizer()->Add( m_TextCtrl, wxSizerFlags().Align( wxALIGN_CENTER_VERTICAL ) );
}

float wxSliderWithTextCtrl::GetValue()
{
	return atof( m_TextCtrl->GetValue().c_str( ) );
}

void wxSliderWithTextCtrl::SetValue( float value )
{
	m_Slider->SetValue( value );
	int decimals = m_Slider->GetStep( ) == 1 ? 0 : 3;
	m_TextCtrl->SetValue( NumberToText( value, decimals ) );
}

void wxSliderWithTextCtrl::OnTextCtrl(wxCommandEvent& event)
{
	m_Slider->SetValue( atof( m_TextCtrl->GetValue().c_str( ) ) );
	event.Skip();
}

void wxSliderWithTextCtrl::OnSlider( wxScrollEvent& event )
{
	int decimals = m_Slider->GetStep( ) == 1 ? 0 : 3;
	m_TextCtrl->SetValue( NumberToText( m_Slider->GetValue(), decimals ) );
	event.Skip();
}

std::string wxSliderWithTextCtrl::GetValueAsString()
{
	return m_TextCtrl->GetValue().c_str();
}

void wxSliderWithTextCtrl::SetValueAsString( const std::string &val )
{
	m_TextCtrl->SetValue( val );
}
