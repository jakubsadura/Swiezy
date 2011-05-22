/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxSliderWithSpinCtrl_H
#define wxSliderWithSpinCtrl_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include "wxFloatSlider.h"

/**
wxSlider with wxSpinCtrl 

\author Xavi Planes
\date 20 Jan 2011
\ingroup DynLib
*/
class TPEXTLIBWXWIDGETS_EXPORT wxSliderWithTextCtrl : public wxPanel
{
public:
	//!
	wxSliderWithTextCtrl(wxWindow *parent, 
		const wxWindowID id, float value , float minValue, 
		float maxValue, float step, const wxPoint& point = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxSL_HORIZONTAL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = "slider");

	//!
	float GetValue();

	//!
	std::string GetValueAsString();

	//!
	void SetValueAsString( const std::string &val );

	//!
	void SetValue( float value );

private:
	DECLARE_EVENT_TABLE()

	//!
	void OnTextCtrl(wxCommandEvent& event);

	//!
	void OnSlider(wxScrollEvent& event);

private:
	//!
	wxFloatSlider* m_Slider;
	//!
	wxTextCtrl* m_TextCtrl;
};

#endif // wxSliderWithSpinCtrl_H
