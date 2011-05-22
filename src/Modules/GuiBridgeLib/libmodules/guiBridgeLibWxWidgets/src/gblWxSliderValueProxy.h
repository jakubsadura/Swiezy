/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXSLIDERVALUEPROXY_H
#define GBLWXSLIDERVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"

namespace gbl
{
	//! Set the text field in a wxSlider
	template <> void SetText(wxSlider* slider, const std::string& _text);

	//! Set the text field in a wxSlider
	template <> std::string GetText(wxSlider* slider);
}

#endif //GBLWXTEXTCTRLVALUEPROXY_H
