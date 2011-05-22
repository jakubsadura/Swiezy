/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptLoopSubdivisionPanelWidget_H
#define ptLoopSubdivisionPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKLoopSubdivisionRefinerFilter.h"

#define wxID_btnLoopSubdivisionSurface wxID_HIGHEST + 1


/**
\brief Pw for loop subdivision processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptLoopSubdivisionPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:

	typedef Core::VTKProcessor<meVTKLoopSubdivisionRefinerFilter> LoopSubdivisionProcessor;
   
	coreDefineBaseWindowFactory( ptLoopSubdivisionPanelWidget )

	//!
    ptLoopSubdivisionPanelWidget(wxWindow* parent, 
							int id = wxID_ANY,	
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

	virtual void OnButtonLoopSubdivisionSurface(wxCommandEvent &event); // wxGlade: <event_handler>

private:
    void do_layout();
    
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    wxButton* btnLoopSubdivisionSurface;
    
    DECLARE_EVENT_TABLE();

private:

	//!
	LoopSubdivisionProcessor::Pointer m_processor;

}; // wxGlade: end class


#endif // ptLoopSubdivisionPanelWidget_H
