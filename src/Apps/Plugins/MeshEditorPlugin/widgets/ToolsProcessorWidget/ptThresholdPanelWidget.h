/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _ptThresholdPanelWidget_H
#define _ptThresholdPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "coreProcessingWidget.h"
#include "coreInputControl.h"
#include "coreDataEntityListBrowser.h"
#include "corePluginMacros.h"

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreThresholdProcessor.h"

#include "wxMitkScalarsArrayControl.h"

#define wxID_MESH_THRESHOLD_BTN_APPLY wxID_HIGHEST + 1

#define wxID_ptThresholdPanelWidget wxID("wxID_ptThresholdPanelWidget")

/**
\brief Pw for threshold processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptThresholdPanelWidget : 
	public wxPanel,
	public Core::Widgets::ProcessingWidget
{
public:
    
	coreDefineBaseWindowFactory( ptThresholdPanelWidget )

	//!
    ptThresholdPanelWidget(wxWindow* parent = NULL, 
						int id = wxID_ptThresholdPanelWidget, 
						const wxPoint& pos=wxDefaultPosition, 
						const wxSize& size=wxDefaultSize, 
						long style=0);

	virtual void OnButtonThreshold(wxCommandEvent &event); // wxGlade: <event_handler>

	void UpdateData();
	void UpdateWidget();


private:
    void do_layout();

    //! Enable the widget
	bool Enable( bool enable = true );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void OnModifiedInput( );

protected:
    // begin wxGlade: ptThresholdPanelWidget::attributes
	wxStaticBox* m_Threshold_staticbox;
	wxStaticText* m_labelLowerThreshold;
    wxTextCtrl* m_LowerThreshold;
    wxStaticText* m_llabelUpperThreshold;
    wxTextCtrl* m_UpperThreshold;
    wxButton* m_btnThreshold;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	Core::ThresholdProcessor::Pointer m_processor;
	//!
	mitk::wxMitkScalarsArrayControl* m_ScalarArrayControl;

}; // wxGlade: end class


#endif // ptThresholdPanelWidget_H
