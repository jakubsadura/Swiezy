/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxFloatSliderValueProxy.h"

#include <wxUnicode.h>

template <> 
void gbl::SetText( wxFloatSlider* slider, const std::string& text )
{
	double num = gbl::ValueConverter::TextToNumber(text);
	if( slider->GetValue() != num )
		slider->SetValue( num );
}

template <> 
std::string gbl::GetText( wxFloatSlider* slider )
{
	return gbl::ValueConverter::NumberToText(slider->GetValue());
}
