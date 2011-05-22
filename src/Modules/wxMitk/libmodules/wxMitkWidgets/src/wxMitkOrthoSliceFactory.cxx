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

#include "wxMitkOrthoSliceFactory.h"
#include "wxMitkSelectableGLWidget.h"


mitk::wxMitkMultiRenderWindow* 
mitk::wxMitkOrthoSliceFactory::Build( 
	wxWindow* parent, 
	wxWindowID id,
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/ )
{
	wxMitkMultiRenderWindow* window;
	window = new wxMitkMultiRenderWindow( parent, id, pos, size );

	InitRenderWindows( window );

	return window;
}



void mitk::wxMitkOrthoSliceFactory::InitRenderWindows(
	mitk::wxMitkMultiRenderWindow* window )
{
	//!
	mitk::wxMitkMultiRenderWindow::WidgetListType widgets;

	// Create the RenderWindows
	mitk::wxMitkSelectableGLWidget *widget3D;
	mitk::wxMitkSelectableGLWidget *widgetX;
	mitk::wxMitkSelectableGLWidget *widgetY;
	mitk::wxMitkSelectableGLWidget *widgetZ;

	widget3D = new mitk::wxMitkSelectableGLWidget(
		window, 
		wxNewId( ), 
		wxT("Win3D"), 
		wxDefaultPosition, 
		wxSize(0, 0), 
		0);
	widget3D->Hide();
	widget3D->SetSize( 200, 200 );

	widgetX = new mitk::wxMitkSelectableGLWidget(
		window, 
		wxNewId( ), 
		wxT("WinX"), 
		wxDefaultPosition, 
		wxSize(0, 0), 
		0 );
	widgetX->Hide();
	widgetX->SetSize( 200, 200 );

	widgetY = new mitk::wxMitkSelectableGLWidget(
		window, 
		wxNewId( ), 
		wxT("WinY"), 
		wxDefaultPosition, 
		wxSize(0, 0), 
		0);
	widgetY->Hide();
	widgetY->SetSize( 200, 200 );

	widgetZ = new mitk::wxMitkSelectableGLWidget(
		window, 
		wxNewId( ), 
		wxT("WinZ"), 
		wxDefaultPosition, 
		wxSize(0, 0), 
		0 );
	widgetZ->Hide();
	widgetZ->SetSize( 200, 200 );

	widgetX->GetSliceNavigationController( )->SetViewDirection( mitk::SliceNavigationController::Sagittal );
	widgetX->GetSliceNavigationController( )->SetTop( true ); // true
	widgetX->GetSliceNavigationController( )->SetFrontSide( true ); // true
	widgetX->GetSliceNavigationController( )->SetRotated( false ); // false

	widgetY->GetSliceNavigationController( )->SetViewDirection( mitk::SliceNavigationController::Frontal );
	widgetY->GetSliceNavigationController( )->SetTop( true ); // true
	widgetY->GetSliceNavigationController( )->SetFrontSide( true ); // true
	widgetY->GetSliceNavigationController( )->SetRotated( false ); // false

	widgetZ->GetSliceNavigationController( )->SetViewDirection( mitk::SliceNavigationController::Transversal );
	widgetZ->GetSliceNavigationController( )->SetTop( true ); // true
	widgetZ->GetSliceNavigationController( )->SetFrontSide( false ); // false
	widgetZ->GetSliceNavigationController( )->SetRotated( true ); // true

	widget3D->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
	widgetX->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);
	widgetY->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);
	widgetZ->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);

	widgets.push_back( widget3D );
	widgets.push_back( widgetZ );
	widgets.push_back( widgetX );
	widgets.push_back( widgetY );
	window->SetWidgets( widgets );

}



