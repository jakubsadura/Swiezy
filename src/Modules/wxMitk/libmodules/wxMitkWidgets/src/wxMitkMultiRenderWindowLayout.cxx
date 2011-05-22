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

#include "wx/utils.h"

#include <math.h>

#include "wxMitkMultiRenderWindowLayout.h"
#include "wxMitkLevelWindowWidget.h"
#include "wxMitkSelectableGLWidget.h"

using namespace mitk;


//!
wxMitkMultiRenderWindowLayout::wxMitkMultiRenderWindowLayout() : wxBoxSizer( wxHORIZONTAL )
{
	m_BackupCurrentLayout = Default2x2_YXZ3D;
	m_CurrentLayout = Default2x2_YXZ3D;
	m_LevelWindowWidget = NULL;
}

//!
wxMitkMultiRenderWindowLayout::~wxMitkMultiRenderWindowLayout(void)
{
}

/**
*/
void wxMitkMultiRenderWindowLayout::ShowLevelWindow(bool enable)
{
	if ( !m_LevelWindowWidget )
	{
		return;
	}

	if(enable)
	{
		m_LevelWindowWidget->GetLevelWindowManager()->SetAutoTopMostImage(true);
		m_LevelWindowWidget->Show(true);
	}
	else
	{
		m_LevelWindowWidget->Show(false);
	}
}

//!
void wxMitkMultiRenderWindowLayout::ChangeLayoutToDefault(void)
{
	// Create the layouts and set the layout config
	m_CurrentLayout = Default2x2;
	
	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	size_t totalViews = m_Views.size();
	float sqrtTotalViews = sqrt( float( totalViews ) );
	int cols = sqrtTotalViews;
	int rows = sqrtTotalViews;

	// If totalViews is not an exact sqrt, add a col
	if ( totalViews > cols * rows )
	{
		cols++;
	}

	// If totalViews is > sqrt + 1 col -> Add a row
	if ( totalViews > cols * rows )
	{
		rows++;
	}


	// Display only the required windows
	wxGridSizer* gLayoutSizer = new wxGridSizer(rows, cols, 3, 3);
	WidgetListType sliceViews = GetSliceViews( );
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->Show( );
		gLayoutSizer->Add((*it), 1, wxEXPAND | wxALL, 2);
	}

	Get3DView()->Show();
	gLayoutSizer->Add(Get3DView(), 1, wxEXPAND | wxALL, 2);

	Add(gLayoutSizer, 1, wxEXPAND);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column

}


//!
void wxMitkMultiRenderWindowLayout::ChangeLayoutTo2DImagesLeft(void)
{
	// Create the layouts and set the layout config
	m_CurrentLayout = Left2DRight3D;
	wxBoxSizer* vLayoutSizer = new wxBoxSizer(wxVERTICAL);
	
	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	// Display only the required windows
	ShowAllViews( true );
	WidgetListType sliceViews = GetSliceViews( );
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		vLayoutSizer->Add( (*it), 1, wxEXPAND | wxALL, 2);
	}

	Add(vLayoutSizer, 1, wxEXPAND);
	Add(Get3DView(), 1, wxEXPAND | wxALL, 2);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column

}

//!
void wxMitkMultiRenderWindowLayout::ChangeLayoutToSingle(
	wxMitkSelectableGLWidget* widget )
{
	// Create the layouts and set the layout config
	if ( widget == GetXView() )
	{
		m_CurrentLayout = Single2D_X;
	}
	else if ( widget == GetYView() )
	{
		m_CurrentLayout = Single2D_Y;
	}
	else if ( widget == GetZView() )
	{
		m_CurrentLayout = Single2D_Z;
	}
	else if ( widget == Get3DView() )
	{
		m_CurrentLayout = Single3D;
	}
	else
	{
		m_CurrentLayout = Single;
	}

	
	if ( widget == NULL )
	{
		widget = GetShowWidget( );
	}

	// 3D is Always available
	if ( widget == NULL )
	{
		widget = Get3DView();
	}

	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	// Display only the required windows
	widget->Show( true );
	
	// Relayout
	Add(widget, 1, wxEXPAND | wxALL, 2);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column
}


//!
void wxMitkMultiRenderWindowLayout::ChangeLayoutToOnly2D(void)
{
	if ( !GetYView() || !GetXView() || !GetZView() )
	{
		return;
	}

	// Create the layouts and set the layout config
	m_CurrentLayout = Only2D;
	wxBoxSizer* vLayoutSizerInternal = new wxBoxSizer(wxVERTICAL);
	
	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	// Display only the required windows
	WidgetListType sliceViews = GetSliceViews( );
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->Show( );
	}

	// Relayout
	vLayoutSizerInternal->Add(GetXView(), 1, wxEXPAND | wxALL, 2);
	vLayoutSizerInternal->Add(GetYView(), 1, wxEXPAND | wxALL, 2);
	vLayoutSizerInternal->Add(GetZView(), 1, wxEXPAND | wxALL, 2);
	
	Add(vLayoutSizerInternal, 1, wxEXPAND);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column

}

void mitk::wxMitkMultiRenderWindowLayout::SetCurrentLayout( 
	LayoutConfiguration layout,
	wxMitkSelectableGLWidget* widget )
{

	switch( layout )
	{
	case NoneLayout:break;
	case Default2x2:ChangeLayoutToDefault( );break;
	case Left2DRight3D:ChangeLayoutTo2DImagesLeft( );break;
	case Single2D_X:ChangeLayoutToSingle( GetXView() );break;
	case Single2D_Y:ChangeLayoutToSingle( GetYView() );break;
	case Single2D_Z:ChangeLayoutToSingle( GetZView() );break;
	case Single:ChangeLayoutToSingle( widget );break;
	case Single3D:ChangeLayoutToSingle( Get3DView( ) );break;
	case Only2D:ChangeLayoutToOnly2D( );break;
	case Default2x2_YXZ3D:ChangeLayoutTo2x2_YXZ3D( );break;
	case Default2x2_XYZ3D:ChangeLayoutTo2x2_XYZ3D( );break;
	default:break;
	}

}

mitk::LayoutConfiguration mitk::wxMitkMultiRenderWindowLayout::GetCurrentLayout() const
{
	return m_CurrentLayout;
}

bool mitk::wxMitkMultiRenderWindowLayout::RestoreLayout()
{
	if ( ( GetCurrentLayout( ) == Single ||
		 GetCurrentLayout( ) == Single2D_X ||
		 GetCurrentLayout( ) == Single2D_Y ||
		 GetCurrentLayout( ) == Single2D_Z ||
		 GetCurrentLayout( ) == Single3D ) 
		&& m_BackupCurrentLayout != NoneLayout )
	{
		SetCurrentLayout( m_BackupCurrentLayout );
		return true;
	}

	return false;
}

void mitk::wxMitkMultiRenderWindowLayout::BackupLayout()
{
	if ( m_CurrentLayout != Single )
	{
		m_BackupCurrentLayout = m_CurrentLayout;
	}
}

void mitk::wxMitkMultiRenderWindowLayout::SetLevelWindowWidget( wxMitkLevelWindowWidget* val )
{
	m_LevelWindowWidget = val;
}

void mitk::wxMitkMultiRenderWindowLayout::SetViews( std::list<wxMitkSelectableGLWidget*> val )
{
	m_Views = val;
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindowLayout::Get3DView()
{
	if ( m_Views.empty() )
	{
		return NULL;
	}

	return (*m_Views.begin());
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindowLayout::GetXView()
{
	WidgetListType widgets;
	widgets = GetSliceViews( mitk::SliceNavigationController::Sagittal );
	if ( widgets.size() == 0 )
	{
		return NULL;
	}
	return (*widgets.begin());
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindowLayout::GetYView()
{
	WidgetListType widgets;
	widgets = GetSliceViews( mitk::SliceNavigationController::Frontal );
	if ( widgets.size() == 0 )
	{
		return NULL;
	}
	return (*widgets.begin());
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindowLayout::GetZView()
{
	WidgetListType widgets;
	widgets = GetSliceViews( mitk::SliceNavigationController::Transversal );
	if ( widgets.size() == 0 )
	{
		return NULL;
	}
	return (*widgets.begin());
}

mitk::wxMitkMultiRenderWindowLayout::WidgetListType 
mitk::wxMitkMultiRenderWindowLayout::GetSliceViews()
{
	WidgetListType sliceViews;

	WidgetListType::iterator it;
	for ( it = m_Views.begin() ; it != m_Views.end() ; it++ )
	{
		if ( (*it)->GetRenderer()->GetMapperID() == mitk::BaseRenderer::Standard2D )
		{
			sliceViews.push_back( (*it) );
		}
	}

	return sliceViews;
}

mitk::wxMitkMultiRenderWindowLayout::WidgetListType 
mitk::wxMitkMultiRenderWindowLayout::GetSliceViews( 
	mitk::SliceNavigationController::ViewDirection direction )
{
	WidgetListType sliceViews;
	WidgetListType::iterator it;
	for ( it = m_Views.begin() ; it != m_Views.end() ; it++ )
	{
		if ( (*it)->GetRenderer()->GetMapperID() == mitk::BaseRenderer::Standard2D &&
			 (*it)->GetSliceNavigationController()->GetViewDirection( ) == direction )
		{
			sliceViews.push_back( (*it) );
		}
	}

	return sliceViews;
}

void mitk::wxMitkMultiRenderWindowLayout::ShowAllViews( bool show )
{
	WidgetListType::iterator it;
	for ( it = m_Views.begin() ; it != m_Views.end() ; it++ )
	{
		(*it)->Show( show );
	}
}

wxMitkSelectableGLWidget* 
mitk::wxMitkMultiRenderWindowLayout::GetShowWidget()
{
	WidgetListType::iterator it;
	for ( it = m_Views.begin() ; it != m_Views.end() ; it++ )
	{
		if ( (*it)->IsShown( ) )
		{
			return (*it);
		}

	}

	return NULL;
}

void mitk::wxMitkMultiRenderWindowLayout::ChangeLayoutTo2x2_YXZ3D( )
{
	if ( !GetYView() || !GetXView() || !GetZView() )
	{
		return;
	}

	// Create the layouts and set the layout config
	m_CurrentLayout = Default2x2_YXZ3D;

	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	// Display only the required windows
	wxGridSizer* gLayoutSizer = new wxGridSizer(2, 2, 3, 3);

	GetYView()->Show();
	gLayoutSizer->Add(GetYView(), 1, wxEXPAND | wxALL, 2);
	GetXView()->Show();
	gLayoutSizer->Add(GetXView(), 1, wxEXPAND | wxALL, 2);
	GetZView()->Show();
	gLayoutSizer->Add(GetZView(), 1, wxEXPAND | wxALL, 2);
	Get3DView()->Show();
	gLayoutSizer->Add(Get3DView(), 1, wxEXPAND | wxALL, 2);

	Add(gLayoutSizer, 1, wxEXPAND);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column}
}

void mitk::wxMitkMultiRenderWindowLayout::ChangeLayoutTo2x2_XYZ3D( void )
{
	if ( !GetYView() || !GetXView() || !GetZView() )
	{
		return;
	}

	// Create the layouts and set the layout config
	m_CurrentLayout = Default2x2_XYZ3D;

	// Clear previous layouts
	Clear(false);
	ShowAllViews( false );

	// Display only the required windows
	wxGridSizer* gLayoutSizer = new wxGridSizer(2, 2, 3, 3);

	GetXView()->Show();
	gLayoutSizer->Add(GetXView(), 1, wxEXPAND | wxALL, 2);
	GetYView()->Show();
	gLayoutSizer->Add(GetYView(), 1, wxEXPAND | wxALL, 2);
	GetZView()->Show();
	gLayoutSizer->Add(GetZView(), 1, wxEXPAND | wxALL, 2);
	Get3DView()->Show();
	gLayoutSizer->Add(Get3DView(), 1, wxEXPAND | wxALL, 2);

	Add(gLayoutSizer, 1, wxEXPAND);
	Add(m_LevelWindowWidget, 0, wxEXPAND); // make m_LevelWindowWidget fill the whole column}
}
