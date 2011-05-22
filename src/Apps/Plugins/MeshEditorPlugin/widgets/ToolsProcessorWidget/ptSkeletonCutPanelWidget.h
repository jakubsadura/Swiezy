/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptSkeletonCutPanelWidget_H
#define ptSkeletonCutPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreSkeletonCutProcessor.h"
#include "coreLandmarkSelectorWidget.h"

#define wxID_btnSkeletonCut wxID_HIGHEST + 1

/**
\brief Pw for sketon cut processor
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 11 Nov 09
*/

class PLUGIN_EXPORT ptSkeletonCutPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget 
{
public:

	coreDefineBaseWindowFactory( ptSkeletonCutPanelWidget )

    ptSkeletonCutPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);

	virtual void OnButtonSkeletonCut(wxCommandEvent &event); // wxGlade: <event_handler>

protected:

	void OnSelectedPoint();
	void OnSelectedSkeleton();
	void OnSelectedInput();
	bool CheckInputs();
	void ConnectInteractor();
	void DisconnectInteractor();
	void OnInit();

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
    // begin wxGlade: ptSkeletonCutPanelWidget::methods
    void do_layout();
    // end wxGlade
	bool Enable( bool enable = true );

protected:
    
    wxButton* btnSkeletonCut;
    
    DECLARE_EVENT_TABLE();
private:

	//!
	Core::SkeletonCutProcessor::Pointer m_processor;
}; // wxGlade: end class


#endif // ptSkeletonCutPanelWidget_H
