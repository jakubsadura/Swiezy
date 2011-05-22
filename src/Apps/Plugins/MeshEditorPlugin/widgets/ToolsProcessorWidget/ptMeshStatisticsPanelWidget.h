/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreptMeshStatisticsPanelWidget_H
#define coreptMeshStatisticsPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreMeshStatisticsProcessor.h"

// begin wxGlade: ::extracode
#define wxID_COMPUTE wxID_HIGHEST + 1
#define wxID_CHK_ASPECTRATIO wxID_HIGHEST + 2
#define wxID_CHK_EDGERATIO wxID_HIGHEST + 3
#define wxID_CHK_MINANGLE wxID_HIGHEST + 4

// end wxGlade

/**
\brief Pw to compute tetra statistics
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 13 Jan 10
*/
class PLUGIN_EXPORT ptMeshStatisticsPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget 
{
public:
    // begin wxGlade: ptMeshStatisticsPanelWidget::ids
    // end wxGlade

	coreDefineBaseWindowFactory( ptMeshStatisticsPanelWidget )

    ptMeshStatisticsPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);

private:
    // begin wxGlade: ptMeshStatisticsPanelWidget::methods
    void do_layout();
    // end wxGlade
	
	//! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );
	
	//!
	void UpdateData();
	void UpdateWidget();
	void OnInit();

protected:
    // begin wxGlade: ptMeshStatisticsPanelWidget::attributes
    wxStaticBox* sizer1_staticbox;
    wxCheckBox* ckb_AspectRatio;
    wxTextCtrl* text_ctrl_1;
    wxCheckBox* ckb_MinAngle;
    wxTextCtrl* text_ctrl_1_copy;
    wxCheckBox* ckb_EgdeRatio;
    wxTextCtrl* text_ctrl_1_copy_1;
    wxStaticText* m_labelNumberofBins;
    wxTextCtrl* m_textNumberofBins;
    wxButton* m_btnCompute;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnAspectRatioChecked(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnEdgeRatioChecked(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnMinAngleChecked(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnComputeBtn(wxCommandEvent &event); // wxGlade: <event_handler>
private:
	//!
	Core::MeshStatisticsProcessor::Pointer m_processor;
}; // wxGlade: end class


#endif // ptMeshStatisticsPanelWidget_H
