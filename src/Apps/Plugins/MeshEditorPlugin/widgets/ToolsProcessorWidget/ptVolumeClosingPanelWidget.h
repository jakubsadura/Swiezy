/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptVolumeClosingPanelWidget_H
#define ptVolumeClosingPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKVolumeClosingFilter.h"
#include "coreCloseHolesProcessor.h"

#define wxID_btnGlobalVolumeClosing wxID_HIGHEST + 1
#define wxID_CHOOSEHOLE wxID("ChooseHole")
#define wxID_COMBOMETHODS wxID("ComboMethods")
#define wxID_CHKCLOSEALL wxID("CloseAll")

/**
\brief Pw for extract surface processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/
class PLUGIN_EXPORT ptVolumeClosingPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:

	//typedef Core::VTKProcessor<meVTKVolumeClosingFilter> VolumeClosingProcessor;

	coreDefineBaseWindowFactory( ptVolumeClosingPanelWidget )

	//!
    ptVolumeClosingPanelWidget(wxWindow* parent, 
							int id = wxID_ANY,	
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

	virtual void OnButtonGlobalVolumeClosing(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnChooseHoles(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnComboboxChoice(wxCommandEvent &event);

	void UpdateData();
	void UpdateWidget();

	void OnModifiedInputDataEntity();
	void OnModifiedOutputDataEntity();
	void OnModifiedSelection();
	void OnInit();

	void ConnectInteractor();
	void DisconnectInteractor();

private:
    void do_layout();
	void set_properties();
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    // begin wxGlade: ptVolumeClosingPanelWidget::attributes
	wxStaticBox* m_GlobalVolumeClosing_staticbox;
	wxStaticBox* sizer_2_staticbox;
	wxCheckBox* checkbox_closeAll;
	wxToggleButton* btnChoosehole;
	wxCheckBox* checkboxPatchOnly;
	wxStaticText* labelmethod;
	wxComboBox* combo_box_1;
	wxStaticText* m_labelGlobalVolumeClosingDensityFactor;
	wxTextCtrl* m_textGlobalVolumeClosingDensityFactor;
	wxStaticText* m_labelGlobalVolumeClosingAngle;
	wxTextCtrl* m_textGlobalVolumeClosingAngle;
	wxStaticText* m_labelGlobalVolumeClosingMinimumEdgesSwapped;
	wxTextCtrl* m_textGlobalVolumeClosingMinimumEdgesSwapped;
	wxStaticText* m_labelGlobalVolumeClosingOrder;
	wxTextCtrl* m_textGlobalVolumeClosingOrder;
	wxButton* btnGlobalVolumeClosing;
    // end wxGlade

	wxStaticBoxSizer* sizer_2;

    DECLARE_EVENT_TABLE();

private:

	//!
	//VolumeClosingProcessor::Pointer m_processor;
	Core::CloseHolesProcessor::Pointer m_processor;
}; // wxGlade: end class


#endif // ptVolumeClosingPanelWidget_H
