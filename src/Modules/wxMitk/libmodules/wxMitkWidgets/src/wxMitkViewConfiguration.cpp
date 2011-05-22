/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkViewConfiguration.h"
#include "wxMitkSelectableGLWidget.h"

mitk::wxMitkViewConfiguration::wxMitkViewConfiguration(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxMitkViewConfigurationUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_ObserverTag = 0;
	m_View = 0;
}

mitk::wxMitkViewConfiguration::~wxMitkViewConfiguration()
{
	// Don't remove observer because m_View is destroyed 
}

void mitk::wxMitkViewConfiguration::OnDirection(wxCommandEvent &event)
{
	if ( m_View )
	{
		mitk::SliceNavigationController::Pointer navigationController;
		navigationController = m_View->GetSliceNavigationController( );
		navigationController->SetViewDirection( this->GetDirection( ) );
		navigationController->Update( );
	}

	event.Skip();
}


void mitk::wxMitkViewConfiguration::OnFlip(wxCommandEvent &event)
{
	if ( m_View )
	{
		mitk::SliceNavigationController::Pointer navigationController;
		navigationController = m_View->GetSliceNavigationController( );
		navigationController->SetFrontSide( this->GetFlip() );
		navigationController->Update( );
	}

	event.Skip();
}


void mitk::wxMitkViewConfiguration::OnRotate(wxCommandEvent &event)
{
	if ( m_View )
	{
		// When SetRotated( ) is called, the widget will be updated
		// so we need to store the local value
		mitk::ScalarType angle = this->GetAngleOfRotation();
		mitk::SliceNavigationController::Pointer navigationController;
		navigationController = m_View->GetSliceNavigationController( );
		navigationController->SetRotated( angle != 0 );
		navigationController->SetAngleOfRotation( angle );
		navigationController->Update( );
	}

	event.Skip();
}

void mitk::wxMitkViewConfiguration::SetView( mitk::wxMitkSelectableGLWidget* view )
{
	if ( m_View )
	{
		m_View->GetSliceNavigationController()->RemoveObserver( m_ObserverTag );
	}

	m_View = view;

	if ( m_View )
	{
		itk::SimpleMemberCommand<wxMitkViewConfiguration>::Pointer command;
		command = itk::SimpleMemberCommand<wxMitkViewConfiguration>::New();
		command->SetCallbackFunction(this, &wxMitkViewConfiguration::UpdateWidget);
		m_ObserverTag = m_View->GetSliceNavigationController()->AddObserver( 
			itk::ModifiedEvent( ), command );

		UpdateWidget( );
	}
}

void mitk::wxMitkViewConfiguration::UpdateWidget( )
{
	if ( !m_View )
	{
		return;
	}

	mitk::SliceNavigationController::Pointer navigationController;
	navigationController = m_View->GetSliceNavigationController( );

	mitk::SliceNavigationController::ViewDirection direction;
	direction = navigationController->GetViewDirection(  );
	m_cmbDirection->SetSelection( direction );

	m_chkFlip->SetValue( navigationController->GetFrontSide( ) );

	int iAngle = 0;
	if ( navigationController->GetRotated( ) )
	{
		if ( navigationController->GetAngleOfRotation( ) == 0 )
		{
			iAngle = 0;
		}
		else if ( navigationController->GetAngleOfRotation( ) == 90 )
		{
			iAngle = 1;
		}
		else if ( navigationController->GetAngleOfRotation( ) == 180 )
		{
			iAngle = 2;
		}
		else if ( navigationController->GetAngleOfRotation( ) == 270 )
		{
			iAngle = 3;
		}
	}

	m_rdbxRotate->SetSelection( iAngle );
}

bool mitk::wxMitkViewConfiguration::GetFlip()
{
	return m_chkFlip->GetValue( );
}

mitk::ScalarType mitk::wxMitkViewConfiguration::GetAngleOfRotation()
{
	switch ( m_rdbxRotate->GetSelection( ) )
	{
	case 0:return 0;break;
	case 1:return 90;break;
	case 2:return 180;break;
	case 3:return 270;break;
	}

	return 0;
}

mitk::SliceNavigationController::ViewDirection mitk::wxMitkViewConfiguration::GetDirection()
{
	mitk::SliceNavigationController::ViewDirection direction;
	direction = mitk::SliceNavigationController::ViewDirection( m_cmbDirection->GetSelection( ) );
	return direction;
}

mitk::wxMitkSelectableGLWidget* mitk::wxMitkViewConfiguration::GetView()
{
	return m_View;
}

