/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarWorkingArea_H
#define _coreToolbarWorkingArea_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"
#include "coreWorkingAreaFactory.h"
#include "coreToolbarPluginTab.h"

namespace Core
{
namespace Widgets
{

/**
\brief Toolbar for selecting WorkingArea
\ingroup gmWidgets
\author Xavi Planes
\date 5 Mar 2010
*/
class GMWIDGETS_EXPORT ToolbarWorkingArea: public ToolbarPluginTab {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarWorkingArea );

	//!
	ToolbarWorkingArea(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

	//! Add observer of active working area state
	void Init( );

	//!
	void OnLinkWorkingAreas(wxCommandEvent& event);
	
	//!
	void SetPluginTab(Core::Widgets::PluginTab* val);

protected:
	//!
	void UpdateState();

	//!
	void SelectWorkingArea( wxWindowID type );

	//!
	void AddWidgetTool( wxWindow* window );

	//!
	virtual void AddTool( 
		wxWindowID id, const wxString &label, const wxBitmap &bitmap );

	DECLARE_EVENT_TABLE();
private:

};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarWorkingArea_H
