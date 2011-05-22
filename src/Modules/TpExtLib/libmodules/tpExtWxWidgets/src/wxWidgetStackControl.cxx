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

#include <wx/wupdlock.h>

#include "wxWidgetStackControl.h"


//!
wxWidgetStackControl::wxWidgetStackControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name), m_currentWidget(NULL), m_currentIndex(-1)
{
	// Layout
	SetAutoLayout(true);
	SetSizer( new wxBoxSizer(wxHORIZONTAL) );
}

//!
wxWidgetStackControl::~wxWidgetStackControl(void)
{
}

/**
\brief Adds a widget (wxWindow*) to the widget stack
\return index assigned to the widget added. It is an unique identifier for that widget, that is internally used
*/
int wxWidgetStackControl::Add(wxWindow* widget)
{
	if(widget == NULL || GetIndexOfWidget( widget ) != -1 )
		return -1;

	widget->Hide();
	int index = m_registeredWindowList.size();
	wxWidgetStackControl::WindowMapElement value(index, widget);
	wxWidgetStackControl::WindowMapInsertResult ret = m_registeredWindowList.insert(value);

	// If insertion could not take place, return -1 signaling an error
	if(!ret.second)
		return -1;

	widget->Hide();
	widget->Reparent(this);

	return index;

}

/**
\brief Removes a widget (wxWindow*) from the widget stack. 
If the widget removed was the widget on top, then you have to Raise another widget, or nothing will be displayed.
*/
void wxWidgetStackControl::Remove(wxWindow* widget)
{
	bool found = false;
	wxWidgetStackControl::WindowMap::iterator it;
	for( it = m_registeredWindowList.begin();
		it != m_registeredWindowList.end(); ++it)
	{
		if((*it).second == widget)
		{
			found = true;
			break;
		}
	}

	if ( found )
	{
		// if it is the widget at top, redo the layout
		if(widget == m_currentWidget || (*it).first == m_currentIndex)
			ClearCurrentWidget();

		// Erase the widget from the list
		m_registeredWindowList.erase(it);
	}

}

/**
\brief Removes a widget (wxWindow*) from the widget stack, given its index. 
If the widget removed was the widget on top, then you have to Raise another widget, or nothing will be displayed.

The widget is selected by its assigned index, when the Add function was called for that widget.
*/
void wxWidgetStackControl::Remove(int widgetIndex)
{
	wxWidgetStackControl::WindowMap::iterator it = m_registeredWindowList.find(widgetIndex);
	if(it != m_registeredWindowList.end())
	{
		// if it is the widget at top, redo the layout
		if((*it).first == m_currentIndex || (*it).second == m_currentWidget)
			ClearCurrentWidget();
		
		// Erase the widget from the list
		m_registeredWindowList.erase(it);
	}
}

/**
Tells the widget stack to display that widget (wxWindow*) and hide the others. 
Raising a widget casts a wxWidgetRisenEvent
*/
void wxWidgetStackControl::Raise(wxWindow* widget)
{
	int widgetIndex, previousIndex;
	wxWindow* previousWidget;

	if(widget == NULL || (widgetIndex = GetIndexOfWidget(widget)) < 0)
		return;

	if ( widget == GetWidgetOnTop( ) )
	{
		return;
	}

	wxWindowUpdateLocker noUpdates( this );

	if(m_currentWidget != NULL)
		m_currentWidget->Hide();

	previousIndex = m_currentIndex;
	previousWidget = m_currentWidget;
	m_currentIndex = widgetIndex;
	m_currentWidget = widget;
	GetSizer()->Clear(false);
	GetSizer()->Add(widget, 1, wxEXPAND);

	// Don't call wiget->Show( ) because the scroll bar is not updated
	GetSizer()->Show( widget, true, true );

	wxSizeEvent resEvent(this->GetBestSize(), this->GetId());
	resEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(resEvent);

	UpdateObserversOfWidgetStack(previousIndex, previousWidget);
}

/**
Tells the widget stack to display that widget (wxWindow*) and hide the others. Raising a widget casts a wxWidgetRisenEvent
The widget is selected by its assigned index, when the Add function was called for that widget.
*/
void wxWidgetStackControl::Raise(int widgetIndex)
{
	wxWindow* widget, *previousWidget;
	int previousIndex;

	if(widgetIndex < 0 || (widget = GetWidgetByIndex(widgetIndex)) == NULL)
		return;

	if(m_currentWidget != NULL)
		m_currentWidget->Hide();

	previousIndex = m_currentIndex;
	previousWidget = m_currentWidget;
	m_currentIndex = widgetIndex;
	m_currentWidget = widget;
	GetSizer()->Clear(false);
	GetSizer()->Add(widget, 1, wxEXPAND);
	widget->Show();
	// Cast a resize event
	wxSizeEvent event(GetBestSize(), GetId());
    event.SetEventObject(this);
	GetEventHandler()->ProcessEvent(event);

	UpdateObserversOfWidgetStack(previousIndex, previousWidget);
}

/**
\brief Clears the widget stack. 
It does not delete nor manage the memmory for the widgets (wxWindow*) that had mapped. It clears also the client
area occupied by the control, so it displays nothing but its background.
*/
void wxWidgetStackControl::Clear(void)
{
	ClearCurrentWidget();
	m_registeredWindowList.clear();
}

/**
\brief Returns the widget currently at the top (being displayed). You may call the Raise function to make a widget being displayed at the top.
*/
wxWindow* wxWidgetStackControl::GetWidgetOnTop(void) const 
{
	return m_currentWidget;
}

/**
\brief Returns the assigned index of the widget currently at the top (being displayed). You may call the Raise function to make a widget being displayed at the top.
The assigned index is retrieved by the Add function was called for that widget.
*/
unsigned int wxWidgetStackControl::GetWidgetIndexOnTop(void) const
{
	return m_currentIndex;
}


//!
int wxWidgetStackControl::GetIndexOfWidget(wxWindow* widget) const 
{
	for(wxWidgetStackControl::WindowMap::const_iterator it = m_registeredWindowList.begin();
		it != m_registeredWindowList.end(); ++it)
	{
		if((*it).second == widget)
			return (*it).first;
	}
	return -1;
}


//!
wxWindow* wxWidgetStackControl::GetWidgetByIndex(int widgetIndex) const 
{
	wxWidgetStackControl::WindowMap::const_iterator it = m_registeredWindowList.find(widgetIndex);
	if(it == m_registeredWindowList.end())
		return NULL;
	return (*it).second;
}



//!
void wxWidgetStackControl::ClearCurrentWidget(void)
{
	if(m_currentWidget != NULL)
		m_currentWidget->Hide();
	m_currentIndex = -1;
	m_currentWidget = NULL;
	GetSizer( )->Clear(false);
}

/**
When user changes the representation, an EVT_WIDGET_RISEN is cast.
*/
void wxWidgetStackControl::UpdateObserversOfWidgetStack(int previousIndex, wxWindow* previousWidget)
{
	// Create a Color Function changed event and configure it
	wxWidgetRisenEvent event(GetId());
    event.SetEventObject(this);
	event.m_currentWidget = m_currentWidget;
	event.m_currentIndex = m_currentIndex;
	event.m_previousWidget = previousWidget;
	event.m_previousIndex = previousIndex;

	// Cast the event
	GetEventHandler()->ProcessEvent(event);
}

int wxWidgetStackControl::GetNumberOfWidgets()
{
	return m_registeredWindowList.size();
}



//////////////////////////////////////////////////////////////////////////////////
//
// Code for the wxWidgetRisenEvent
//
//////////////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxEVT_WIDGET_RISEN)
IMPLEMENT_DYNAMIC_CLASS(wxWidgetRisenEvent, wxCommandEvent)

//!
wxWidgetRisenEvent::wxWidgetRisenEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid)
{
}

//!
wxWidgetRisenEvent::~wxWidgetRisenEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxWidgetRisenEvent::Clone(void)
{
	return new wxWidgetRisenEvent(*this);
}

//!
wxWindow* wxWidgetRisenEvent::GetWidgetOnTop(void)
{
	return m_currentWidget;
}

//!
int wxWidgetRisenEvent::GetWidgetIndexOnTop(void)
{
	return m_currentIndex;
}

//!
wxWindow* wxWidgetRisenEvent::GetPreviousWidgetOnTop(void)
{
	return m_previousWidget;
}

//!
int wxWidgetRisenEvent::GetPreviousWidgetIndexOnTop(void)
{
	return m_previousIndex;
}
