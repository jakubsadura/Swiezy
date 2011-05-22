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

#include "wxMitkWidgetUpdateEvent.h"


using namespace mitk;

DEFINE_EVENT_TYPE(mitk::wxEVT_WIDGET_UPDATE)
IMPLEMENT_DYNAMIC_CLASS(wxMitkWidgetUpdateEvent, wxCommandEvent)

//!
wxMitkWidgetUpdateEvent::wxMitkWidgetUpdateEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid)
{
}

//!
wxMitkWidgetUpdateEvent::~wxMitkWidgetUpdateEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxMitkWidgetUpdateEvent::Clone(void)
{
	return new wxMitkWidgetUpdateEvent(*this);
}
