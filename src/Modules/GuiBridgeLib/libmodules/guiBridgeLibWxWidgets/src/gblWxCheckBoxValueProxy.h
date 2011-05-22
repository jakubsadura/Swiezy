/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCHECKBOXVALUEPROXY_H
#define GBLWXCHECKBOXVALUEPROXY_H

#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "gblValueProxy.h"
#include "wxCheckableControl.h"

namespace gbl
{
	//! Overload to set checkbox contents as text. See gblValueProxy.h
	template <> void SetText(wxCheckBox* checkbox, const std::string& text);

	//! Overload to get checkbox contents as text. See gblValueProxy.h
	template <> std::string GetText(wxCheckBox* checkbox);
} // namespace gbl

#endif //GBLQCHECKBOXVALUEPROXY_H
