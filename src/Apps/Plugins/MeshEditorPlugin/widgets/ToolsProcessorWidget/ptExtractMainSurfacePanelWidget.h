/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptExtractMainSurfacePanelWidget_H
#define ptExtractMainSurfacePanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKExtractMainSurfaceFilter.h"

#define wxID_btnExtractMainSurface wxID_HIGHEST + 1


/**
\brief Pw for extract surface processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptExtractMainSurfacePanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget 
{
public: 
	typedef Core::VTKProcessor<meVTKExtractMainSurfaceFilter> ExtractMainSurfaceProcessor;

	coreDefineBaseWindowFactory( ptExtractMainSurfacePanelWidget )

	//!
    ptExtractMainSurfacePanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);

	virtual void OnButtonExtractMainSurface(wxCommandEvent &event); // wxGlade: <event_handler>

private:
    // begin wxGlade: ptExtractMainSurfacePanelWidget::methods
    void do_layout();
    // end wxGlade
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    
    wxButton* btnExtractMainSurface;
    
    DECLARE_EVENT_TABLE();
private:

	//!
	ExtractMainSurfaceProcessor::Pointer m_processor;
}; // wxGlade: end class

#endif // ptExtractMainSurfacePanelWidget_H
