/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreptShapeScalePanelWidget_H
#define coreptShapeScalePanelWidget_H

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreShapeScaleProcessor.h"
#include "wxID.h"
#include <wx/wx.h>
#include <wx/image.h>

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#define wxID_BTN_SCALE  wxID( "wxID_BTN_SCALE")
// end wxGlade


/**
\brief Scales the shape
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 1 Feb 2010
*/

class PLUGIN_EXPORT ptShapeScalePanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
  
    ptShapeScalePanelWidget(wxWindow* parent, 
		int id = wxID_ANY,
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	Core::BaseProcessor::Pointer GetProcessor();

protected:
	virtual void OnScaleClick(wxCommandEvent &event); // wxGlade: <event_handler>

	//!
	void OnInit();

	//!
	void UpdateWidget();

	//!
	void UpdateData();


private:
    // begin wxGlade: ptShapeScalePanelWidget::methods
    void do_layout();
    // end wxGlade

protected:

	Core::ShapeScaleProcessor::Pointer m_processor;

    // begin wxGlade: ptShapeScalePanelWidget::attributes
    wxStaticBox* m_ShapeScale_staticbox;
    wxStaticText* m_labelScale;
    wxTextCtrl* m_textScale;
    wxButton* m_btnScale;
    // end wxGlade

    DECLARE_EVENT_TABLE();


}; // wxGlade: end class

#endif // ptShapeScalePanelWidget_H
