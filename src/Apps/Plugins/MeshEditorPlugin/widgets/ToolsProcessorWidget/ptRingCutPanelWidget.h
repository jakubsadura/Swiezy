/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptRingCutPanelWidget_H
#define ptRingCutPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreRingCutProcessor.h"
#include "coreLandmarkSelectorWidget.h"

#define wxID_btnRingCut wxID_HIGHEST + 1
#define wxID_btnEnable wxID_HIGHEST + 2
#define wxID_btnChooseSurface wxID_HIGHEST + 3
#define wxID_cmbSelectColor wxID_HIGHEST + 4

/**
\brief Pw for ring cut processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 6 Nov 09
*/

class PLUGIN_EXPORT ptRingCutPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget
{
public:

	coreDefineBaseWindowFactory( ptRingCutPanelWidget )

    ptRingCutPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);
								
	~ptRingCutPanelWidget();

	virtual void OnButtonRingCut(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnButtonEnableInteraction(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnButtonChooseSurfaces(wxCommandEvent &event); // wxGlade: <event_handler>

protected:

	void DoCancel();
	void DoEnableNewCut();
	void OnSelectedPoint();
	void OnSelectedInput();
	void ConnectInteractor();
	void DisconnectInteractor();
	bool CheckInputs();
	void OnInit();
	void UpdateWidget();

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
    // begin wxGlade: ptRingCutPanelWidget::methods
    void do_layout();
    // end wxGlade
	bool Enable( bool enable = true );
	//!
	double *ChangeNodeColor (mitk::DataTreeNode::Pointer node, int colorIndex);
protected:
    
    wxButton* btnRingCut;
	wxButton* btnChooseSurface;
	wxComboBox* cmbSelectColor;
	wxToggleButton* toggleBtnEnable;
    
    DECLARE_EVENT_TABLE();
private:

	//!
	Core::RingCutProcessor::Pointer m_processor;
	//!
	bool m_interactionEnabled;
	//!
	double *m_oldSurfaceColor;
}; // wxGlade: end class

#endif // ptRingCutPanelWidget_H
