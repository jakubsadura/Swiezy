/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#ifndef _svPlotWidgetToolbar_H
#define _svPlotWidgetToolbar_H

class svPlotManagerWidget;

/**
\brief Layout for plot widgets
\ingroup SignalViewerPlugin
\author Xavi Planes
\date 4 Jan 2010
*/
class svPlotWidgetToolbar: public wxToolBar {
public:

	svPlotWidgetToolbar(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_DOCKABLE, 
		const wxString& name = wxPanelNameStr);

	//!
	svPlotManagerWidget* GetsvPlotManagerWidget() const;
	void SetsvPlotManagerWidget(svPlotManagerWidget* val);

protected:

	//!
    void OnBtnAnnotations(wxCommandEvent &event);

	//!
	void OnBtnSynchronize(wxCommandEvent &event);
	
	//!
	void OnBtnFilterAnnotations(wxCommandEvent &event);

	//!
	void OnBtnConfiguration(wxCommandEvent &event);
	
	DECLARE_EVENT_TABLE();
private:

	//!
	svPlotManagerWidget* m_svPlotManagerWidget;
};

#endif // _svPlotWidgetToolbar_H
