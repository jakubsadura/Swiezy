/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptSignalTimePropagationPanelWidget_H
#define ptSignalTimePropagationPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreSignalTimePropagationProcessor.h"
#include "ptSignalTimePropagationPanelWidgetUI.h"


/**
\brief Pw for extract surface processor
\ingroup SignalViewerPlugin
\author Chiara Riccobene
\date 5 Nov 09
*/

class PLUGIN_EXPORT ptSignalTimePropagationPanelWidget: 
	public ptSignalTimePropagationPanelWidgetUI,
	public Core::Widgets::ProcessingWidget 
{
public:

	coreDefineBaseWindowFactory( ptSignalTimePropagationPanelWidget )

	//!
    ptSignalTimePropagationPanelWidget(wxWindow* parent, 
								int id = wxID_ANY, 
								const wxPoint& pos=wxDefaultPosition, 
								const wxSize& size=wxDefaultSize, 
								long style=0);

	//!
	void OnButtonSignalTimePropagation(wxCommandEvent &event);

	//!
	void OnMarkersPerScan(wxCommandEvent &event);

private:
    //! Enable the widget
	bool Enable( bool enable = true );
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void UpdateData( );

	//!
	void UpdateWidget( );

protected:

private:

	//!
	Core::SignalTimePropagationProcessor::Pointer m_processor;
};


#endif // ptSignalTimePropagationPanelWidget_H
