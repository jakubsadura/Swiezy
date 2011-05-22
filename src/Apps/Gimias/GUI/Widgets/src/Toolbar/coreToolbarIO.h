/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarIO_H
#define _coreToolbarIO_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"

namespace Core
{
namespace Widgets
{

/**
\brief Toolbar for IO
\ingroup gmWidgets
\author Xavi Planes
\date 10 June 2010
*/
class GMWIDGETS_EXPORT ToolbarIO: public wxToolBar, public BaseWindow {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarIO );

	//!
	ToolbarIO(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );



protected:
	//!
	void UpdateState();
	

	DECLARE_EVENT_TABLE();
private:

};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarIO_H
