/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxTextCtrlValueProxy.h"

#include <wxUnicode.h>

template <> 
void gbl::SetText( wxTextCtrl* textCtrl, const std::string& text )
{
	wxString s(_U(text)); 
	if( textCtrl->GetValue() != s )
		textCtrl->SetValue( s );
}

template <> 
std::string gbl::GetText( wxTextCtrl* textCtrl )
{
	return _U(textCtrl->GetValue());
}
