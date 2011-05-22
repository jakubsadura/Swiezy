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

#include "wxMitkSurfaceLightingControl.h"
#include "wxMitkSliderDouble.h"
#include "wxMitkColorSelectorControl.h"
#include "mitkRenderingManager.h"

using namespace mitk;

#define ID_cmbInterpolation (1 + wxID_HIGHEST)
#define ID_sldDiffuse (2 + wxID_HIGHEST)
#define ID_sldSpecular (3 + wxID_HIGHEST)
#define ID_sldSpecularPower (4 + wxID_HIGHEST)
#define ID_sldOpacity (5 + wxID_HIGHEST)
#define ID_diffuseColorSelector (6 + wxID_HIGHEST)
#define ID_specularColorSelector (7 + wxID_HIGHEST)
#define ID_sldAmbient (8 + wxID_HIGHEST)
#define ID_ambientColorSelector (9 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxMitkSurfaceLightingControl, wxPanel)
	EVT_CHOICE(ID_cmbInterpolation, wxMitkSurfaceLightingControl::OnInterpolationSelected)
	EVT_SLIDER_CHANGE(ID_sldDiffuse, wxMitkSurfaceLightingControl::OnDiffuseCoefficientChange)
	EVT_SLIDER_CHANGE(ID_sldSpecular, wxMitkSurfaceLightingControl::OnSpecularCoefficientChange)
	EVT_SLIDER_CHANGE(ID_sldSpecularPower, wxMitkSurfaceLightingControl::OnSpecularPowerChange)
	EVT_SLIDER_CHANGE(ID_sldOpacity, wxMitkSurfaceLightingControl::OnOpacityCoefficientChange)
	EVT_COLOURPICKER_CHANGED(ID_diffuseColorSelector, wxMitkSurfaceLightingControl::OnDiffuseColorChange)
	EVT_COLOURPICKER_CHANGED(ID_specularColorSelector, wxMitkSurfaceLightingControl::OnSpecularColorChange)
	EVT_SLIDER_CHANGE(ID_sldAmbient, wxMitkSurfaceLightingControl::OnAmbientCoefficientChange)
	EVT_COLOURPICKER_CHANGED(ID_ambientColorSelector, wxMitkSurfaceLightingControl::OnAmbientColorChange)
END_EVENT_TABLE()


//!
wxMitkSurfaceLightingControl::wxMitkSurfaceLightingControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	// Generate the list of available representations
	wxString items[3];
	items[0] = wxT("Flat");
	items[1] = wxT("Phong");
	items[2] = wxT("Gouraud");
	
	// Create the widgets
	m_plblAmbientColor = new wxStaticText(this, wxID_ANY, wxT("Ambient colour"));
	this->ambientColorSelector = new wxMitkColorSelectorControl(this, ID_ambientColorSelector);
	m_plblAmbient = new wxStaticText(this, wxID_ANY, wxT("Ambient coefficient"));
	this->sldAmbient = new wxMitkSliderDouble(this, ID_sldAmbient);
	m_plblDiffuseColor = new wxStaticText(this, wxID_ANY, wxT("Diffuse colour"));
	this->diffuseColorSelector = new wxMitkColorSelectorControl(this, ID_diffuseColorSelector);
	m_plblDiffuse = new wxStaticText(this, wxID_ANY, wxT("Diffuse coefficient"));
	this->sldDiffuse = new wxMitkSliderDouble(this, ID_sldDiffuse);
	m_plblSpecularColor = new wxStaticText(this, wxID_ANY, wxT("Specular colour"));
	this->specularColorSelector = new wxMitkColorSelectorControl(this, ID_specularColorSelector);
	m_plblSpecular = new wxStaticText(this, wxID_ANY, wxT("Specular coefficient"));
	this->sldSpecular = new wxMitkSliderDouble(this, ID_sldSpecular);
	m_plblSpecularPower = new wxStaticText(this, wxID_ANY, wxT("Specular power"));
	this->sldSpecularPower = new wxMitkSliderDouble(this, ID_sldSpecularPower, 0.0, 100.0, 1000.0);
	m_plblOpacity = new wxStaticText(this, wxID_ANY, wxT("Opacity"));
	this->sldOpacity = new wxMitkSliderDouble(this, ID_sldOpacity);
	m_plblInterpolation = new wxStaticText(this, wxID_ANY, wxT("Lighting model"));
	this->cmbInterpolation = new wxChoice(this, ID_cmbInterpolation, wxDefaultPosition, wxDefaultSize, 3, items);
	
	// Assign the layout
	m_pGridSizer = new wxFlexGridSizer(2);
	m_pGridSizer->AddGrowableCol(1, 1);
	m_pGridSizer->SetFlexibleDirection(wxHORIZONTAL);
	m_pGridSizer->Add( m_plblAmbientColor );
	m_pGridSizer->Add(this->ambientColorSelector, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblAmbient );
	m_pGridSizer->Add(this->sldAmbient, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblDiffuseColor );
	m_pGridSizer->Add(this->diffuseColorSelector, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblDiffuse );
	m_pGridSizer->Add(this->sldDiffuse, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblSpecularColor );
	m_pGridSizer->Add(this->specularColorSelector, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblSpecular );
	m_pGridSizer->Add(this->sldSpecular, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblSpecularPower );
	m_pGridSizer->Add(this->sldSpecularPower, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblOpacity );
	m_pGridSizer->Add(this->sldOpacity, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	m_pGridSizer->Add( m_plblInterpolation );
	m_pGridSizer->Add(this->cmbInterpolation, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	this->SetSizer( m_pGridSizer );

	for ( int i = 0 ; i < MAX_CONTROLS ; i++ )
	{
		m_ControlsEnableStatus[ i ] = true;
	}
}

//!
wxMitkSurfaceLightingControl::~wxMitkSurfaceLightingControl(void)
{
}


//!
void wxMitkSurfaceLightingControl::SetMaterial(MaterialProperty::Pointer material)
{
	if(material.IsNotNull())
	{
		this->sldAmbient->SetValue((double)material->GetAmbientCoefficient());
		this->sldDiffuse->SetValue((double)material->GetDiffuseCoefficient());
		this->sldSpecular->SetValue((double)material->GetSpecularCoefficient());
		this->sldSpecularPower->SetValue((double)material->GetSpecularPower());
		this->sldOpacity->SetValue((double)material->GetOpacity());
		this->SetInterpolation(material->GetInterpolation());
		mitk::MaterialProperty::Color color = material->GetColor();
		double rgb[3];
		rgb[0] = color.GetRed();
		rgb[1] = color.GetGreen();
		rgb[2] = color.GetBlue();
		this->diffuseColorSelector->SetRGBColour(rgb);
		color = material->GetSpecularColor();
		rgb[0] = color.GetRed();
		rgb[1] = color.GetGreen();
		rgb[2] = color.GetBlue();
		this->specularColorSelector->SetRGBColour(rgb);
		color = material->GetAmbientColor();
		rgb[0] = color.GetRed();
		rgb[1] = color.GetGreen();
		rgb[2] = color.GetBlue();
		this->ambientColorSelector->SetRGBColour(rgb);
	}
}

//!
void wxMitkSurfaceLightingControl::SetInterpolation(wxMitkSurfaceLightingControl::InterpolationType interpolation)
{
	switch(interpolation)
	{
	case mitk::MaterialProperty::Flat:
		this->cmbInterpolation->Select(0); 
		break;
	case mitk::MaterialProperty::Phong: 
		this->cmbInterpolation->Select(1); 
		break;
	case mitk::MaterialProperty::Gouraud:
		this->cmbInterpolation->Select(2); 
		break;
	default: 
		break;
	}
}

//!
void wxMitkSurfaceLightingControl::SetInterpolationIndex(int index)
{
	this->cmbInterpolation->Select(index);
	switch(index)
	{
	case 0: 
		this->currentInterpolation = mitk::MaterialProperty::Flat; 
		break;
	case 1: 
		this->currentInterpolation = mitk::MaterialProperty::Phong; 
		break;
	case 2: 
		this->currentInterpolation = mitk::MaterialProperty::Gouraud; 
		break;
	default:
		break;
	}
}

/**
Returns the currents selected interpolation type.
*/
wxMitkSurfaceLightingControl::InterpolationType wxMitkSurfaceLightingControl::GetInterpolation(void)
{
	return this->currentInterpolation;
}

/**
Returns the currents selected representation type.
If nothing was selected, returns wxNOT_FOUND.
*/
int wxMitkSurfaceLightingControl::GetInterpolationIndex(void) const
{
	return this->cmbInterpolation->GetSelection();
}

//!
double wxMitkSurfaceLightingControl::GetAmbientCoefficient(void) const
{
	return this->sldAmbient->GetValue();
}

//!
double wxMitkSurfaceLightingControl::GetDiffuseCoefficient(void) const
{
	return this->sldDiffuse->GetValue();
}

//!
double wxMitkSurfaceLightingControl::GetSpecularCoefficient(void) const
{
	return this->sldSpecular->GetValue();
}

//!
double wxMitkSurfaceLightingControl::GetSpecularPower(void) const
{
	return this->sldSpecularPower->GetValue();
}

//!
double wxMitkSurfaceLightingControl::GetOpacity(void) const
{
	return this->sldOpacity->GetValue();
}

//!
void wxMitkSurfaceLightingControl::GetAmbientColor(float color[3]) const
{
	wxColour ambientColor = this->ambientColorSelector->GetColour();
	color[0] = ambientColor.Red()/255.0f;
	color[1] = ambientColor.Green()/255.0f;
	color[2] = ambientColor.Blue()/255.0f;
}

//!
void wxMitkSurfaceLightingControl::GetDiffuseColor(float color[3]) const
{
	wxColour diffuseColor = this->diffuseColorSelector->GetColour();
	color[0] = diffuseColor.Red()/255.0f;
	color[1] = diffuseColor.Green()/255.0f;
	color[2] = diffuseColor.Blue()/255.0f;
}

//
void wxMitkSurfaceLightingControl::GetSpecularColor(float color[3]) const
{
	wxColour specularColor = this->specularColorSelector->GetColour();
	color[0] = specularColor.Red()/255.0f;
	color[1] = specularColor.Green()/255.0f;
	color[2] = specularColor.Blue()/255.0f;
}

//!
void wxMitkSurfaceLightingControl::CastWidgetUpdateEvent(void)
{
	// Create a command event and configure it
	wxMitkWidgetUpdateEvent eventUpdate(this->GetId());
	eventUpdate.SetEventObject(this);
	// Cast the event
	this->GetEventHandler()->ProcessEvent(eventUpdate);
}

//!
void wxMitkSurfaceLightingControl::OnInterpolationSelected(wxCommandEvent& WXUNUSED(event))
{ 
	this->SetInterpolationIndex(this->cmbInterpolation->GetSelection());
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnDiffuseCoefficientChange(wxMitkSliderChangeEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnSpecularCoefficientChange(wxMitkSliderChangeEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnSpecularPowerChange(wxMitkSliderChangeEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnOpacityCoefficientChange(wxMitkSliderChangeEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnDiffuseColorChange(wxColourPickerEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}

//!
void wxMitkSurfaceLightingControl::OnSpecularColorChange(wxColourPickerEvent& WXUNUSED(event))
{
	this->CastWidgetUpdateEvent();
}




void mitk::wxMitkSurfaceLightingControl::OnAmbientCoefficientChange( wxMitkSliderChangeEvent& event )
{
	this->CastWidgetUpdateEvent();
}

void mitk::wxMitkSurfaceLightingControl::OnAmbientColorChange( wxColourPickerEvent& event )
{
	this->CastWidgetUpdateEvent();
}

// TBI (To Be Implemented)
void mitk::wxMitkSurfaceLightingControl::DoAddEnabledControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl )
{
	switch( whichControl )
	{
		case AMBIENT_COLOR_CONTROL:
		{
			break;
		}
		case AMBIENT_COEFFICIENT_CONTROL:
		{
			break;
		}
		case DIFFUSE_COLOR_CONTROL:
		{
			break;
		}
		case DIFFUSE_COEFFICIENT_CONTROL:
		{
			break;
		}
		case SPECULAR_COLOR_CONTROL:
		{
			break;
		}
		case SPECULAR_COEFFICIENT_CONTROL:
		{
			break;
		}
		case SPECULAR_POWER_CONTROL:
		{
			break;
		}
		case OPACITY_CONTROL:
		{
			break;
		}
		case LIGHTING_MODEL_CONTROL:
		{
			break;
		}
	}
}

void mitk::wxMitkSurfaceLightingControl::DoRemoveDisabledControl( wxWindow* pControl )
{
	m_pGridSizer->Detach( pControl );
	RemoveChild( pControl );
	pControl->Hide();
}

void mitk::wxMitkSurfaceLightingControl::DoRemoveDisabledControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl )
{
	switch( whichControl )
	{
		case AMBIENT_COLOR_CONTROL:
		{
			DoRemoveDisabledControl( m_plblAmbientColor );
			DoRemoveDisabledControl( this->ambientColorSelector );
			break;
		}
		case AMBIENT_COEFFICIENT_CONTROL:
		{
			DoRemoveDisabledControl( m_plblAmbient );
			DoRemoveDisabledControl( this->sldAmbient );
			break;
		}
		case DIFFUSE_COLOR_CONTROL:
		{
			DoRemoveDisabledControl( m_plblDiffuseColor );
			DoRemoveDisabledControl( this->diffuseColorSelector );
			break;
		}
		case DIFFUSE_COEFFICIENT_CONTROL:
		{
			DoRemoveDisabledControl( m_plblDiffuse );
			DoRemoveDisabledControl( this->sldDiffuse );
			break;
		}
		case SPECULAR_COLOR_CONTROL:
		{
			DoRemoveDisabledControl( m_plblSpecularColor );
			DoRemoveDisabledControl( this->specularColorSelector );
			break;
		}
		case SPECULAR_COEFFICIENT_CONTROL:
		{
			DoRemoveDisabledControl( m_plblSpecular );
			DoRemoveDisabledControl( this->sldSpecular );
			break;
		}
		case SPECULAR_POWER_CONTROL:
		{
			DoRemoveDisabledControl( m_plblSpecularPower );
			DoRemoveDisabledControl( this->sldSpecularPower );
			break;
		}
		case OPACITY_CONTROL:
		{
			DoRemoveDisabledControl( m_plblOpacity );
			DoRemoveDisabledControl( this->sldOpacity );
			break;
		}
		case LIGHTING_MODEL_CONTROL:
		{
			DoRemoveDisabledControl( m_plblInterpolation );
			DoRemoveDisabledControl( this->cmbInterpolation );
			break;
		}
	}
}

bool mitk::wxMitkSurfaceLightingControl::EnableControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl, bool bEnable )
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
