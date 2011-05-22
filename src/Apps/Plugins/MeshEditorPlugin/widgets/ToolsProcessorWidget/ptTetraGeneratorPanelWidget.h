/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptTetraGeneratorPanelWidget_H
#define ptTetraGeneratorPanelWidget_H


#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreTetraGenerationProcessor.h"

#define wxID_btnTetraGenerator wxID_HIGHEST + 1
#define wxID_RADIOBOX_TETRA wxID_HIGHEST + 2
// end wxGlade


/**
\brief Pw for tetra generation processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/


class PLUGIN_EXPORT ptTetraGeneratorPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
    
	coreDefineBaseWindowFactory( ptTetraGeneratorPanelWidget )

    ptTetraGeneratorPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);

	virtual void OnButtonTetraGenerator(wxCommandEvent &event); // wxGlade: <event_handler>
	 virtual void OnTypeTetra(wxCommandEvent &event); // wxGlade: <event_handler>

	//!
	void UpdateData();
	//!
	void UpdateWidget();
	//!
	void UpdateLayout();

private:

	void do_layout();
	void set_properties();

    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );


protected:
    // begin wxGlade: ptTetraGeneratorPanelWidget::attributes
    wxStaticBox* m_TetraGenerator_staticbox;
	wxRadioBox* m_typeRadiobox;
	wxStaticBoxSizer* m_TetraGenerator ;

    wxStaticBox* m_Tetgen_staticbox;
	wxStaticText* m_labelTetraGeneratorMaxVolume;
    wxTextCtrl* m_textTetraGeneratorMaxVolume;
    wxStaticText* m_labelTetraGeneratorAspectRatio;
    wxTextCtrl* m_textTetraGeneratorAspectRatio;
	wxStaticBoxSizer* m_Tetgen;

	wxStaticBox* m_GlobalOptimization_staticbox;
	wxStaticText* m_labelMaxMeshSize;
	wxTextCtrl* m_textMaxMeshSize;
	wxStaticText* m_labelElementsPerCurvatureRadius;
	wxTextCtrl* m_textElementsPerCurvatureRadius;
	wxStaticText* m_labelElementsPerEdge;
	wxTextCtrl* m_textElementsPerEdge;
	wxStaticBoxSizer* m_GlobalOptimization;

    wxButton* btnTetraGenerator;
    // end wxGlade

    DECLARE_EVENT_TABLE();

private:

	//!
	Core::TetraGenerationProcessor::Pointer m_processor;

}; // wxGlade: end class


#endif // ptTetraGeneratorPanelWidget_H
