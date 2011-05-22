/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreTreeWorkflow_H
#define _coreTreeWorkflow_H

#include "coreWorkflow.h"
#include "coreBaseTreeWorkflow.h"

namespace Core
{

namespace Widgets
{

/** 
\brief Specific wxTreeCtrl for Workflow contents
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class TreeWorkflow : public BaseTreeWorkflow
{
public:

	//!
	TreeWorkflow(wxWindow*  parent, wxWindowID id, 
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = wxTR_HAS_BUTTONS, 
		const wxValidator&  validator = wxDefaultValidator, 
		const wxString&  name = "treeCtrl");

	//! Update tree with the workflow information
	void SetWorkflow( Core::Workflow::Pointer workflow );

	//! Add a list of items checking in the BaseWindowFactory the type
	void AddItems( std::list<std::string> &items );

	//! Delete items
	void DeleteItems( wxArrayTreeItemIds items );

	//! Restrict selection of DnD to childs of root only
	bool GetRestrictedSelection() const;
	void SetRestrictedSelection(bool val);

	//! Update internal workflow with tree information
	void UpdateWorkflow( );

	//! Move a set of items to a destItem (only childs of root)
	void MoveItems( wxArrayTreeItemIds srcItems, wxArrayTreeItemIds destItems );

	//!
	bool IsProcessingStep( wxTreeItemId item );

	//!
	bool IsChildOfRoot( wxTreeItemId item );

private:

	DECLARE_EVENT_TABLE();

	//! Search the workflow step item accessing the parent items
	wxTreeItemId GetWorkflowStepItem( wxTreeItemId itemId );

	//!
	wxTreeItemId GetChildItem(  const wxTreeItemId &stepItemId, const std::string factoryName );

	//!
	wxTreeItemId GetSubStepsItem( const wxTreeItemId &stepItemId );

	//!
	wxTreeItemId GetWindowsItem( const wxTreeItemId &stepItemId );

	//!
	void OnRightClick( wxTreeEvent& event );

	//!
	void OnBeginLabelEdit( wxTreeEvent& event );

	//! Add a step to the workflow
	void OnMenuAddStep(wxCommandEvent& event );

	//!
	void OnKeyPressEvent(wxKeyEvent& event);

	//!
	void AddPluginName( 
		const std::string &factoryName, 
		Workflow::PluginNamesListType &pluginNamesList );

	//!
	void UpdatePluginNamesList( );

	//! Set colour to red if widget is not registered
	wxTreeItemId  AppendWidget( 
		const wxTreeItemId&  parent, 
		const wxString&  factoryName,
		int image,
		bool last = true );

	//! Copy all children of src to dest. Recurse if desired.
	void CopyChildren( wxTreeItemId dest, wxTreeItemId src, bool recursively = true );

	//! Copy all data of a item
	void CopyItem( wxTreeItemId dest, wxTreeItemId src );

	//! If there's any working area, add the manager first
	void CheckWorkingAreaManager( wxTreeItemId item );

protected:

	//! Only select childs of root
	bool m_RestrictedSelection;

	//!
	Core::Workflow::Pointer m_Workflow;
};

} // namespace Core
} // namespace Widgets

#endif // _coreTreeWorkflow_H
