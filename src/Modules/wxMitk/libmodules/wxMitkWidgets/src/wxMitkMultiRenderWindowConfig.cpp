/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkMultiRenderWindowConfig.h"
#include "wxMitkViewConfiguration.h"
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkSelectableGLWidget.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"

BEGIN_EVENT_TABLE(mitk::wxMitkMultiRenderWindowConfig, wxMitkMultiRenderWindowConfigUI)
    // begin wxGlade: wxMitkViewConfigurationUI::event_table
    EVT_COMBOBOX(wxID_VIEW_CMB_DIRECTION, mitk::wxMitkMultiRenderWindowConfig::OnDirection)
    EVT_CHECKBOX(wxID_VIEW_CHK_FLIP, mitk::wxMitkMultiRenderWindowConfig::OnFlip)
    EVT_RADIOBOX(wxID_RDBX_ROTATE, mitk::wxMitkMultiRenderWindowConfig::OnRotate)
    // end wxGlade
END_EVENT_TABLE();


mitk::wxMitkMultiRenderWindowConfig::wxMitkMultiRenderWindowConfig(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxMitkMultiRenderWindowConfigUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_View = NULL;
	m_spinNumberOfSlices->SetRange( 0, INT_MAX );
}


void mitk::wxMitkMultiRenderWindowConfig::SetView( mitk::wxMitkMultiRenderWindow* view )
{
	m_View = view;
	if ( m_View == NULL )
	{
		Enable( false );
		SetActiveWidget( -1 );
		return;
	}

	Enable( true );

	UpdateWidget( );

	SetActiveWidget( 0 );
}

void mitk::wxMitkMultiRenderWindowConfig::UpdateWidget( )
{
	// Get Spin value
	wxMitkMultiRenderWindow::WidgetListType widgets;
	widgets = m_View->GetLayout()->GetSliceViews( );
	m_spinNumberOfSlices->SetValue( widgets.size() );

	// Fill combo box
	m_cmbCurrentView->Clear();
	wxMitkMultiRenderWindow::WidgetListType::iterator it;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		m_cmbCurrentView->Append( (*it)->GetName( ) );
	}

	if ( m_ViewConfig->GetView() )
	{
		m_cmbCurrentView->SetStringSelection( m_ViewConfig->GetView()->GetName().c_str() );
	}
}

void mitk::wxMitkMultiRenderWindowConfig::OnDirection( wxCommandEvent &event )
{
	if ( !ApplyConfigToAllSlices( true, false, false ) )
	{
		m_View->ReInitializeViewControllers();
	}
}

void mitk::wxMitkMultiRenderWindowConfig::OnFlip( wxCommandEvent &event )
{
	ApplyConfigToAllSlices( false, true, false );
}

void mitk::wxMitkMultiRenderWindowConfig::OnRotate( wxCommandEvent &event )
{
	ApplyConfigToAllSlices( false, false, true );
}

void mitk::wxMitkMultiRenderWindowConfig::OnApplyAll( wxCommandEvent &event )
{
	ApplyConfigToAllSlices( true, true, true );
}

bool mitk::wxMitkMultiRenderWindowConfig::ApplyConfigToAllSlices(
	bool direction, bool flip, bool rotation )
{
	if ( m_chkApplyToAll->IsChecked() )
	{
		wxMitkMultiRenderWindow::WidgetListType widgets;
		widgets = m_View->GetLayout()->GetSliceViews( );
		wxMitkMultiRenderWindow::WidgetListType::iterator it;
		for ( it = widgets.begin() ; it != widgets.end() ; it++ )
		{
			mitk::SliceNavigationController::Pointer navigationController;
			navigationController = (*it)->GetSliceNavigationController( );
			if ( direction )
			{
				navigationController->SetViewDirection( m_ViewConfig->GetDirection( ) );
			}
			if ( flip )
			{
				navigationController->SetFrontSide( m_ViewConfig->GetFlip() );
			}
			if ( rotation )
			{
				navigationController->SetAngleOfRotation( m_ViewConfig->GetAngleOfRotation() );
				navigationController->SetRotated( m_ViewConfig->GetAngleOfRotation() != 0 );
			}
			navigationController->Update( );
		}

		m_View->ReInitializeViewControllers();

		return true;
	}

	return false;
}

void mitk::wxMitkMultiRenderWindowConfig::OnSelectedView( wxCommandEvent &event )
{
	wxString selText = m_cmbCurrentView->GetStringSelection( );
	SetActiveWidget( selText.c_str() );
}

void mitk::wxMitkMultiRenderWindowConfig::OnNumberOfSlices( wxSpinEvent &event )
{
	if ( m_View == NULL )
	{
		return;
	}

	wxMitkMultiRenderWindow::WidgetListType widgets;
	widgets = m_View->GetLayout( )->GetSliceViews();

	if ( m_spinNumberOfSlices->GetValue() != widgets.size() )
	{
		// This is very important because 
		// mitk::Geometry2DDataMapper2D::GenerateData() will get a pointer to 
		// the nodes of the planes
		// mitk::Geometry2DDataMapper2D::Paint(BaseRenderer *renderer)
		// will use the pointers to render the window
		// wxMitkRenderWindow::OnPaint( ) cannot be called while we
		// are destroying window
		m_View->Hide();

		// Destroy widgets
		wxMitkMultiRenderWindow::WidgetListType::iterator it;
		int widgetCount = 0;
		for ( it = widgets.begin() ; it != widgets.end() ; it++ )
		{
			if ( widgetCount >= m_spinNumberOfSlices->GetValue() )
			{
				m_View->Hide( );

				m_View->RemoveWidget( (*it) );

				if ( m_ViewConfig->GetView() == (*it) )
				{
					SetActiveWidget( -1 );
				}

				(*it)->Destroy();
			}
			widgetCount++;
		}

		// Create widgets
		for ( ;widgetCount < m_spinNumberOfSlices->GetValue(); widgetCount++ )
		{

			// Create the RenderWindows
			mitk::wxMitkSelectableGLWidget *widget;
			widget = new mitk::wxMitkSelectableGLWidget(
				m_View, 
				wxNewId( ), 
				wxString::Format( wxT( "Win%d" ), widgetCount ),
				wxDefaultPosition, 
				wxSize(0, 0), 
				0 );
			widget->Hide();
			widget->SetSize( 200, 200 );

			mitk::SliceNavigationController::Pointer navigationController;
			navigationController = widget->GetSliceNavigationController( );

			navigationController->SetViewDirection( m_ViewConfig->GetDirection() );
			navigationController->SetFrontSide( m_ViewConfig->GetFlip() );
			navigationController->SetAngleOfRotation( m_ViewConfig->GetAngleOfRotation() );
			navigationController->SetRotated( m_ViewConfig->GetAngleOfRotation() != 0 );
			navigationController->Update();

			widget->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);

			m_View->AddWidget( widget );

			wxMitkDisplayPlaneSubtreeHelper::UpdateDisplayPlaneSubTree( widget );
		}

		m_View->ReInitializeViewControllers();

		m_View->Show();
	}

	UpdateWidget();
}

void mitk::wxMitkMultiRenderWindowConfig::SetActiveWidget( int i )
{
	if ( i < 0  )
	{
		m_cmbCurrentView->SetSelection( wxNOT_FOUND );
		m_ViewConfig->SetView( NULL );
		return;
	}

	// Check range of i
	wxMitkMultiRenderWindow::WidgetListType widgets;
	widgets = m_View->GetLayout()->GetSliceViews( );
	if ( i >= widgets.size() )
	{
		m_cmbCurrentView->SetSelection( wxNOT_FOUND );
		m_ViewConfig->SetView( NULL );
		return;
	}

	m_cmbCurrentView->SetSelection( i );

	SetActiveWidget( m_cmbCurrentView->GetStringSelection().c_str() );
}

void mitk::wxMitkMultiRenderWindowConfig::SetActiveWidget( std::string name )
{
	if ( !name.empty() )
	{
		wxMitkMultiRenderWindow::WidgetListType widgets;
		widgets = m_View->GetLayout()->GetSliceViews( );
		wxMitkMultiRenderWindow::WidgetListType::iterator it;
		for ( it = widgets.begin() ; it != widgets.end() ; it++ )
		{
			if ( name == (*it)->GetName( ) )
			{
				m_ViewConfig->SetView( (*it) );
				m_cmbCurrentView->SetStringSelection( name );
			}
		}

	}
}

