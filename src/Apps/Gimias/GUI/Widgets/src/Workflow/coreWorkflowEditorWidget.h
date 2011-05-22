/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkflowEditorWidget_H
#define _coreWorkflowEditorWidget_H

#include "coreWorkflowEditorWidgetUI.h"
#include "coreWorkflow.h"

namespace Core
{

namespace Widgets
{

/** 
\brief Widget to edit a Workflow
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class WorkflowEditorWidget: public coreWorkflowEditorWidgetUI
{
public:
	//!
	WorkflowEditorWidget(wxWindow* parent, int id);

	//!
	void SetWorkflow( Core::Workflow::Pointer workflow );

	//!
	bool Show(bool show = true );

private:

	DECLARE_EVENT_TABLE();

	//!
	void OnBeginDragTreeRegisteredWindows(wxTreeEvent& event);

	//!
	void OnBeginDragTreeWorkflow(wxTreeEvent& event);

	//!
	void UpdateRegisteredWindows( );

	//!
	void OnOk(wxCommandEvent &event);

	//!
	void OnCancel(wxCommandEvent& event);

protected:

};

} // namespace Core
} // namespace Widgets

#endif // _coreWorkflowEditorWidget_H
