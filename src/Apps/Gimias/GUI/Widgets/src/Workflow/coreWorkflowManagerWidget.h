/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkflowManagerWidget_H
#define _coreWorkflowManagerWidget_H

#include "coreWorkflowManagerWidgetUI.h"

namespace Core{
namespace Widgets{

class WorkflowEditorWidget;

/** 
\brief Manager for all available workflows
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class WorkflowManagerWidget : public coreWorkflowManagerWidgetUI {
public:

	//!
    WorkflowManagerWidget(wxWindow* parent, int id);

	//!
	bool Show(bool show = true );

protected:
	DECLARE_EVENT_TABLE( )

    void OnBtnNew(wxCommandEvent &event);
    void OnBtnEdit(wxCommandEvent &event);
	void OnBtnActivate(wxCommandEvent &event);
	void OnBtnDelete(wxCommandEvent &event);
	void OnBtnImport(wxCommandEvent &event);
	void OnItemActivated( wxTreeEvent& event );
	void OnBtnRename(wxCommandEvent &event);
	void OnBeginLabelEdit(wxTreeEvent &event);
	void OnEndLabelEdit(wxTreeEvent &event);
	void OnBtnSaveLayout(wxCommandEvent &event);

	//!
	void UpdateWidget( );

	//!
	std::string GetSelectedWorkflow( );

private:
	//!
	WorkflowEditorWidget* m_WorkflowEditorWidget;
	//! For renaming
	std::string m_OldWorkflowName;
};


} // Widgets
} // Core


#endif // _coreWorkflowManagerWidget_H
