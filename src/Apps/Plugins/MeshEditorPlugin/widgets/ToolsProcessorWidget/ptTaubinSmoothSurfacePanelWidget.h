/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef ptTaubinSmoothSurfacePanelWidget_H
#define ptTaubinSmoothSurfacePanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKSmoothFilter.h"

#define wxID_btnTaubinSmoothSurface wxID_HIGHEST + 1


/**
\brief Pw for taubin smooth processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptTaubinSmoothSurfacePanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:

	typedef Core::VTKProcessor<meVTKSmoothFilter> TaubinSmoothProcessor;

	coreDefineBaseWindowFactory( ptTaubinSmoothSurfacePanelWidget )

	//!
    ptTaubinSmoothSurfacePanelWidget(wxWindow* parent, 
									int id = wxID_ANY, 
									const wxPoint& pos=wxDefaultPosition, 
									const wxSize& size=wxDefaultSize, 
									long style=0);

	virtual void OnButtonTaubinSmoothSurface(wxCommandEvent &event); // wxGlade: <event_handler>

private:
    void do_layout();

    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    // begin wxGlade: ptTaubinSmoothSurfacePanelWidget::attributes
    wxButton* btnTaubinSmoothSurface;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	TaubinSmoothProcessor::Pointer m_processor;
}; // wxGlade: end class


#endif // ptTaubinSmoothSurfacePanelWidget_H
