/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarWindows_H
#define _coreToolbarWindows_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"
#include "coreWindowConfig.h"
#include "coreToolbarPluginTab.h"

namespace Core
{
namespace Widgets
{

/**
\brief Open different widgets
\ingroup gmWidgets
\author Xavi Planes
\date 1 April 2010
*/
class GMWIDGETS_EXPORT ToolbarWindows: public ToolbarPluginTab {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarWindows );

	//!
	ToolbarWindows(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

protected:
	//!
	void AddWidgetTool( wxWindow* window );

	DECLARE_EVENT_TABLE();
private:
};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarWindows_H
