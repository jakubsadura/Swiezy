/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXFLOATSLIDERVALUEPROXY_H
#define GBLWXFLOATSLIDERVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "wxFloatSlider.h"

namespace gbl
{
	//! Set the text field in a wxSlider
	template <> void SetText(wxFloatSlider* slider, const std::string& _text);

	//! Set the text field in a wxSlider
	template <> std::string GetText(wxFloatSlider* slider);
}

#endif //GBLWXTEXTCTRLVALUEPROXY_H
