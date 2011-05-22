/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCHECKABLECONBTROLVALUEPROXY_H
#define GBLWXCHECKABLECONBTROLVALUEPROXY_H

#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "gblValueProxy.h"
#include "wxCheckableControl.h"

namespace gbl
{
	//! Overload to set checkbox contents as text. See gblValueProxy.h
	template <> void SetText(wxCheckableControl* checkbox, const std::string& text);

	//! Overload to get checkbox contents as text. See gblValueProxy.h
	template <> std::string GetText(wxCheckableControl* checkbox);
} // namespace gbl

#endif //GBLWXCHECKABLECONBTROLVALUEPROXY_H
