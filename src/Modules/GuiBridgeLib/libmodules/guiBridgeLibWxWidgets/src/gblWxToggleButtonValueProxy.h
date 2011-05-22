/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXTOGGLEBUTTONVALUEPROXY_H
#define GBLWXTOGGLEBUTTONVALUEPROXY_H

#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "gblValueProxy.h"

namespace gbl
{
	//! Overload to set togglebutton contents as text. See gblValueProxy.h
	template <> void SetText(wxToggleButton* toggleButton, const std::string& text);

	//! Overload to get checkbox contents as text. See gblValueProxy.h
	template <> std::string GetText(wxToggleButton* toggleButton);
} // namespace gbl

#endif //GBLQTOGGLEBUTTONVALUEPROXY_H
