/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxToolBoxItemSelectedEvent_H
#define wxToolBoxItemSelectedEvent_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include "wxToolBoxItem.h"
#include <wx/panel.h>
#include <list>

class wxBoxSizer;
class wxToggleButton;
class wxColour;

// Definitions for enabling the custom wxToolBoxItemSelectedEvent 
// event in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(TPEXTLIBWXWIDGETS_EXPORT, wxEVT_TBOX_ITEM_SELECTED, -1)

/**
\brief An event class responsible for signaling that the user selected
another item of the toolbox, in a wxToolBoxItem
\sa wxToolBoxItemSelectedEvent

\ingroup TpExtLibWxWidgets
\author Juan Antonio Moya
\date 08 Jan 2008
*/
class TPEXTLIBWXWIDGETS_EXPORT wxToolBoxItemSelectedEvent 
	: public wxCommandEvent
{

friend class wxToolBoxItem;

public:
	wxToolBoxItemSelectedEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_TBOX_ITEM_SELECTED);
	virtual ~wxToolBoxItemSelectedEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	wxToolBoxItem* GetSelectedItem(void);

private:
	wxToolBoxItem* item;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxToolBoxItemSelectedEvent)
};


// Definitions for enabling the custom wxToolBoxItemSelectedEvent 
// event in wxWidgets ///////////
typedef void (wxEvtHandler::*wxToolBoxItemSelectedEventFunction)
(wxToolBoxItemSelectedEvent&);
#define wxToolBoxItemSelectedEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxToolBoxItemSelectedEventFunction, & func )
#define EVT_TBOX_ITEM_SELECTED(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		wxEVT_TBOX_ITEM_SELECTED, \
		id, wxID_ANY, wxToolBoxItemSelectedEventHandler(fn), NULL), 


// Definitions for enabling the custom wxToolBoxItemSelectedEvent
// event in wxWidgets ///////////
typedef void (wxEvtHandler::*wxToolBoxItemSelectedEventFunction)
(wxToolBoxItemSelectedEvent&);
#define wxToolBoxItemSelectedEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxToolBoxItemSelectedEventFunction, & func )
#define EVT_TBOX_ITEM_SELECTED(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		wxEVT_TBOX_ITEM_SELECTED, \
			id, wxID_ANY, wxToolBoxItemSelectedEventHandler(fn), NULL), 



#endif // wxToolBoxItemSelectedEvent_H
