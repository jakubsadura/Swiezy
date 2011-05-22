/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef ptNGOptimizationPanelWidget_H
#define ptNGOptimizationPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreNetgenProcessor.h"


#define wxID_btnNetgenGlobalOptimization wxID_HIGHEST + 3
#define wxID_btnNetgenGlobalSmoothing wxID_HIGHEST + 2
#define wxID_btnNetgenGlobalRefinement wxID_HIGHEST + 1



/**
\brief Pw for netgen processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptNGOptimizationPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
   
	coreDefineBaseWindowFactory( ptNGOptimizationPanelWidget )

    ptNGOptimizationPanelWidget(wxWindow* parent, 
							int id = wxID_ANY, 
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

	virtual void OnButtonNetgenGlobalRefinement(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnButtonNetgenGlobalSmoothing(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnButtonNetgenGlobalOptimization(wxCommandEvent &event); // wxGlade: <event_handler>

	void UpdateData();
	void UpdateWidget();

private:
    void do_layout();
    
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    // begin wxGlade: ptNGOptimizationPanelWidget::attributes
    wxStaticBox* m_GlobalOptimization_staticbox;
    wxStaticText* m_labelMaxMeshSize;
    wxTextCtrl* m_textMaxMeshSize;
    wxStaticText* m_labelElementsPerCurvatureRadius;
    wxTextCtrl* m_textElementsPerCurvatureRadius;
    wxStaticText* m_labelElementsPerEdge;
    wxTextCtrl* m_textElementsPerEdge;
    wxButton* btnNetgenGlobalRefinement;
    wxButton* btnNetgenGlobalSmoothing;
    wxButton* btnNetgenGlobalOptimization;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	Core::NetgenProcessor::Pointer m_processor;

}; // wxGlade: end class


#endif // ptNGOptimizationPanelWidget_H
