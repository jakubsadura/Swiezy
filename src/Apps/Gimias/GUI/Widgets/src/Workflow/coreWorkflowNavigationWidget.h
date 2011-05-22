/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflowNavigationWidget_H
#define coreWorkflowNavigationWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreWorkflowNavigationWidgetUI.h"
#include "coreWorkflow.h"
#include "coreProcessingWidget.h"

namespace Core
{
namespace Widgets
{

/** 
\brief Widget for Workflow navigation

\ingroup gmWidgets
\author Xavi Planes
\date 28 May 2010
*/
class GMWIDGETS_EXPORT WorkflowNavigationWidget 
	: public coreWorkflowNavigationWidgetUI, 
	public BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( WorkflowNavigationWidget );
	//!
	WorkflowNavigationWidget(
		wxWindow* parent, 
		int id, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0 );
	//!
	~WorkflowNavigationWidget(void);

	//!
	Core::BaseProcessor::Pointer GetProcessor(void);

	//!
	void SetWorkflowStep( Core::WorkflowStep::Pointer workflowStep );

private:

	//!
	virtual void OnBack(wxCommandEvent &event);
	//!
	virtual void OnAdd(wxCommandEvent &event);
	//!
	virtual void OnNext(wxCommandEvent &event);
	//! A context menu item has been clicked
	void OnConextMenu( wxCommandEvent& event );
	//!
	void UpdateWidget( );
	//! Check if the current m_ProcessingWidget matches current step of workflow
	std::string CheckCurrentClassname( );
	//!
	void ReplaceWidget( const std::string &factoryName );

	DECLARE_EVENT_TABLE()

private:
	//!
	int m_CurrentStep;

	//! Name of alternative step
	std::string m_AlternativeStep;

	//!
	Core::WorkflowStep::Pointer m_WorkflowStep;

	//!
	wxWindow* m_ProcessingWidget;

	//!
	wxMenu* m_ContextMenu;
};

} // namespace Widgets
} // namespace Core

#endif // coreWorkflowNavigationWidget_H
