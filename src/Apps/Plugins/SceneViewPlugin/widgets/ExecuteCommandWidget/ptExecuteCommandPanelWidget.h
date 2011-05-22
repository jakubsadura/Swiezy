/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ptExecuteCommandPanelWidget_H
#define ptExecuteCommandPanelWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "corePluginMacros.h"
#include "coreProcessingWidget.h"
#include "coreExecuteCommandProcessor.h"

/**
Execute a Commandline with specific options.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 18 nov 2009
*/

class PLUGIN_EXPORT ptExecuteCommandPanelWidget: 
	public wxPanel,
	public Core::Widgets::ProcessingWidget {
public:
 
	coreDefineBaseWindowFactory( ptExecuteCommandPanelWidget )

    ptExecuteCommandPanelWidget(wxWindow* parent, 
							int id = wxID_ANY, 
							const wxPoint& pos=wxDefaultPosition, 
							const wxSize& size=wxDefaultSize, 
							long style=0);

private:
  
    void do_layout();
    // end wxGlade
    //! Enable the widget
	bool Enable( bool enable = true );

public:
	virtual void OnExecuteButton(wxCommandEvent &event); // wxGlade: <event_handler>

	Core::BaseProcessor::Pointer GetProcessor();

	void UpdateData();
	
	void UpdateWidget();

protected:
    // begin wxGlade: ptExecuteCommandPanelWidget::attributes
    wxStaticBox* m_ExecuteCommand_staticbox;
	 wxStaticText* label_0;
	 wxTextCtrl* text_ctrl_0;
	 wxStaticText* label_1;
    wxTextCtrl* text_ctrl_1;
    wxStaticText* label_2;
    wxTextCtrl* text_ctrl_2;
    wxButton* button_1;
    // end wxGlade

    DECLARE_EVENT_TABLE();

protected:
	
	Core::ExecuteCommandProcessor::Pointer m_processor;

}; // wxGlade: end class

#endif // ptExecuteCommandPanelWidget_H
