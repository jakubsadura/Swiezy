/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginToolbar_H
#define _TavernaPluginToolbar_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "coreBaseWindow.h"

namespace TavernaPlugin
{

/**
\brief Toolbar for Cmgui Plugin
\ingroup TavernaPlugin
\author Xavi Planes
\date Nov 2010
*/
class Toolbar: public wxToolBar, public Core::BaseWindow {
public:
	//!
	coreDefineBaseWindowFactory( Toolbar );

	//!
	Toolbar(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

protected:

	//! 
	void OnRefresh( wxCommandEvent& event );

	DECLARE_EVENT_TABLE();
private:

	//!
	std::string m_PluginToolbarPath;
};


} // namespace TavernaPlugin

#endif // _TavernaPluginToolbar_H
