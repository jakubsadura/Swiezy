/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarRegisteredWindows_H
#define _coreToolbarRegisteredWindows_H

#include "gmWidgetsWin32Header.h"

#include "coreToolbarBase.h"

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
class GMWIDGETS_EXPORT ToolbarRegisteredWindows: public ToolbarBase {
public:
	typedef std::map<std::string,wxWindowID> ToolsMapType;
public:

	ToolbarRegisteredWindows(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

	//!
	void SetToolsState(WIDGET_TYPE val) { m_ToolsType = val; }

	//!
	void SetPluginTab(Core::Widgets::PluginTab* val);

	//!
	void OnInit( );

protected:
	//! Update tools when BaseWindowFactory changes
	virtual void UpdateTools();

	//! Show selected tool
	void OnSelectedTool( wxCommandEvent& event );

	//! Add a new widget tool using factory name of Registered Window
	void AddWidgetTool( const std::string &factoryName );

	DECLARE_EVENT_TABLE();
private:

	//!
	WIDGET_TYPE m_ToolsType;

	//! From registered window factory name to tool ID
	ToolsMapType m_ToolWidgetMap;
};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarRegisteredWindows_H
