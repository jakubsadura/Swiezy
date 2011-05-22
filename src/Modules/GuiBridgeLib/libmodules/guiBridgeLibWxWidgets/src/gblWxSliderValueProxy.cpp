/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxSliderValueProxy.h"

#include <wxUnicode.h>

template <> 
void gbl::SetText( wxSlider* slider, const std::string& text )
{
	double num = gbl::ValueConverter::TextToNumber(text);
	if( slider->GetValue() != num )
		slider->SetValue( num );
}

template <> 
std::string gbl::GetText( wxSlider* slider )
{
	return gbl::ValueConverter::NumberToText(slider->GetValue());
}
