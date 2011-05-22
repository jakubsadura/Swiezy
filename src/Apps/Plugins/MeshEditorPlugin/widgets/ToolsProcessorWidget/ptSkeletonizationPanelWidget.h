/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptSkeletonizationPanelWidget_H
#define ptSkeletonizationPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreVTKProcessor.h"
#include "meVTKSkeletonizationFilter.h"

#define wxID_btnSkeletonization wxID_HIGHEST + 1


/**
\brief Pw for skeleton processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptSkeletonizationPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
    
	typedef Core::VTKProcessor<meVTKSkeletonizationFilter> SkeletonizationProcessor;

	coreDefineBaseWindowFactory( ptSkeletonizationPanelWidget )

    ptSkeletonizationPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);
	
	virtual void OnButtonSkeletonization(wxCommandEvent &event); // wxGlade: <event_handler>
	//!
	void UpdateData();
	void UpdateWidget();
	
private:
    void do_layout();
    
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
    // begin wxGlade: ptSkeletonizationPanelWidget::attributes
    wxStaticBox* m_GenerateSkeleton_staticbox;
    wxStaticText* m_labelPrecision;
    wxTextCtrl* m_textPrecision;
    wxButton* btnSkeletonization;
    // end wxGlade

    DECLARE_EVENT_TABLE();

    
private:

	//!
	SkeletonizationProcessor::Pointer m_processor;
	
}; // wxGlade: end class


#endif // ptSkeletonizationPanelWidget_H
