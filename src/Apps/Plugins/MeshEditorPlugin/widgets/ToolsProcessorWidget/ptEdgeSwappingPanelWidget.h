/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptEdgeSwappingPanelWidget_H
#define ptEdgeSwappingPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "meVTKEdgeSwappingFilter.h"
#include "coreVTKProcessor.h"

#define wxID_btnEdgeSwapping wxID_HIGHEST + 1


/**
\brief Pw for edge swapping processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptEdgeSwappingPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {

public:
	typedef Core::VTKProcessor<meVTKEdgeSwappingFilter> EdgeSwappingProcessor;

public:
    
	coreDefineBaseWindowFactory( ptEdgeSwappingPanelWidget )

	//!
    ptEdgeSwappingPanelWidget(wxWindow* parent, 
							int id = wxID_ANY, 
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

	virtual void OnButtonEdgeSwapping(wxCommandEvent &event); // wxGlade: <event_handler>
	//!
	void UpdateData();
	//!
	void UpdateWidget();

private:
    void do_layout();
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    // begin wxGlade: ptEdgeSwappingPanelWidget::attributes
    wxStaticBox* m_EdgeSwapping_staticbox;
    wxStaticText* m_labelEdgeSwappingAngle;
    wxTextCtrl* m_textEdgeSwappingAngle;
    wxStaticText* m_labelEdgeSwapingMinimumEdgesSwapped;
    wxTextCtrl* m_textEdgeSwapingMinimumEdgesSwapped;
    wxButton* btnEdgeSwapping;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	EdgeSwappingProcessor::Pointer m_processor;
}; // wxGlade: end class


#endif // ptEdgeSwappingPanelWidget_H
