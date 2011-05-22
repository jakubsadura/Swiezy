/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXSLIDERWITHTEXTVALUEPROXY_H
#define GBLWXSLIDERWITHTEXTVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "wxSliderWithTextCtrl.h"

namespace gbl
{
	//! Set the text field in a wxSliderWithTextCtrl
	template <> void SetText(wxSliderWithTextCtrl* slider, const std::string& _text);

	//! Set the text field in a wxSlider
	template <> std::string GetText(wxSliderWithTextCtrl* slider);
}

#endif //GBLWXTEXTCTRLVALUEPROXY_H
