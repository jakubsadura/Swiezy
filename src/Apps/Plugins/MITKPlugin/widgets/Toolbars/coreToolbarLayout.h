/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarLayout_H
#define _coreToolbarLayout_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"

namespace Core
{
namespace Widgets
{

class MultiRenderWindowMITK;

/**
\brief Toolbar for RenderWindowBase Layout
\ingroup gmWidgets
\author Chiara Riccobene
\date 7 Jan 2010
*/
class PLUGIN_EXPORT ToolbarLayout: public wxToolBar, public BaseWindow {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarLayout );

	//!
	ToolbarLayout(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);


	//!
	void SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );


	//! Layout Group
	void OnChangeLayoutToDefault(wxCommandEvent& event);
	void On2DViewsLeft(wxCommandEvent& event);
	void OnOnly2DViews(wxCommandEvent& event);
	void OnBig3DView(wxCommandEvent& event);
	void OnXView(wxCommandEvent& event);
	void OnYView(wxCommandEvent& event);
	void OnZView(wxCommandEvent& event);

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

#endif // _coreToolbarLayout_H
