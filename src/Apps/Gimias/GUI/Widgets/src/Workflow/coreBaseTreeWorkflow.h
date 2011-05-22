/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseTreeWorkflow_H
#define _coreBaseTreeWorkflow_H

#include "processing.xpm"
#include "folder.xpm"
#include "window.xpm"
#include "tab.xpm"
#include "workflow.xpm"

namespace Core
{

namespace Widgets
{

const int IMG_WORKFLOW = 0;
const int IMG_TAB = 1;
const int IMG_PROCESSING = 2;
const int IMG_FOLDER = 3;
const int IMG_WINDOW = 4;


/** 
\brief base tree for workflow
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class BaseTreeWorkflow : public wxTreeCtrl
{
public:
	//!
	BaseTreeWorkflow(wxWindow*  parent, wxWindowID id, 
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = wxTR_HAS_BUTTONS, 
		const wxValidator&  validator = wxDefaultValidator, 
		const wxString&  name = "treeCtrl");

	//! 
	std::string GetItemFactoryName( const wxTreeItemId &itemId );

	//!
	wxTreeItemId AppendItem(
		const wxTreeItemId& parent,
		const wxString& text,
		int image = -1 );

	//!
	wxTreeItemId AppendItem( 
		const wxTreeItemId& parent, const wxString& text, int image, const wxString& factoryName );

	//!
	wxTreeItemId InsertItem(const wxTreeItemId& parent, size_t index, const wxString& text, int image, const wxString& classname );

};


} // namespace Core
} // namespace Widgets

#endif // _coreBaseTreeWorkflow_H
