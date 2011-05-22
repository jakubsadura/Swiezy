/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalFreeAnnotationDialog_H
#define _svSignalFreeAnnotationDialog_H


#include "svSignalFreeAnnotationDialogUI.h"

/**
Modal dialog to show a text control

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 9 June 2010 
*/
class svSignalFreeAnnotationDialog: public svSignalFreeAnnotationDialogUI {
public:
    svSignalFreeAnnotationDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

	//!
	wxTextCtrl* GetTxtName() const;
private:

	//!
	void OnTextEnter(wxCommandEvent &event);

protected:
};


#endif // _svSignalFreeAnnotationDialog_H
