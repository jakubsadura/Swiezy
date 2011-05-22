/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCHOICEVALUEPROXY_H
#define GBLWXCHOICEVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "gblWxControlWithItemsValueProxy.h"

namespace gbl
{
	//! Overload to set choice contents as text. See gblValueProxy.h
	template <> inline void SetText(wxChoice* choice, const std::string& text)
	{
		gbl::wx::SetTextOfControlWithItems((wxControlWithItems*)choice, text);
	}

	//! Overload to get choice contents as text. See gblValueProxy.h
	template <> inline std::string GetText(wxChoice* choice)
	{
		return gbl::wx::GetTextOfControlWithItems((wxControlWithItems*)choice);
	}

	//! Overload to set choice contents as a number. See gblValueProxy.h
	template <> inline void SetNumber(wxChoice* choice, double number, const int maxNrOfDecimals)
	{
		gbl::wx::SetNumberOfControlWithItems((wxControlWithItems*)choice, int(number));
	}

	//! Overload to get choice contents as a number. See gblValueProxy.h
	template <> inline double GetNumber(wxChoice* choice)
	{
		return gbl::wx::GetNumberOfControlWithItems((wxControlWithItems*)choice);
	}
} // namespace gbl

#endif //GBLWXCHOICEVALUEPROXY_H
