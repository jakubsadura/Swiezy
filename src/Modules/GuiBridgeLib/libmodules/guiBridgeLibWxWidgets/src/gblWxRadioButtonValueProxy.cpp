/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxRadioButtonValueProxy.h"
#include "gblWxEventHandler.h"

template <>
void gbl::SetText( wxRadioButton* radioButton, const std::string& text )
{
	radioButton->SetValue( gbl::ValueConverter::TextToFlag(text) );
}

template <>
std::string gbl::GetText( wxRadioButton* radioButton )
{
	return gbl::ValueConverter::FlagToText( radioButton->GetValue() );
}
