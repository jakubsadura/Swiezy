/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxToolBoxItemSelectedEvent.h"

DEFINE_EVENT_TYPE(wxEVT_TBOX_ITEM_SELECTED)
IMPLEMENT_DYNAMIC_CLASS(wxToolBoxItemSelectedEvent, wxCommandEvent)

//!
wxToolBoxItemSelectedEvent::wxToolBoxItemSelectedEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid)
{
}

//!
wxToolBoxItemSelectedEvent::~wxToolBoxItemSelectedEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxToolBoxItemSelectedEvent::Clone(void)
{
	return new wxToolBoxItemSelectedEvent(*this);
}

//!
wxToolBoxItem* wxToolBoxItemSelectedEvent::GetSelectedItem(void)
{
	return this->item;
}




