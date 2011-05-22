/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkWidgetStackControl_H
#define wxMitkWidgetStackControl_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include <wx/panel.h>
#include <map>

class wxBoxSizer;

// Definitions for enabling the custom wxWidgetRisenEvent event 
// in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(TPEXTLIBWXWIDGETS_EXPORT, wxEVT_WIDGET_RISEN, -1)

/**
\brief An event class responsible for signaling that the widget on top 
has changed, in a wxWidgetStackControl
\sa wxWidgetStackControl

\ingroup TpExtLibWxWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class TPEXTLIBWXWIDGETS_EXPORT wxWidgetRisenEvent : public wxCommandEvent
{

friend class wxWidgetStackControl;

public:
	wxWidgetRisenEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_WIDGET_RISEN);
	virtual ~wxWidgetRisenEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	wxWindow* GetWidgetOnTop(void);
	int GetWidgetIndexOnTop(void);
	wxWindow* GetPreviousWidgetOnTop(void);
	int GetPreviousWidgetIndexOnTop(void);

private:
	wxWindow* m_currentWidget;
	wxWindow* m_previousWidget;
	int m_currentIndex;
	int m_previousIndex;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxWidgetRisenEvent)
};


// Definitions for enabling the custom wxWidgetRisenEvent event in 
// wxWidgets ///////////
typedef void (wxEvtHandler::*wxWidgetRisenEventFunction)
(wxWidgetRisenEvent&);
#define wxWidgetRisenEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxWidgetRisenEventFunction, & func )
#define EVT_WIDGET_RISEN(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		wxEVT_WIDGET_RISEN, \
			id, wxID_ANY, wxWidgetRisenEventHandler(fn), NULL), 


// ------------------------------------------------------------------------
/*
The wxWidgetStackControl class provides a stack of widgets of which 
only the top widget is user-visible.

The application programmer can move any widget to the top of the stack at 
any time using Raise(), and add or remove widgets using Add() and Remove().

wxWidgetStackControl also provides the ability to manipulate widgets 
through application-specified integer IDs. You can also translate from 
widget pointers to IDs using GetIndexOfWidget() and from IDs to widget 
pointers using GetWidgetByIndex(). These numeric IDs are unique in the 
wxWidgetStackControl context, but it does not attach any additional 
meaning to them.

The default widget stack is frameless, but you can use the usual wxWindow 
Styles to add a frame.

wxWidgetStackControl emits a wxWidgetRisenEvent when the user 
changes the widget on top. You may catch this by using the EVT_WIDGET_RISEN
macro.

\ingroup TpExtLibWxWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class TPEXTLIBWXWIDGETS_EXPORT wxWidgetStackControl : public wxPanel
{
protected:
	typedef std::map<int, wxWindow*> WindowMap;
	typedef WindowMap::value_type WindowMapElement;
	typedef std::pair<WindowMap::iterator, bool> WindowMapInsertResult;

public:
	wxWidgetStackControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxWidgetStackControl"));
	virtual ~wxWidgetStackControl(void);

	int Add(wxWindow* widget);
	void Remove(wxWindow* widget);
	void Remove(int widgetIndex);
	void Raise(wxWindow* widget);
	void Raise(int widgetIndex);
	void Clear(void);

	wxWindow* GetWidgetOnTop(void) const;
	unsigned int GetWidgetIndexOnTop(void) const;
	int GetIndexOfWidget(wxWindow* widget) const;
	wxWindow* GetWidgetByIndex(int widgetIndex) const;
	int GetNumberOfWidgets( );

protected:
	void ClearCurrentWidget(void);
	
private:
	void UpdateObserversOfWidgetStack(
		int previousIndex, 
		wxWindow* previousWidget);

	WindowMap m_registeredWindowList;
	wxWindow* m_currentWidget;
	int m_currentIndex;
};
 

#endif // wxMitkWidgetStackControl_H
