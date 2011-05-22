/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarLayout.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMultiRenderWindowMITK.h"
#include "coreMultiRenderWindowEventHandler.h"
#include "wxMitkSelectableGLWidget.h"

//#include ".xpm"
#include "2DViewsLeftOn.xpm"
#include "Big3DViewOn.xpm"
#include "Only2DViewsOn.xpm"
#include "StandartLayoutOn.xpm"
#include "XPlaneIconSetOn.xpm"
#include "YPlaneIconSetOn.xpm"
#include "ZPlaneIconSetOn.xpm"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarLayout, wxToolBar)
  EVT_TOOL(wxID_ViewLayout2DViewsLeft, Core::Widgets::ToolbarLayout::On2DViewsLeft)
  EVT_TOOL(wxID_ViewLayoutDefault, Core::Widgets::ToolbarLayout::OnChangeLayoutToDefault)
  EVT_TOOL(wxID_ViewLayoutOnly2DViews, Core::Widgets::ToolbarLayout::OnOnly2DViews)
  EVT_TOOL(wxID_ViewLayoutXView, Core::Widgets::ToolbarLayout::OnXView)
  EVT_TOOL(wxID_ViewLayoutYView, Core::Widgets::ToolbarLayout::OnYView)
  EVT_TOOL(wxID_ViewLayoutZView, Core::Widgets::ToolbarLayout::OnZView)
  EVT_TOOL(wxID_ViewLayoutBig3DView, Core::Widgets::ToolbarLayout::OnBig3DView)
END_EVENT_TABLE()


Core::Widgets::ToolbarLayout::ToolbarLayout(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	wxBitmap bitmap;

	// Layout of multiRenderWindow

	bitmap = wxBitmap( standartlayouton_xpm );
	AddTool(wxID_ViewLayoutDefault, _T("Standard"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout Standard"), _T("Layout Standard"));

	bitmap = wxBitmap( s2dviewslefton_xpm );
	AddTool(wxID_ViewLayout2DViewsLeft, _T("2D Views Left"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout 2D windows left"), _T("Layout 2D windows left"));

	bitmap = wxBitmap( only2dviewson_xpm );
	AddTool(wxID_ViewLayoutOnly2DViews, _T("Only 2d Views"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout Only 2d Views"), _T("Layout Only 2d Views"));

	bitmap = wxBitmap( xplaneiconseton_xpm );
	AddTool(wxID_ViewLayoutXView, _T("X View"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout X View"), _T("Layout X View"));

	bitmap = wxBitmap( yplaneiconseton_xpm );
	AddTool(wxID_ViewLayoutYView, _T("Y View"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout Y View"), _T("Layout Y View"));

	bitmap = wxBitmap( zplaneiconseton_xpm );
	AddTool(wxID_ViewLayoutZView, _T("Z View"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Z View"), _T("Z View"));

	bitmap = wxBitmap( big3dviewon_xpm );
	AddTool(wxID_ViewLayoutBig3DView, _T("Single 3D window"),
		bitmap, wxNullBitmap, wxITEM_RADIO,
		_T("Layout Single 3D window"), _T("Layout Single 3D window"));

	Realize();

	Enable( GetMultiRenderWindowMITK() );
}

void Core::Widgets::ToolbarLayout::OnChangeLayoutToDefault( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Default2x2 );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::On2DViewsLeft( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Left2DRight3D );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::OnOnly2DViews( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Only2D );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::OnBig3DView( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Single3D );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::OnXView( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Single2D_X );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::OnYView( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Single2D_Y );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::OnZView( wxCommandEvent& event )
{
	GetMultiRenderWindowMITK()->SetCurrentLayout( mitk::Single2D_Z );
	event.Skip();
}

void Core::Widgets::ToolbarLayout::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	if ( GetMultiRenderWindow() != NULL )
	{
		GetMultiRenderWindow()->GetMetadataHolder( )->RemoveObserver( 
			this,
			&Core::Widgets::ToolbarLayout::UpdateState );
	}

	BaseWindow::SetMultiRenderWindow( val );

	if ( GetMultiRenderWindowMITK() != NULL )
	{
		GetMultiRenderWindow()->GetMetadataHolder( )->AddObserver( 
			this,
			&Core::Widgets::ToolbarLayout::UpdateState );
	}

	UpdateState( );
}

void Core::Widgets::ToolbarLayout::UpdateState()
{
	// Multi render window
	if ( GetMultiRenderWindowMITK() && 
		GetMultiRenderWindow()->GetMetadataHolder( ).IsNotNull() )
	{
		// If it's single, check current window
		int layoutType = GetMultiRenderWindow()->GetMetadata( )->GetTagValue<int>( "LayoutType" );
		if ( layoutType == mitk::Single )
		{
			mitk::wxMitkMultiRenderWindow::WidgetListType widgets;
			widgets = GetMultiRenderWindowMITK( )->GetWidgets();
			mitk::wxMitkMultiRenderWindow::WidgetListType::iterator it;
			int count = 0;
			for ( it = widgets.begin() ; it != widgets.end() ; it++ )
			{
				if ( (*it)->IsShown() )
				{
					switch(count)
					{
					case mitk::ThreeD_Win:layoutType = mitk::Single3D;break;
					case mitk::X_Win:layoutType = mitk::Single2D_X;break;
					case mitk::Y_Win:layoutType = mitk::Single2D_Y;break;
					case mitk::Z_Win:layoutType = mitk::Single2D_Z;break;
					}
				}
				count++;
			}
		}

		switch ( layoutType )
		{
		case mitk::NoneLayout:ToggleTool( wxID_ViewLayoutDefault, true );break;
		case mitk::Default2x2:ToggleTool( wxID_ViewLayoutDefault, true );break;
		case mitk::Left2DRight3D:ToggleTool( wxID_ViewLayout2DViewsLeft, true );break;
		case mitk::Single3D:ToggleTool( wxID_ViewLayoutBig3DView, true );break;
		case mitk::Single2D_X:ToggleTool( wxID_ViewLayoutXView, true );break;
		case mitk::Single2D_Y:ToggleTool( wxID_ViewLayoutYView, true );break;
		case mitk::Single2D_Z:ToggleTool( wxID_ViewLayoutZView, true );break;
		case mitk::Only2D:ToggleTool( wxID_ViewLayoutOnly2DViews, true );break;
		default:break;
		}
	}

	Enable( GetMultiRenderWindowMITK() );
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarLayout::GetProcessor()
{
	return NULL;
}

Core::Widgets::MultiRenderWindowMITK* Core::Widgets::ToolbarLayout::GetMultiRenderWindowMITK()
{
	return dynamic_cast<MultiRenderWindowMITK*> ( GetMultiRenderWindow() );
}
