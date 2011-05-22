/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXTEXTCTRLVALUEPROXY_H
#define GBLWXTEXTCTRLVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"

namespace gbl
{
	//! Set the text field in a wxTextCtrl
	template <> void SetText(wxTextCtrl* _textCtrl, const std::string& _text);

	//! Set the text field in a wxTextCtrl
	template <> std::string GetText(wxTextCtrl* _textCtrl);
}

#endif //GBLWXTEXTCTRLVALUEPROXY_H
