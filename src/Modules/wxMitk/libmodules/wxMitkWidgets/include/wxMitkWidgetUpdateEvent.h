/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkWidgetUpdateEvent_H
#define wxMitkWidgetUpdateEvent_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/event.h>



namespace mitk
{
// Definitions for enabling the custom wxMitkWidgetUpdateEvent event 
// in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(WXMITKWIDGETS_EXPORT, wxEVT_WIDGET_UPDATE, -1)

/**
\brief An event class responsible for signaling that the value or values 
of a widget have changed. The class catching this event should then 
access the widget and retrieve the information from it

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 11 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkWidgetUpdateEvent : public wxCommandEvent
{

public:
	wxMitkWidgetUpdateEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_WIDGET_UPDATE);
	virtual ~wxMitkWidgetUpdateEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();
	
private:

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkWidgetUpdateEvent)
};



// Definitions for enabling the custom wxMitkWidgetUpdateEvent event 
// in wxWidgets ///////////
typedef void (wxEvtHandler::*wxMitkWidgetUpdateEventFunction)
(mitk::wxMitkWidgetUpdateEvent&);
#define wxMitkWidgetUpdateEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
	wxStaticCastEvent( mitk::wxMitkWidgetUpdateEventFunction, & func )
#define EVT_WIDGET_UPDATE(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY( mitk::wxEVT_WIDGET_UPDATE, \
		id, wxID_ANY, wxMitkWidgetUpdateEventHandler(fn), NULL), 


} // namespace mitk


#endif // wxMitkWidgetUpdateEvent_H

