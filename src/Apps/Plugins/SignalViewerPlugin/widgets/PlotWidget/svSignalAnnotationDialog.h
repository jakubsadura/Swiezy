/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalAnnotationDialog_H
#define _svSignalAnnotationDialog_H

#include <wx/wx.h>
#include <wx/image.h>

#include "svSignalAnnotationListBox.h"

/**
Modal dialog to show a signal list box

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 25 Jan 2010 
*/
class svSignalAnnotationDialog: public wxDialog {
public:

    svSignalAnnotationDialog(
		wxWindow* parent, 
		int id, 
		const wxString& title, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	
	svSignalAnnotationListBox* GetAnnotationList();

private:
    void set_properties();
    void do_layout();

	//! Finish modal when pressing ESC
	void OnKeyDown( wxKeyEvent& event );

	//!
	void OnEnterTextAnnotation(wxCommandEvent& event );

	DECLARE_EVENT_TABLE()

protected:
	//!
	svSignalAnnotationListBox* m_AnnotationList;
};

#endif // _svSignalAnnotationDialog_H
