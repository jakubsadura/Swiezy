/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxControlWithItemsValueProxy.h"
#include "boost/format.hpp"

#include <wxUnicode.h>

void gbl::wx::SetTextOfControlWithItems( wxControlWithItems* listBox, const std::string& text )
{
	size_t i = listBox->FindString(_U(text));
	if( i >= 0 && i < listBox->GetCount() )
	{
		listBox->SetSelection( i );
	}
	else
	{
		std::string message = (boost::format("Cannot find list box item %1%") % text).str();
		throw gbl::Exception(message.c_str());
	}
}

std::string gbl::wx::GetTextOfControlWithItems( wxControlWithItems* listBox )
{
	return _U(listBox->GetStringSelection());
}

double gbl::wx::GetNumberOfControlWithItems( wxControlWithItems* listBox )
{
	return listBox->GetSelection();
}

void gbl::wx::SetNumberOfControlWithItems( wxControlWithItems* listBox, int number)
{
	listBox->SetSelection(number);
}
