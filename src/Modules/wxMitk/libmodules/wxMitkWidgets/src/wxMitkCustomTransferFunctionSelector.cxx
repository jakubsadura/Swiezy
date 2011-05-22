/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkCustomTransferFunctionSelector.h"

using namespace mitk;


//!
wxMitkCustomTransferFunctionSelector::wxMitkCustomTransferFunctionSelector(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxChoice(parent, id, pos, size, 0, NULL, style, validator, name)
{
}

//!
wxMitkCustomTransferFunctionSelector::~wxMitkCustomTransferFunctionSelector(void)
{
}

