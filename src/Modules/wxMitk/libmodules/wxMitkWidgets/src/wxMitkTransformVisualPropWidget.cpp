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

#include "wxMitkTransformVisualPropWidget.h"

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkTransformVisualPropWidget, wxMitkTransformVisualPropWidgetUI)
END_EVENT_TABLE();


//!
wxMitkTransformVisualPropWidget::wxMitkTransformVisualPropWidget(
	wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkTransformVisualPropWidgetUI(parent, id, pos, size, style)
{
}

//!
wxMitkTransformVisualPropWidget::~wxMitkTransformVisualPropWidget(void)
{
}

//!
void wxMitkTransformVisualPropWidget::SetDataTreeNode(mitk::DataTreeNode::Pointer node)
{
	m_node = node;
	if ( m_node.IsNull() )
	{
		Enable( false );
		return;
	}

	Enable( true );

	UpdateWidget( );
}


void mitk::wxMitkTransformVisualPropWidget::OnSpacingX( wxCommandEvent &event )
{
	double value;
	m_txtSpacingX->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridSpacingX", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::OnSpacingY( wxCommandEvent &event )
{
	double value;
	m_txtSpacingY->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridSpacingY", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::OnSpacingZ( wxCommandEvent &event )
{
	double value;
	m_txtSpacingZ->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridSpacingZ", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::OnOffsetX( wxCommandEvent &event )
{
	double value;
	m_txtOffsetX->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridOffsetX", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::OnOffsetY( wxCommandEvent &event )
{
	double value;
	m_txtOffsetY->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridOffsetY", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::OnOffsetZ( wxCommandEvent &event )
{
	double value;
	m_txtOffsetZ->GetValue().ToDouble( &value );
	m_node->ReplaceProperty( "gridOffsetZ", mitk::FloatProperty::New( value ) );
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );
}

void mitk::wxMitkTransformVisualPropWidget::UpdateWidget()
{
	mitk::Vector3D gridSpacing;
	gridSpacing[ 0 ] = 0.5;
	gridSpacing[ 1 ] = 0.5;
	gridSpacing[ 2 ] = 0.5;
	mitk::Vector3D gridOffset;
	gridOffset[ 0 ] = 0;
	gridOffset[ 1 ] = 0;
	gridOffset[ 2 ] = 0;

	if ( m_node.IsNotNull() )
	{
		m_node->GetFloatProperty( "gridSpacingX", gridSpacing[ 0 ] );
		m_node->GetFloatProperty( "gridSpacingY", gridSpacing[ 1 ] );
		m_node->GetFloatProperty( "gridSpacingZ", gridSpacing[ 2 ] );
		m_node->GetFloatProperty( "gridOffsetX", gridOffset[ 0 ] );
		m_node->GetFloatProperty( "gridOffsetY", gridOffset[ 1 ] );
		m_node->GetFloatProperty( "gridOffsetZ", gridOffset[ 2 ] );
	}

	m_txtSpacingX->SetValue( wxString::Format( "%.2f", gridSpacing[ 0 ] ) );
	m_txtSpacingY->SetValue( wxString::Format( "%.2f", gridSpacing[ 1 ] ) );
	m_txtSpacingZ->SetValue( wxString::Format( "%.2f", gridSpacing[ 2 ] ) );
	m_txtOffsetX->SetValue( wxString::Format( "%.2f", gridOffset[ 0 ] ) );
	m_txtOffsetY->SetValue( wxString::Format( "%.2f", gridOffset[ 1 ] ) );
	m_txtOffsetZ->SetValue( wxString::Format( "%.2f", gridOffset[ 2 ] ) );
}
