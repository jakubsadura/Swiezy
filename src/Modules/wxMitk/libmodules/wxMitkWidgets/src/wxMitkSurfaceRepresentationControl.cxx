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

#include "wxMitkSurfaceRepresentationControl.h"
#include "wxWidgetStackControl.h"
#include <wx/spinctrl.h>
#include "mitkRenderingManager.h"

using namespace mitk;

#define ID_cmbRepresentation (1 + wxID_HIGHEST)
#define ID_udcLineWidth (2 + wxID_HIGHEST)
#define ID_udcPointSize (3 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxMitkSurfaceRepresentationControl, wxPanel)
	EVT_CHOICE(ID_cmbRepresentation, wxMitkSurfaceRepresentationControl::OnRepresentationSelected)
	EVT_SPINCTRL(ID_udcLineWidth, wxMitkSurfaceRepresentationControl::OnLineWidthChange)
	EVT_SPINCTRL(ID_udcPointSize, wxMitkSurfaceRepresentationControl::OnPointSizeChange)
END_EVENT_TABLE()


//!
wxMitkSurfaceRepresentationControl::wxMitkSurfaceRepresentationControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name), currentRepresentation(mitk::MaterialProperty::Surface)
{
	// Generate the list of available representations
	wxString items[4];
	items[0] = wxT("Surface");
	items[1] = wxT("Wireframe");
	items[2] = wxT("Points");
	items[3] = wxT("Wireframe over Surface");

	// Create an empty panel for when selecting no representation
	this->emptyPanel = new wxPanel(this);
	wxStaticText* emptyLabel = new wxStaticText(this->emptyPanel, wxID_ANY, wxT("< The selected representation \nhas not aditional properties >"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	wxBoxSizer* emptySizer = new wxBoxSizer(wxHORIZONTAL);
	emptySizer->Add(emptyLabel, 1, wxEXPAND);
	this->emptyPanel->SetSizer(emptySizer);

	// Create the widgets
	this->widgetStack = new wxWidgetStackControl(this, wxID_ANY);
	this->wireframeOptions = new wxPanel(this->widgetStack, wxID_ANY);
	wxStaticBox* wireframeBox = new wxStaticBox(this->wireframeOptions, wxID_ANY, wxT("Wireframe representation options"));
	wxStaticText* lblLineWidth = new wxStaticText(this->wireframeOptions, wxID_ANY, wxT("Edge width"));
	this->udcLineWidth = new wxSpinCtrl(this->wireframeOptions, ID_udcLineWidth, wxT("2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 2);
	this->pointOptions = new wxPanel(this->widgetStack, wxID_ANY);
	wxStaticBox* pointBox = new wxStaticBox(this->pointOptions, wxID_ANY, wxT("Point representation options"));
	wxStaticText* labelPointSize = new wxStaticText(this->pointOptions, wxID_ANY, wxT("Point size"));
	this->udcPointSize = new wxSpinCtrl(this->pointOptions, ID_udcPointSize, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20, 2);

	this->cmbRepresentation = new wxChoice(this, ID_cmbRepresentation, wxDefaultPosition, wxDefaultSize, 4, items);
	
	// Mount the options frame for each representation type
	wxStaticBoxSizer* wireframeOptionsLayout = new wxStaticBoxSizer(wireframeBox, wxHORIZONTAL);
	wireframeOptionsLayout->Add(lblLineWidth);
	wireframeOptionsLayout->Add(this->udcLineWidth, 1, wxEXPAND);

	wxBoxSizer* wireframeSizer = new wxBoxSizer(wxVERTICAL);
	wireframeSizer->Add(wireframeOptionsLayout, 0, wxEXPAND);
	wireframeSizer->AddSpacer(8);
	this->wireframeOptions->SetSizer(wireframeSizer);

	wxStaticBoxSizer* pointOptionsLayout = new wxStaticBoxSizer(pointBox, wxHORIZONTAL);
	pointOptionsLayout->Add(labelPointSize);
	pointOptionsLayout->Add(this->udcPointSize, 1, wxEXPAND);

	wxBoxSizer* pointSizer = new wxBoxSizer(wxVERTICAL);
	pointSizer->Add(pointOptionsLayout, 0, wxEXPAND);
	pointSizer->AddSpacer(8);
	this->pointOptions->SetSizer(pointSizer);

	// Place options in the widget stack
	this->widgetStack->Add(this->emptyPanel);
	this->widgetStack->Add(this->wireframeOptions);
	this->widgetStack->Add(this->pointOptions);
	this->widgetStack->Raise(this->emptyPanel);

	// Layout items
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(this->cmbRepresentation, 0, wxEXPAND);
	sizer->Add(this->widgetStack, 1, wxEXPAND);
	this->SetSizer(sizer);

}


//!
wxMitkSurfaceRepresentationControl::~wxMitkSurfaceRepresentationControl(void)
{
}


//!
void wxMitkSurfaceRepresentationControl::SetMaterial(MaterialProperty::Pointer material)
{
	if(material.IsNotNull())
	{
		this->SetRepresentation(material->GetRepresentation());
		this->udcLineWidth->SetValue((int)material->GetLineWidth());
	    this->udcPointSize->SetValue((int)material->GetPointSize());
	}
}

//!
void wxMitkSurfaceRepresentationControl::SetRepresentation(wxMitkSurfaceRepresentationControl::RepresentationType representation)
{
	switch(representation)
	{
	case mitk::MaterialProperty::Surface:
		this->cmbRepresentation->Select(0); 
		this->widgetStack->Raise(this->emptyPanel);
		break;
	case mitk::MaterialProperty::Wireframe: 
		this->cmbRepresentation->Select(1); 
		this->widgetStack->Raise(this->wireframeOptions);
		break;
	case mitk::MaterialProperty::Points:
		this->cmbRepresentation->Select(2); 
		this->widgetStack->Raise(this->pointOptions); 
		break;
	case mitk::MaterialProperty::WireframeSurface:
		this->cmbRepresentation->Select(3); 
		this->widgetStack->Raise(this->emptyPanel);
		break;
	default: 
		this->widgetStack->Raise(this->emptyPanel); 
		break;
	}
}

//!
void wxMitkSurfaceRepresentationControl::SetRepresentationIndex(int index)
{
	this->cmbRepresentation->Select(index);
	switch(index)
	{
	case 0: 
		this->widgetStack->Raise(this->emptyPanel); 
		this->currentRepresentation = mitk::MaterialProperty::Surface; 
		break;
	case 1: 
		this->widgetStack->Raise(this->wireframeOptions); 
		this->currentRepresentation = mitk::MaterialProperty::Wireframe; 
		break;
	case 2: 
		this->widgetStack->Raise(this->pointOptions); 
		this->currentRepresentation = mitk::MaterialProperty::Points; 
		break;
	case 3: 
		this->widgetStack->Raise(this->emptyPanel); 
		this->currentRepresentation = mitk::MaterialProperty::WireframeSurface; 
		break;
	default:
		this->widgetStack->Raise(this->emptyPanel); 
		break;
	}
}

/**
Returns the currents selected representation type.
If nothing was selected, returns NONE.
*/
wxMitkSurfaceRepresentationControl::RepresentationType wxMitkSurfaceRepresentationControl::GetRepresentation(void) const
{
	return this->currentRepresentation;
}

/**
Returns the currents selected representation type.
If nothing was selected, returns wxNOT_FOUND.
*/
int wxMitkSurfaceRepresentationControl::GetRepresentationIndex(void) const
{
	return this->cmbRepresentation->GetSelection();
}

/**
Returns the currents selected representation type.
If nothing was selected, returns NONE.
*/
float wxMitkSurfaceRepresentationControl::GetLineWidth(void) const
{
	return (float)(this->udcLineWidth->GetValue() * 1.0f);
}

float wxMitkSurfaceRepresentationControl::GetPointSize(void) const
{
	return (float)(this->udcPointSize->GetValue() * 1.0f);
}


//!
void wxMitkSurfaceRepresentationControl::OnRepresentationSelected(wxCommandEvent& WXUNUSED(event))
{ 
	this->SetRepresentationIndex(this->cmbRepresentation->GetSelection());
	// Create a command event and configure it
	wxMitkWidgetUpdateEvent eventUpdate(this->GetId());
	eventUpdate.SetEventObject(this);
	// Cast the event
	this->GetEventHandler()->ProcessEvent(eventUpdate);
}

//!
void wxMitkSurfaceRepresentationControl::OnLineWidthChange(wxSpinEvent& WXUNUSED(event))
{
	// Create a command event and configure it
	wxMitkWidgetUpdateEvent eventUpdate(this->GetId());
	eventUpdate.SetEventObject(this);
	// Cast the event
	this->GetEventHandler()->ProcessEvent(eventUpdate);
}

//!
void wxMitkSurfaceRepresentationControl::OnPointSizeChange(wxSpinEvent& WXUNUSED(event))
{
	// Create a command event and configure it
	wxMitkWidgetUpdateEvent eventUpdate(this->GetId());
	eventUpdate.SetEventObject(this);
	// Cast the event
	this->GetEventHandler()->ProcessEvent(eventUpdate);
}
