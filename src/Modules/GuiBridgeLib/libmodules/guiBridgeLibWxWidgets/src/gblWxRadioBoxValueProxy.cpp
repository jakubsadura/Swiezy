/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxRadioBoxValueProxy.h"
#include "gblWxEventHandler.h"

#include <wxUnicode.h>

template <>
void gbl::SetText( wxRadioBox* radioBox, const std::string& text )
{
	wxString s(_U(text)); 
	if( radioBox->GetStringSelection() != s )
		radioBox->SetStringSelection( s );
}

template <>
std::string gbl::GetText( wxRadioBox* radioBox )
{
	return _U( radioBox->GetStringSelection() );
}
