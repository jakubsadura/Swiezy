/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalAnnotationListBox_H
#define _svSignalAnnotationListBox_H

#include <wx/listbox.h>

#include "blSignalEvent.h"


/**
List box to show annotations list

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 25 Jan 2010 
*/
class svSignalAnnotationListBox : public wxListBox
{
public:
	svSignalAnnotationListBox(
		wxWindow *parent, 
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		int n = 0, 
		const wxString choices[] = NULL,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxListBoxNameStr);

	//!
	bool GetSelectedEvent( blSignalEvent &event );

protected:

	//! Fill listbox with selected events
	void FillEvents( );

	DECLARE_EVENT_TABLE()

	//! Propagate key to parent to catch ESC
	void OnKeyDown(wxKeyEvent& event);

};

#endif //_svSignalAnnotationListBox_H
