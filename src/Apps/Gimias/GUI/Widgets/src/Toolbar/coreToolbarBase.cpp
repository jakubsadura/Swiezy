/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarBase.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarBase, wxToolBar)
END_EVENT_TABLE()


Core::Widgets::ToolbarBase::ToolbarBase(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	SetName( "Toolbar Base" );
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarBase::GetProcessor()
{
	return NULL;
}
