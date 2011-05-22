/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginToolbar_H
#define _CmguiPluginToolbar_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "coreBaseWindow.h"

namespace CmguiPlugin
{

/**
\brief Toolbar for Cmgui Plugin
\ingroup CMGUIPlugin
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
	void BuildToolbarPluginPath( );

	//!
	void ScanToolbarPluginPath( );

	//!
	void LoadTool( std::string path );

	//! 
	void OnClickedTool( wxCommandEvent& event );

	DECLARE_EVENT_TABLE();
private:

	//!
	std::string m_PluginToolbarPath;
};

/**
\brief Toolbar item for Cmgui Plugin
\ingroup CMGUIPlugin
\author Xavi Planes
\date Nov 2010
*/
class wxComItem: public wxObject
{
	DECLARE_CLASS(wxComItem)
public:
	// Constructors

	wxComItem( const wxString& filename = wxEmptyString)
	{ m_FileName = filename; }

	//! Filename of the .com file
	std::string m_FileName;
};

} // namespace CmguiPlugin

#endif // _CmguiPluginToolbar_H
