/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreTreeRegisteredWindows_H
#define _coreTreeRegisteredWindows_H

#include "coreWorkflow.h"
#include "coreBaseTreeWorkflow.h"

namespace Core
{

namespace Widgets
{

/** 
\brief Specific wxTreeCtrl for Registered Windows
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class TreeRegisteredWindows : public BaseTreeWorkflow
{
public:

	//!
	TreeRegisteredWindows(wxWindow*  parent, wxWindowID id, 
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = wxTR_HAS_BUTTONS, 
		const wxValidator&  validator = wxDefaultValidator, 
		const wxString&  name = "treeCtrl");

	//!
	void UpdateRegisteredWindows( );

private:

	DECLARE_EVENT_TABLE();

	//!
	wxTreeItemId AddPluginItem( std::string name );

protected:

};

} // namespace Core
} // namespace Widgets

#endif // _coreTreeRegisteredWindows_H
