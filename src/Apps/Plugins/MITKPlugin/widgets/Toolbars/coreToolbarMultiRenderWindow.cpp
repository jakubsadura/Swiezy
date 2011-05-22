/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMultiRenderWindowMITK.h"
#include "coreMultiRenderWindowEventHandler.h"

//#include ".xpm"
#include "EnableAxisOn.xpm"
#include "FitSceneOn.xpm"
#include "InitAxis.xpm"
#include "XYZPlanesOn.xpm"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarMultiRenderWindow, wxToolBar)
  EVT_TOOL(wxID_FitScene, Core::Widgets::ToolbarMultiRenderWindow::OnFitScene)
  EVT_TOOL(wxID_LockAxis, Core::Widgets::ToolbarMultiRenderWindow::OnEnableAxis)
  EVT_TOOL(wxID_InitAxis, Core::Widgets::ToolbarMultiRenderWindow::OnInitAxis)
  EVT_TOOL(wxID_HideXYZPlanes, Core::Widgets::ToolbarMultiRenderWindow::OnHideXYZPlanes)
END_EVENT_TABLE()


Core::Widgets::ToolbarMultiRenderWindow::ToolbarMultiRenderWindow(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	wxBitmap bitmap;

	// View Interaction with planes
	bitmap = wxBitmap( fitsceneon_xpm );
	AddTool(wxID_FitScene, _T("Fit Scene"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Fit Scene"), _T("Fit Scene"));

	bitmap = wxBitmap( initaxis_xpm );
	AddTool(wxID_InitAxis, _T("Init Axis"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Initialize Axis"), _T("Initialize Axis"));

	bitmap = wxBitmap( enableaxison_xpm );
	AddTool(wxID_LockAxis, _T("Lock Axis"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Lock Axis in the current position"), _T("Lock Axis in the current position"));

	bitmap = wxBitmap( xyzplaneson_xpm );
	AddTool(wxID_HideXYZPlanes, _T("Hide X Y Z planes"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Hide X Y Z planes"), _T("Hide X Y Z planes"));

	Realize();

	Enable( GetMultiRenderWindowMITK() );
}

void Core::Widgets::ToolbarMultiRenderWindow::OnEnableAxis( wxCommandEvent& event )
{
	if ( IsEnabled() )
	{
		GetMultiRenderWindowMITK()->EnableAxis(!GetToolState( wxID_LockAxis ) );
		ToggleTool(wxID_LockAxis, GetMultiRenderWindow()->GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
	}

	event.Skip();
	//m_ROIHelper->StopInteraction();
}

void Core::Widgets::ToolbarMultiRenderWindow::OnFitScene( wxCommandEvent& event )
{
	if( GetToolState(wxID_FitScene) )
	{
		GetMultiRenderWindowMITK()->FitScene();
		GetMultiRenderWindowMITK()->RequestUpdate();
		ToggleTool(wxID_FitScene, false);
	}
	event.Skip();

}

void Core::Widgets::ToolbarMultiRenderWindow::OnInitAxis( wxCommandEvent& event )
{
	if( GetToolState(wxID_InitAxis))
	{
		GetMultiRenderWindowMITK()->ReInitializeViewControllers( );
		ToggleTool(wxID_InitAxis, false);
	}
	event.Skip();
}

void Core::Widgets::ToolbarMultiRenderWindow::OnHideXYZPlanes( wxCommandEvent& event )
{
	if ( GetMultiRenderWindow() != NULL )
	{
		GetMultiRenderWindowMITK()->EnableDisplayPlaneSubtree( !(GetToolState( wxID_HideXYZPlanes )) );
	}

	event.Skip();
}

void Core::Widgets::ToolbarMultiRenderWindow::SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val )
{
	if ( GetMultiRenderWindow() != NULL )
	{
		GetMultiRenderWindow()->GetMetadataHolder( )->RemoveObserver( 
			this,
			&Core::Widgets::ToolbarMultiRenderWindow::UpdateState );
	}

	BaseWindow::SetMultiRenderWindow( val );

	if ( GetMultiRenderWindowMITK() != NULL )
	{
		GetMultiRenderWindow()->GetMetadataHolder( )->AddObserver( 
			this,
			&Core::Widgets::ToolbarMultiRenderWindow::UpdateState );
	}

	UpdateState( );
}

void Core::Widgets::ToolbarMultiRenderWindow::UpdateState()
{
	// Multi render window
	if ( GetMultiRenderWindowMITK() && GetMultiRenderWindow()->GetMetadataHolder( ).IsNotNull() )
	{
		ToggleTool(wxID_LockAxis, GetMultiRenderWindow()->GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
		ToggleTool(wxID_HideXYZPlanes, !GetMultiRenderWindow()->GetMetadata( )->GetTagValue<bool>( "SlicesPlanesEnabled" ) );
	}

	Enable( GetMultiRenderWindowMITK() );
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarMultiRenderWindow::GetProcessor()
{
	return NULL;
}

Core::Widgets::MultiRenderWindowMITK* Core::Widgets::ToolbarMultiRenderWindow::GetMultiRenderWindowMITK()
{
	return dynamic_cast<MultiRenderWindowMITK*> ( GetMultiRenderWindow() );
}
