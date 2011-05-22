/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarWorkingArea.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMultiRenderWindow.h"
#include "corePluginTab.h"
#include "coreDataEntityListBrowser.h"
#include "coreRenderWindowContainer.h"

//#include ".xpm"
#include "Link.xpm"
#include "UnLink.xpm"

#define wxID_LINK_WORKING_AREAS wxID("wxID_LINK_WORKING_AREAS")

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarWorkingArea, Core::Widgets::ToolbarPluginTab)
  EVT_TOOL(wxID_LINK_WORKING_AREAS, Core::Widgets::ToolbarWorkingArea::OnLinkWorkingAreas)
END_EVENT_TABLE()


Core::Widgets::ToolbarWorkingArea::ToolbarWorkingArea(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
		ToolbarPluginTab(parent, id, pos, size, style, name)
{
	wxBitmap bitmap;

	bitmap = wxBitmap( link_xpm );
	AddCheckTool(wxID_LINK_WORKING_AREAS, _T("Link/Unlink render windows"),
		bitmap, wxNullBitmap, 
		_T("Link/Unlink render windows"), _T("Link/Unlink render windows"));

	AddSeparator();

	Realize();

}

void Core::Widgets::ToolbarWorkingArea::OnLinkWorkingAreas( wxCommandEvent& event )
{
	RenderWindowContainer* window;
	window = dynamic_cast<RenderWindowContainer*> ( GetPluginTab()->GetCurrentWorkingArea() );
	if ( window )
	{
		window->LinkWindows( GetToolState( wxID_LINK_WORKING_AREAS ) );
	}

	UpdateState( );
}

void Core::Widgets::ToolbarWorkingArea::Init()
{
	if ( !GetPluginTab()->GetWorkingAreaManager() )
	{
		return;
	}

	IntHolderType::Pointer workingAreaHolder;
	workingAreaHolder = GetPluginTab()->GetWorkingAreaManager()->GetActiveWorkingAreaHolder();
	workingAreaHolder->AddObserver( 
		this,
		&ToolbarWorkingArea::UpdateState );
}

void Core::Widgets::ToolbarWorkingArea::UpdateState()
{
	ToolbarPluginTab::UpdateState();

	RenderWindowContainer* window;
	window = dynamic_cast<RenderWindowContainer*> ( GetPluginTab()->GetCurrentWorkingArea() );
	if ( window )
	{
		EnableTool( wxID_LINK_WORKING_AREAS, true );

		if ( window->GetStateHolder()->GetSubject().m_LinkedWindows )
		{
			ToggleTool( wxID_LINK_WORKING_AREAS, true );
			//SetToolNormalBitmap( wxID_LINK_WORKING_AREAS, wxBitmap( unlink_xpm ) );
		}
		else
		{
			ToggleTool( wxID_LINK_WORKING_AREAS, false );
			//SetToolNormalBitmap( wxID_LINK_WORKING_AREAS, wxBitmap( link_xpm ) );
		}

	}
	else
	{
		EnableTool( wxID_LINK_WORKING_AREAS, false );
	}
}


void Core::Widgets::ToolbarWorkingArea::AddWidgetTool( wxWindow* window )
{
	BaseWindow* baseWindow = GetPluginTab()->GetWorkingArea( window->GetId() );
	if ( !baseWindow )
	{
		return;
	}

	RenderWindowContainer* container;
	container = dynamic_cast<RenderWindowContainer*> ( baseWindow );
	if ( container && !container->CheckValidProperties( ) )
	{
		return;
	}

	ToolbarPluginTab::AddWidgetTool( window );
}


void Core::Widgets::ToolbarWorkingArea::AddTool( 
	wxWindowID id, 
	const wxString &label, 
	const wxBitmap &bitmap )
{
	wxToolBar::AddTool(id, 
		label,
		bitmap, wxNullBitmap, wxITEM_RADIO,
		label, 
		label );
}

void Core::Widgets::ToolbarWorkingArea::SetPluginTab( Core::Widgets::PluginTab* val )
{
	ToolbarPluginTab::SetPluginTab( val );
	Init( );
	UpdateState();
}