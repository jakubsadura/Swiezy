/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkMultiSliceFactory.h"
#include "wxMitkSelectableGLWidget.h"


mitk::wxMitkMultiRenderWindow* 
mitk::wxMitkMultiSliceFactory::Build( 
	wxWindow* parent, 
	wxWindowID id,
	mitk::SliceNavigationController::ViewDirection direction,
	int numSlices,
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/ )
{
	wxMitkMultiRenderWindow* window;
	window = new wxMitkMultiRenderWindow( parent, id, pos, size );

	InitRenderWindows( window, direction, numSlices );

	return window;
}



void mitk::wxMitkMultiSliceFactory::InitRenderWindows(
	mitk::wxMitkMultiRenderWindow* window,
	mitk::SliceNavigationController::ViewDirection direction,
	int numSlices )
{
	//!
	mitk::wxMitkMultiRenderWindow::WidgetListType widgets;

	// Create the RenderWindows
	mitk::wxMitkSelectableGLWidget *widget3D;
	widget3D = new mitk::wxMitkSelectableGLWidget(
		window, 
		wxNewId( ), 
		wxT("Win3D"),
		wxDefaultPosition, 
		wxSize(0, 0), 
		0);
	widget3D->Hide();
	widget3D->SetSize( 200, 200 );
	widget3D->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	widgets.push_back( widget3D );

	// Init rest of widgets
	for ( int i = 0 ; i < numSlices ; i++ )
	{

		// Create the RenderWindows
		mitk::wxMitkSelectableGLWidget *widget;
		widget = new mitk::wxMitkSelectableGLWidget(
			window, 
			wxNewId( ), 
			wxString::Format( wxT( "Win%d" ), i ),
			wxDefaultPosition, 
			wxSize(0, 0), 
			0 );
		widget3D->Hide();
		widget3D->SetSize( 200, 200 );

		widget->GetSliceNavigationController( )->SetViewDirection( direction );
		widget->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);

		widgets.push_back( widget );
	}

	window->SetWidgets( widgets );
}



