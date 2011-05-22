/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxCheckableControlValueProxy.h"

namespace gbl {

template <>
void SetText( wxCheckableControl* checkbox, const std::string& text )
{
	checkbox->SetValue( gbl::ValueConverter::TextToFlag(text) );
}

template <>
std::string GetText( wxCheckableControl* checkbox )
{
	return gbl::ValueConverter::FlagToText( checkbox->GetValue() );
}

}
