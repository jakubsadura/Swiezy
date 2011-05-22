/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wxMitkMaterialSettingsPanel.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkSphereViewCanvas.h"
#include "wxMitkSurfaceLightingControl.h"
#include "wxMitkSurfaceRepresentationControl.h"
#include "wxMitkScalarsArrayControl.h"
#include "wxMitkRenderWindow.h"
#include "wxID.h"
#include "mitkRenderingManager.h"
#include "vtkDataSetAttributes.h"
#include "vtkSmartPointer.h"

namespace mitk
{

DEFINE_EVENT_TYPE( wxEVT_MATERIALSETTINGS_INTERACTIVEAPPLY_CHANGED )
DEFINE_EVENT_TYPE( wxEVT_MATERIALSETTINGS_INTERACTIVEAPPLY_PRESSED )
DEFINE_EVENT_TYPE( wxEVT_MATERIALSETTINGS_REPRESENTATION_CHANGED )

const wxWindowID wxMitkMaterialSettingsPanel::wxID_representation = wxNewId();
const wxWindowID wxMitkMaterialSettingsPanel::wxID_lighting = wxNewId();

BEGIN_EVENT_TABLE( wxMitkMaterialSettingsPanel, wxPanel )
  EVT_WIDGET_UPDATE( wxID_lighting, wxMitkMaterialSettingsPanel::OnLightingChange )
  EVT_WIDGET_UPDATE( wxID_representation, wxMitkMaterialSettingsPanel::OnRepresentationChange )
END_EVENT_TABLE()

//!
wxMitkMaterialSettingsPanel::wxMitkMaterialSettingsPanel( wxWindow *parent,
	wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
: wxPanel( parent, winid, pos, size, style, name )
{
	m_lighting              = new wxMitkSurfaceLightingControl( this, wxID_lighting );
	m_representation        = new wxMitkSurfaceRepresentationControl( this, wxID_representation );

	m_MaterialProperty      = NULL;

	m_vLayoutLighting = new wxBoxSizer( wxVERTICAL );

	m_pInteractiveApplySizer = new wxBoxSizer( wxVERTICAL );
	m_SphereAndCoSizer = new wxBoxSizer( wxHORIZONTAL );
	m_SphereAndCoSizer->AddSpacer( 8 );
	m_SphereAndCoSizer->Add( m_pInteractiveApplySizer );

	m_vLayoutLighting->AddSpacer( 20 );
	m_vLayoutLighting->Add( m_SphereAndCoSizer );

	m_vLayoutLighting->AddSpacer( 8 );
	m_vLayoutLighting->Add( m_lighting, 0, wxEXPAND | wxALL, 4 );
	m_vLayoutLighting->AddSpacer( 8 );
	m_vLayoutLighting->Add( m_representation, 0, wxEXPAND | wxALL, 4 );
	m_vLayoutLighting->AddSpacer( 20 );


	SetSizer( m_vLayoutLighting );

	for ( int i = 0 ; i < WXMITKMATERIALSETTINGSPANEL_MAX_CONTROLS ; i++ )
	{
		m_ControlsEnableStatus[ i ] = true;
	}
}

wxMitkMaterialSettingsPanel::~wxMitkMaterialSettingsPanel()
{

}

void wxMitkMaterialSettingsPanel::OnLightingChange( wxMitkWidgetUpdateEvent& WXUNUSED(event) )
{
	UpdateMaterial();
}

void wxMitkMaterialSettingsPanel::OnRepresentationChange( wxMitkWidgetUpdateEvent& WXUNUSED(event) )
{
	if ( m_MaterialProperty.IsNotNull( ) )
	{
		m_MaterialProperty->SetRepresentation( m_representation->GetRepresentation() );
		m_MaterialProperty->SetLineWidth( m_representation->GetLineWidth() );
		m_MaterialProperty->SetPointSize( m_representation->GetPointSize() );

		m_representation->Fit();
		m_vLayoutLighting->Layout();

		wxCommandEvent ce;
		ce.SetEventType( wxEVT_MATERIALSETTINGS_REPRESENTATION_CHANGED );
		ProcessEvent( ce );
	}
}

void wxMitkMaterialSettingsPanel::UpdateMaterial()
{
	if ( m_MaterialProperty.IsNotNull( ) )
	{
		m_MaterialProperty->SetInterpolation( m_lighting->GetInterpolation() );

		float color[3];
		m_lighting->GetAmbientColor( color );
		m_MaterialProperty->SetAmbientColor( color[0], color[1], color[2] );
		m_MaterialProperty->SetAmbientCoefficient( m_lighting->GetAmbientCoefficient() );

		m_lighting->GetDiffuseColor( color );
		m_MaterialProperty->SetDiffuseColor( color[0], color[1], color[2] );
		m_MaterialProperty->SetDiffuseCoefficient( m_lighting->GetDiffuseCoefficient() );
		m_MaterialProperty->SetColor( color[0], color[1], color[2] );

		m_lighting->GetSpecularColor( color );
		m_MaterialProperty->SetSpecularColor( color[0], color[1], color[2] );
		m_MaterialProperty->SetSpecularCoefficient( m_lighting->GetSpecularCoefficient() );
		m_MaterialProperty->SetSpecularPower( m_lighting->GetSpecularPower() );

		m_MaterialProperty->SetOpacity( m_lighting->GetOpacity() );
	}
}

MaterialProperty::Pointer wxMitkMaterialSettingsPanel::GetMaterialProperty()
{
	return m_MaterialProperty;
}

void wxMitkMaterialSettingsPanel::DoAddEnabledControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl )
{
	switch( whichControl )
	{
		case LIGHTING_CONTROL:
		{
			break;
		}
		case SURFACE_REPRESENTATION_CONTROL:
		{
			break;
		}
	}
}

void wxMitkMaterialSettingsPanel::DoRemoveDisabledControl( wxWindow* pControl, wxSizer* pSizer )
{
	if( !pSizer )
	{
		m_vLayoutLighting->Detach( pControl );
	}
	RemoveChild( pControl );
	pControl->Hide();
}

void wxMitkMaterialSettingsPanel::DoRemoveDisabledControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl )
{
	switch( whichControl )
	{
		case LIGHTING_CONTROL:
		{
			DoRemoveDisabledControl( m_lighting );
			break;
		}
		case SURFACE_REPRESENTATION_CONTROL:
		{
			DoRemoveDisabledControl( m_representation );
			break;
		}
	}
}

bool wxMitkMaterialSettingsPanel::EnableControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl, bool bEnable )
{
	bool previousStatus = m_ControlsEnableStatus[whichControl];
	m_ControlsEnableStatus[whichControl] = bEnable;
	if( previousStatus != bEnable )
	{
		if( bEnable )
		{
			DoAddEnabledControl( whichControl );
		}
		else
		{
			DoRemoveDisabledControl( whichControl );
		}
	}
	return previousStatus;
}

const wxMitkSurfaceLightingControl& wxMitkMaterialSettingsPanel::GetLightingControl() const
{
	return *m_lighting;
}

wxMitkSurfaceLightingControl* wxMitkMaterialSettingsPanel::GetLightingControlPointer()
{
	return m_lighting;
}

const wxMitkSurfaceRepresentationControl& wxMitkMaterialSettingsPanel::GetRepresentationControl() const
{
	return *m_representation;
}

void wxMitkMaterialSettingsPanel::SetMaterialProperty( MaterialProperty::Pointer material )
{
	m_MaterialProperty = material;
	m_lighting->SetMaterial( m_MaterialProperty );
	m_representation->SetMaterial( m_MaterialProperty );
}

}
