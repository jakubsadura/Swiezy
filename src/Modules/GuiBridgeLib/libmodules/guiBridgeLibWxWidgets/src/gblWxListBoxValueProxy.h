/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXLISTBOXVALUEPROXY_H
#define GBLWXLISTBOXVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "gblWxControlWithItemsValueProxy.h"

namespace gbl
{
	//! Overload to set listBox contents as text. See gblValueProxy.h
	template<>
	inline void SetText(wxListBox* listBox, const std::string& text)
	{
		gbl::wx::SetTextOfControlWithItems(listBox, text);
	}

	//! Overload to get listbox contents as text. See gblValueProxy.h
	template<>
	inline std::string GetText(wxListBox* listBox)
	{
		return gbl::wx::GetTextOfControlWithItems(listBox);
	}

	//! Overload to set listBox contents as a number. See gblValueProxy.h
	template<>
	inline void SetNumber(wxListBox* listBox, double number, const int maxNrOfDecimals)
	{
		gbl::wx::SetNumberOfControlWithItems(listBox, int(number));
	}

	//! Overload to get listbox contents as a number. See gblValueProxy.h
	template<>
	inline double GetNumber(wxListBox* listBox)
	{
		return gbl::wx::GetNumberOfControlWithItems(listBox);
	}

} // namespace gbl

#endif //GBLWXLISTBOXVALUEPROXY_H
