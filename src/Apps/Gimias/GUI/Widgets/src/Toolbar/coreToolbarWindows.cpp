/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarWindows.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarWindows, Core::Widgets::ToolbarPluginTab)
END_EVENT_TABLE()


Core::Widgets::ToolbarWindows::ToolbarWindows(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    ToolbarPluginTab(parent, id, pos, size, style, name)
{
}

void Core::Widgets::ToolbarWindows::AddWidgetTool( wxWindow* window )
{
	if ( GetPluginTab()->GetWorkingArea( window->GetId() ) )
	{
		return;
	}

	ToolbarPluginTab::AddWidgetTool( window );
}

