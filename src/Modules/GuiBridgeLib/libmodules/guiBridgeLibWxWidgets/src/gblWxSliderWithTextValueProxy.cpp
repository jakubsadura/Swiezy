/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxSliderWithTextValueProxy.h"

#include <wxUnicode.h>

template <> 
void gbl::SetText( wxSliderWithTextCtrl* slider, const std::string& text )
{
	if( slider->GetValueAsString() != text )
		slider->SetValueAsString( text );
}

template <> 
std::string gbl::GetText( wxSliderWithTextCtrl* slider )
{
	return slider->GetValueAsString();
}
