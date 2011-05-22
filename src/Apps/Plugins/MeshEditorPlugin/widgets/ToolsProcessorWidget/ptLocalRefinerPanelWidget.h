/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptLocalRefinerPanelWidget_H
#define ptLocalRefinerPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKLocalRefinerFilter.h"

#define wxID_btnLocalRefiner wxID_HIGHEST + 1


/**
\brief Pw for local refiner processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptLocalRefinerPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
    // begin wxGlade: ptLocalRefinerPanelWidget::ids
    // end wxGlade

	typedef Core::VTKProcessor<meVTKLocalRefinerFilter> LocalRefinerProcessor;

	coreDefineBaseWindowFactory( ptLocalRefinerPanelWidget )

    ptLocalRefinerPanelWidget(wxWindow* parent, 
							int id = wxID_ANY, 
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

	virtual void OnButtonLocalRefiner(wxCommandEvent &event); // wxGlade: <event_handler>

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
    // begin wxGlade: ptLocalRefinerPanelWidget::attributes
    wxStaticBox* m_LocalRefiner_staticbox;
    wxStaticText* m_labelLocalRefinerDensityFactor;
    wxTextCtrl* m_textLocalRefinerDensityFactor;
    wxStaticText* m_labelLocalRefinerAngle;
    wxTextCtrl* m_textLocalRefinerAngle;
    wxStaticText* m_labelLocalRefinerMinimumEdgesSwapped;
    wxTextCtrl* m_textLocalRefinerMinimumEdgesSwapped;
    wxButton* btnLocalRefiner;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	LocalRefinerProcessor::Pointer m_processor;
}; // wxGlade: end class

#endif // ptLocalRefinerPanelWidget_H
