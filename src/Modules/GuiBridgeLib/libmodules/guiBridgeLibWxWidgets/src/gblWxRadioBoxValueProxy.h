/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXTOGGLEBOXVALUEPROXY_H
#define GBLWXTOGGLEBOXVALUEPROXY_H

#include "wx/wx.h"
#include "wx/radiobox.h"
#include "gblValueProxy.h"

namespace gbl
{
	//! Overload to set togglebutton contents as text. See gblValueProxy.h
	template <> void SetText(wxRadioBox* radioBox, const std::string& text);

	//! Overload to get checkbox contents as text. See gblValueProxy.h
	template <> std::string GetText(wxRadioBox* radioBox);
} // namespace gbl

#endif //GBLWXTOGGLEBOXVALUEPROXY_H
