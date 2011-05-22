/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXRADIOBUTTONVALUEPROXY_H
#define GBLWXRADIOBUTTONVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"

namespace gbl
{
	//! Overload to set radiobutton contents as text. See gblValueProxy.h
	template <> void SetText(wxRadioButton* radioButton, const std::string& text);

	//! Overload to get checkbox contents as text. See gblValueProxy.h
	template <> std::string GetText(wxRadioButton* radioButton);
} // namespace gbl

#endif //GBLQRADIOBUTTONVALUEPROXY_H
