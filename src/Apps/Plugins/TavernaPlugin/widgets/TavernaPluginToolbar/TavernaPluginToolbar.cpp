/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginToolbar.h"
#include "TavernaPluginWorkflowPanelWidgetFactory.h"
#include "TavernaRefresh.xpm"

#define wxID_Refresh wxID( "wxID_Refresh" )

BEGIN_EVENT_TABLE(TavernaPlugin::Toolbar, wxToolBar)
  EVT_TOOL(wxID_Refresh, TavernaPlugin::Toolbar::OnRefresh)
END_EVENT_TABLE()

TavernaPlugin::Toolbar::Toolbar(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	AddTool( wxID_Refresh, _T( "Refresh Taverna workflows" ),
		wxBitmap( tavernarefresh_xpm ), wxNullBitmap, wxITEM_NORMAL );

	Realize();

}


void TavernaPlugin::Toolbar::OnRefresh( wxCommandEvent& event )
{
	// Scan workflows folder
	WorkflowPanelWidgetFactory::Scan();
}
