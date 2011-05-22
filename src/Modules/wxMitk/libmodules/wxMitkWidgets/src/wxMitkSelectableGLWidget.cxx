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

#include "wxMitkSelectableGLWidget.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"

#include <mitkVtkPropRenderer.h>
#include <mitkSliceNavigationController.h>
#include <mitkCameraRotationController.h>


using namespace mitk;

// Colour borders so as to ease identifying the window / view direction
// This call produces memory leaks: wxT("BLUE")
wxColor wxMitkSelectableGLWidget::RED_COLOR = wxColor( 255, 0, 0 );
wxColor wxMitkSelectableGLWidget::GREEN_COLOR = wxColor( 0, 255, 0 );
wxColor wxMitkSelectableGLWidget::BLUE_COLOR = wxColor( 128, 128, 255 );
wxColor wxMitkSelectableGLWidget::YELLOW_COLOR = wxColor( 255, 255, 0 );
wxColor wxMitkSelectableGLWidget::BLACK_COLOR = wxColor( 255, 255, 255 );


wxMitkSelectableGLWidget::wxMitkSelectableGLWidget(
	wxWindow* parent, 
	wxWindowID id, 
	const wxString& name,
	const wxPoint& pos, 
	const wxSize& size, 
	long style) 
: wxPanel(parent, id, pos, size, style, name)
{
	//this->SetId(wxID_ANY);

	// Create the RenderWindow
	this->internalRenderWindow = new mitk::wxMitkRenderWindow(
		this, 
		wxNewId( ), 
		NULL, 
		wxDefaultPosition, 
		size,
		0,
		name );

	// Colour background at the border in order to ease identification of windows
	// This call produces memory leaks: wxColour(wxT("BLACK"))
	this->SetOwnBackgroundColour( wxColour( 0, 0, 0 ) );
	this->SetOwnForegroundColour( wxColour( 0, 0, 0 ) );
	
	// Colour background black displays nicer when resizing fast, due to the rendering lag 
	this->internalRenderWindow->SetOwnBackgroundColour( wxColour( 0, 0, 0 ) );
	this->internalRenderWindow->SetOwnForegroundColour( wxColour( 0, 0, 0 ) );

	// Layout renderwindow inside its placeholder in order to show a coloured border
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(this->internalRenderWindow, 1, wxEXPAND | wxALL, 3);
	this->SetSizer(sizer);
	this->SetAutoLayout(true);

	m_CornerAnnotation = blMitkCornerAnnotation::New( );
	m_CornerAnnotation->SetRenderWindow( GetVtkRenderWindow( ) );

	itk::SimpleMemberCommand<wxMitkSelectableGLWidget>::Pointer command;
	command = itk::SimpleMemberCommand<wxMitkSelectableGLWidget>::New();
	command->SetCallbackFunction(this, &wxMitkSelectableGLWidget::OnModifiedNavigationController);
	m_NavigationControllerObserverTag = GetSliceNavigationController()->AddObserver( 
		itk::ModifiedEvent( ), command );

	EnableOrientationAnnotations( false );
	EnableCornerAnnotations( false );

	// Set default parameters for SliceNavigationController
	GetSliceNavigationController( )->SetViewDirection( mitk::SliceNavigationController::Original );
	GetSliceNavigationController( )->SetTop( true ); // true
	GetSliceNavigationController( )->SetFrontSide( true ); // true
	GetSliceNavigationController( )->SetRotated( false ); // false

	m_SlicePosObserverTag = -1;
}

//!
wxMitkSelectableGLWidget::~wxMitkSelectableGLWidget(void)
{
	wxMitkDisplayPlaneSubtreeHelper::RemoveDisplayPlaneSubTree( this );

	GetSliceNavigationController()->RemoveObserver( m_NavigationControllerObserverTag );
	Enable( false );
}

/** 
Colours the border of the rendering area in order to ease identification of windows when the 
client is displaying multiple views
*/
void wxMitkSelectableGLWidget::SetFrameBorderColor(wxColour& color)
{
	this->SetOwnBackgroundColour(color);
	this->SetOwnForegroundColour(color);
}

/** 
*/
wxColour wxMitkSelectableGLWidget::GetFrameBorderColor()
{
	return this->GetBackgroundColour();
}

void wxMitkSelectableGLWidget::GetColourAsFloat( float colour[3] )
{
	if (GetBackgroundColour().Ok())
	{
		colour[ 0 ] = (float)(GetBackgroundColour().Red()/256.0);
		colour[ 1 ] = (float)(GetBackgroundColour().Green()/256.0);
		colour[ 2 ] = (float)(GetBackgroundColour().Blue()/256.0);
	}
}

//!
mitk::VtkPropRenderer* wxMitkSelectableGLWidget::GetRenderer(void) const
{
	return this->internalRenderWindow->GetRenderer();
}

//!
wxMitkRenderWindow* wxMitkSelectableGLWidget::GetMitkRenderWindow(void) const
{
	return this->internalRenderWindow;
}

//!
vtkRenderWindow* wxMitkSelectableGLWidget::GetVtkRenderWindow(void) const
{
	if(this->internalRenderWindow == NULL) 
		return NULL;
	return this->internalRenderWindow->GetRenderWindow();
}

//!
mitk::SliceNavigationController* wxMitkSelectableGLWidget::GetSliceNavigationController(void) const
{
	return this->internalRenderWindow->GetSliceNavigationController();
}

//!
mitk::CameraRotationController* wxMitkSelectableGLWidget::GetCameraRotationController(void) const
{
	return this->internalRenderWindow->GetCameraRotationController();
}

//!
mitk::BaseController* wxMitkSelectableGLWidget::GetController(void) const
{
	return this->internalRenderWindow->GetController();
}

/**
Tells the rendering manager to request update on the render windows managed by this widget
*/
void wxMitkSelectableGLWidget::RequestUpdate(void)
{
	this->GetMitkRenderWindow()->RequestUpdate();
}

/**
Tells the rendering manager to force inmediate update on the render windows managed by this widget
*/
void wxMitkSelectableGLWidget::ForceImmediateUpdate(void)
{
	this->GetMitkRenderWindow()->ForceImmediateUpdate();
}

bool mitk::wxMitkSelectableGLWidget::Enable( bool enable /*= true */ )
{
	if ( enable )
	{
		mitk::RenderingManager::GetInstance()->AddRenderWindow(this->GetVtkRenderWindow());
	}
	else
	{
		mitk::RenderingManager::GetInstance()->RemoveRenderWindow(this->GetVtkRenderWindow());
	}

	return wxPanel::Enable( enable );
}

void mitk::wxMitkSelectableGLWidget::EnableCornerAnnotations( bool enable )
{
	m_CornerAnnotation->SetShowCornerAnnotations( enable );
}

bool mitk::wxMitkSelectableGLWidget::GetCornerAnnotationsEnabled()
{
	return m_CornerAnnotation->GetShowCornerAnnotations( );
}

void mitk::wxMitkSelectableGLWidget::EnableOrientationAnnotations( bool enable )
{
	m_CornerAnnotation->SetShowOrientationAnnotations( enable );
}

bool mitk::wxMitkSelectableGLWidget::GetOrientationAnnotationsEnabled()
{
	return m_CornerAnnotation->GetShowOrientationAnnotations( );
}

void mitk::wxMitkSelectableGLWidget::UpdateAnnotations()
{

	std::vector<std::string> letters;

	// Standard letters
	mitk::SliceNavigationController::ViewDirection direction;
	direction = GetSliceNavigationController( )->GetViewDirection( );
	switch ( direction )
	{
	case mitk::SliceNavigationController::Transversal: 
		letters.push_back( "P" );
		letters.push_back( "R" );
		letters.push_back( "A" );
		letters.push_back( "L" );
		break;
	case mitk::SliceNavigationController::Sagittal:
		letters.push_back( "S" );
		letters.push_back( "A" );
		letters.push_back( "I" );
		letters.push_back( "P" );
		break;
	case mitk::SliceNavigationController::Frontal:
		letters.push_back( "S" );
		letters.push_back( "R" );
		letters.push_back( "I" );
		letters.push_back( "L" );
	}

	// Horizontal flip -> Switch 1 and 3
	if ( letters.size( ) && GetSliceNavigationController( )->GetFrontSide( ) )
	{
		std::string letter;
		letter = letters[ 1 ];
		letters[ 1 ] = letters[ 3 ];
		letters[ 3 ] = letter;
	}
	
	long angleOfRotation = 0;
	if ( GetSliceNavigationController( )->GetRotated( ) )
	{
		angleOfRotation = GetSliceNavigationController( )->GetAngleOfRotation( );
	}


	if ( letters.size() )
	{
		m_CornerAnnotation->SetNorthAnnotation( letters[ (0 - angleOfRotation / 90 + 4) % 4 ].c_str() );
		m_CornerAnnotation->SetEastAnnotation ( letters[ (1 - angleOfRotation / 90 + 4) % 4 ].c_str() );    
		m_CornerAnnotation->SetSouthAnnotation( letters[ (2 - angleOfRotation / 90 + 4) % 4 ].c_str() );    
		m_CornerAnnotation->SetWestAnnotation( letters[ (3 - angleOfRotation / 90 + 4) % 4 ].c_str() );
	}

}

blMitkCornerAnnotation::Pointer mitk::wxMitkSelectableGLWidget::GetCornerAnnotation() const
{
	return m_CornerAnnotation;
}

void mitk::wxMitkSelectableGLWidget::UpdateController( 
	mitk::TimeSlicedGeometry::Pointer geometry, 
	bool keepOrientation )
{
	GetSliceNavigationController()->SetKeepPlanePosition( keepOrientation );
	GetSliceNavigationController()->SetInputWorldGeometry(geometry);
	GetSliceNavigationController()->Update( );
}


void mitk::wxMitkSelectableGLWidget::OnModifiedNavigationController()
{

	UpdateAnnotations();

	UpdateColor();
}

void mitk::wxMitkSelectableGLWidget::UpdateColor()
{
	// Update frame border color
	wxColor color;
	switch ( GetSliceNavigationController()->GetViewDirection() )
	{
	case mitk::SliceNavigationController::Sagittal:
		color = wxMitkSelectableGLWidget::RED_COLOR; break;
	case mitk::SliceNavigationController::Frontal:
		color = wxMitkSelectableGLWidget::GREEN_COLOR; break;
	case mitk::SliceNavigationController::Transversal:
		color = wxMitkSelectableGLWidget::BLUE_COLOR; break;
	case mitk::SliceNavigationController::Original:
		color = wxMitkSelectableGLWidget::YELLOW_COLOR; break;
	}
	SetFrameBorderColor( color );

	// Update color of node
	if ( GetSliceNavigationController()->GetViewDirection() != 
		mitk::SliceNavigationController::Original )
	{
		wxMitkDisplayPlaneSubtreeHelper::UpdateDisplayPlaneSubTree( this );
	}

	Refresh();
}

void mitk::wxMitkSelectableGLWidget::RemoveSlicePosObserver()
{
	GetSliceNavigationController()->GetSlice()->RemoveObserver( m_SlicePosObserverTag );
}

