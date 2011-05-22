/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCOMBOBOXVALUEPROXY_H
#define GBLWXCOMBOBOXVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "gblWxControlWithItemsValueProxy.h"

namespace gbl
{
	//! Overload to set comboBox contents as text. See gblValueProxy.h
	template <> inline void SetText(wxComboBox* comboBox, const std::string& text)
	{
		gbl::wx::SetTextOfControlWithItems((wxControlWithItems*)comboBox, text);
	}

	//! Overload to get combobox contents as text. See gblValueProxy.h
	template <> inline std::string GetText(wxComboBox* comboBox)
	{
		return gbl::wx::GetTextOfControlWithItems((wxControlWithItems*)comboBox);
	}

	//! Overload to set comboBox contents as a number. See gblValueProxy.h
	template <> inline void SetNumber(wxComboBox* comboBox, double number, const int maxNrOfDecimals)
	{
		gbl::wx::SetNumberOfControlWithItems((wxControlWithItems*)comboBox, int(number));
	}

	//! Overload to get combobox contents as a number. See gblValueProxy.h
	template <> inline double GetNumber(wxComboBox* comboBox)
	{
		return gbl::wx::GetNumberOfControlWithItems((wxControlWithItems*)comboBox);
	}
} // namespace gbl

#endif //GBLWXLISTBOXVALUEPROXY_H
