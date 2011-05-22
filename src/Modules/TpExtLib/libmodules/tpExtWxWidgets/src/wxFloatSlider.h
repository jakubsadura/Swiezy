/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxFloatSlider_H
#define wxFloatSlider_H

#include "TpExtLibWxWidgetsWin32Header.h"

/**
wxSlider with float values

\author Xavi Planes
\date 20 Jan 2011
\ingroup DynLib
*/
class TPEXTLIBWXWIDGETS_EXPORT wxFloatSlider : public wxSlider
{
public:
	wxFloatSlider(
		wxWindow* parent, wxWindowID id, float value , float minValue, 
		float maxValue, float step, const wxPoint& point = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxSL_HORIZONTAL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = "slider");

	float GetValue();

	void SetValue( float value );

	float GetStep() const;
	void SetStep(float val);

private:
	//!
	float m_Step;
};

#endif // wxFloatSlider_H
