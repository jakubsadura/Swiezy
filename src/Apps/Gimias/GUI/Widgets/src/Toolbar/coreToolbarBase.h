/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarBase_H
#define _coreToolbarBase_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"
#include "coreWindowConfig.h"

namespace Core
{
namespace Widgets
{

/**
\brief Automatically create tools from BaseWindow bitmaps and update
shown/hidden state
\ingroup gmWidgets
\author Xavi Planes
\date 1 April 2010
*/
class GMWIDGETS_EXPORT ToolbarBase: public wxToolBar, public BaseWindow {
public:

	ToolbarBase(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);


	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:

	DECLARE_EVENT_TABLE();
private:
};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarBase_H
