/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCHOICEBOOKVALUEPROXY_H
#define GBLWXCHOICEBOOKVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "gblWxControlWithItemsValueProxy.h"
#include "boost/format.hpp"
#include "gblWxChoiceBookValueProxy.h"
#include <wx/choicebk.h>

#include <wxUnicode.h>

namespace gbl
{
	//! Overload to set listBox contents as text. See gblValueProxy.h
	template<>
	inline void SetText(wxChoicebook* choiceBk, const std::string& text)
	{
		bool found(false);
		for( unsigned int i = 0; i < choiceBk->GetPageCount(); ++i )
		{
			wxString wxtext(_U(text)); 
			if( choiceBk->GetPageText(i) == wxtext )
			{
				found = true;
				choiceBk->SetSelection(i);
			}
		}
		if( !found )
		{
			std::string message = (boost::format("Cannot find choice book item %1%") % text).str();
			throw gbl::Exception(message.c_str());
		}
	}

	//! Overload to get listbox contents as text. See gblValueProxy.h
	template<>
	inline std::string GetText(wxChoicebook* choiceBk)
	{
		int currentSelectedPage = choiceBk->GetSelection();
		return _U(choiceBk->GetPageText( currentSelectedPage ));
	}

	//! Overload to set listBox contents as a number. See gblValueProxy.h
	template<>
	inline void SetNumber(wxChoicebook* choiceBk, double number, const int maxNrOfDecimals)
	{
		choiceBk->SetSelection(number);
	}

	//! Overload to get listbox contents as a number. See gblValueProxy.h
	template<>
	inline double GetNumber(wxChoicebook* choiceBk)
	{
		return choiceBk->GetSelection();
	}

} // namespace gbl

#endif //GBLWXCHOICEBOOKVALUEPROXY_H
