/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginWorkflowPanelWidget_H
#define _TavernaPluginWorkflowPanelWidget_H

#include "TavernaPluginWorkflowPanelWidgetUI.h"

// CoreLib
#include "coreRenderingTree.h"
#include "coreProcessingWidget.h"
#include "coreDynProcessor.h"
#include "wflWorkflow.h"

namespace Core{ namespace Widgets {
	class UserHelper;
	class DataEntityListBrowser;
}}
	
namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class WorkflowPanelWidget : 
	public TavernaPluginWorkflowPanelWidgetUI,
	public Core::Widgets::ProcessingWidget 
{

// OPERATIONS
public:
	//!
	WorkflowPanelWidget(
		wflWorkflow::Pointer workflow, 
		wxWindow* parent, int id= wxID_ANY,
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = 0);

	//!
	~WorkflowPanelWidget( );

	//! Add button events to the bridge and call UpdateWidget()
	void OnInit(  );
	
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	//! Update GUI from working data
	void UpdateWidget();

	//! Update working data from GUI
	void UpdateData();

	//! Button has been pressed
	void OnBtnApply(wxCommandEvent& event);

// ATTRIBUTES
private:
	//! Working data of the processor
	Core::DynProcessor::Pointer m_Processor;
	
	//!
	wflWorkflow::Pointer m_Workflow;

	//!
	ModuleDescription m_Module;

};

} //namespace TavernaPlugin{

#endif //_TavernaPluginWorkflowPanelWidget_H
