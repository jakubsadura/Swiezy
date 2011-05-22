/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxToggleButtonValueProxy.h"

template <>
void gbl::SetText( wxToggleButton* toggleButton, const std::string& text )
{
	toggleButton->SetValue( gbl::ValueConverter::TextToFlag(text) );
}

template <>
std::string gbl::GetText( wxToggleButton* toggleButton )
{
	return gbl::ValueConverter::FlagToText( toggleButton->GetValue() );
}
