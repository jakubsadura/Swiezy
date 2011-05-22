/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarMultiRenderWindow_H
#define _coreToolbarMultiRenderWindow_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"

namespace Core
{
namespace Widgets
{

#define wxID_FitScene				wxID("wxID_FitScene")
#define wxID_InitAxis				wxID("wxID_InitAxis")
#define wxID_HideXYZPlanes			wxID("wxID_HideXYZPlanes")

class MultiRenderWindowMITK;

/**
\brief Toolbar for RenderWindowBase
\ingroup gmWidgets
\author Chiara Riccobene
\date 7 Jan 2010
*/
class PLUGIN_EXPORT ToolbarMultiRenderWindow: public wxToolBar, public BaseWindow {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarMultiRenderWindow );

	//!
	ToolbarMultiRenderWindow(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);


	//!
	void SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );


	//! Axis and Planes
	void OnEnableAxis(wxCommandEvent& event);
	void OnFitScene(wxCommandEvent& event);
	void OnInitAxis(wxCommandEvent& event);
	//! Show/Hide the X, Y and Z plane cuts
	void OnHideXYZPlanes( wxCommandEvent& event );
	
protected:
	//!
	void UpdateState();
	
	//!
	MultiRenderWindowMITK* GetMultiRenderWindowMITK();

	DECLARE_EVENT_TABLE();
private:

};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarMultiRenderWindow_H
