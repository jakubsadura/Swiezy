/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarSelectionTools_H
#define _coreToolbarSelectionTools_H

#include "gmWidgetsWin32Header.h"

#include "coreToolbarRegisteredWindows.h"

namespace Core
{
namespace Widgets
{

/**
\brief Automatically create tools from  registered windows on BaseWindowFactory
\ingroup gmWidgets
\author Xavi Planes
\date 1 April 2010
*/
class GMWIDGETS_EXPORT ToolbarSelectionTools: public ToolbarRegisteredWindows {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarSelectionTools );

	//!
	ToolbarSelectionTools(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr )
		:ToolbarRegisteredWindows( parent, id, pos, size, style, name )
	{
		SetToolsState( WIDGET_TYPE_SELECTION_TOOL );
	};

protected:

private:
};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarSelectionTools_H
