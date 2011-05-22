/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxCheckBoxValueProxy.h"

template <>
void gbl::SetText( wxCheckBox* checkbox, const std::string& text )
{
	checkbox->SetValue( gbl::ValueConverter::TextToFlag(text) );
}

template <>
std::string gbl::GetText( wxCheckBox* checkbox )
{
	return gbl::ValueConverter::FlagToText( checkbox->GetValue() );
}
