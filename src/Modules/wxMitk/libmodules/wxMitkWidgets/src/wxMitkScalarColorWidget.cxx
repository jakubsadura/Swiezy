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

#include "wxMitkScalarColorWidget.h"
#include "wxMitkColorFunctionControl.h"
#include "wxMitkColorGradientControl.h"
#include <vtkColorTransferFunction.h>
#include <sstream>
#include <wxUnicode.h>

using namespace mitk;

#define ID_wxMitkScalarColorWidget_Function (1 + wxID_HIGHEST)
#define ID_wxMitkScalarColorWidget_Gradient (2 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxMitkScalarColorWidget, wxPanel)
	EVT_COLORFUNCTION_CHANGED	(ID_wxMitkScalarColorWidget_Function, wxMitkScalarColorWidget::OnColorFunctionChanged)
	EVT_MOUSEOVERHISTOGRAM		(ID_wxMitkScalarColorWidget_Function, wxMitkScalarColorWidget::OnMousePositionChangedOverHistogram)
	EVT_MOUSEOVERHISTOGRAM		(ID_wxMitkScalarColorWidget_Gradient, wxMitkScalarColorWidget::OnMousePositionChangedOverHistogram)
END_EVENT_TABLE()

//!
wxMitkScalarColorWidget::wxMitkScalarColorWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
: wxPanel(parent, id, pos, size, style, name)
{
	this->SetId(wxID_ANY);

	wxStaticText* lblTitle = new wxStaticText(this, wxID_ANY, wxT("Scalar color mapping"));
	this->lblRange = new wxStaticText(this, wxID_ANY, wxT("Range: [-]"));
	this->lblScalar = new wxStaticText(this, wxID_ANY, wxT("Scalar: -  R(-)G(-)B(-)"));
	
	this->colorFunctionControl = new wxMitkColorFunctionControl(this, ID_wxMitkScalarColorWidget_Function);
	this->colorGradientControl = new wxMitkColorGradientControl(this, ID_wxMitkScalarColorWidget_Gradient);
	
	// Fix their sizes
	this->colorFunctionControl->SetMaxSize(wxSize(0, 64));
	this->colorFunctionControl->SetMinSize(wxSize(0, 64));
	this->colorGradientControl->SetMaxSize(wxSize(0, 24));
	this->colorGradientControl->SetMinSize(wxSize(0, 24));

	// Apply some style
	wxFont minorFont = this->lblRange->GetFont();
	minorFont.SetPointSize(7);
	this->lblRange->SetFont(minorFont);
	this->lblScalar->SetFont(minorFont);

	// Layout the labels
	wxBoxSizer* labelLayout = new wxBoxSizer(wxHORIZONTAL);
	labelLayout->Add(this->lblRange, 0, wxEXPAND);
	labelLayout->AddSpacer(20);
	labelLayout->Add(this->lblScalar, 0, wxEXPAND);

	// Layout the canvases
	wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
	layout->Add(lblTitle);
	layout->Add(this->colorFunctionControl, 0, wxEXPAND);
	layout->Add(this->colorGradientControl, 0, wxEXPAND);
	layout->Add(labelLayout, 0, wxEXPAND);
	this->SetSizer(layout);
}

//!
wxMitkScalarColorWidget::~wxMitkScalarColorWidget(void)
{
}

//!
void wxMitkScalarColorWidget::SetColorTransferFunction(vtkColorTransferFunction* func)
{
	this->colorFunctionControl->SetColorTransferFunction(func);
	this->colorGradientControl->SetColorTransferFunction(func);
}


//!
void wxMitkScalarColorWidget::ResetWidget(void)
{
	this->SetColorTransferFunction(NULL);
	this->SetHistogram(NULL);
}

//!
vtkColorTransferFunction* wxMitkScalarColorWidget::GetColorTransferFunction(void) const
{
	return this->colorGradientControl->GetColorTransferFunction();
}


//!
void wxMitkScalarColorWidget::SetHistogram(const mitk::HistogramGenerator::HistogramType* histogram)
{
	double rangeMin, rangeMax;
	std::ostringstream rangeStream;
	if(histogram != NULL)
	{
		// Assign the range to the label
		rangeMin = histogram->GetBinMin(0, 0);
		rangeMax = histogram->GetBinMax(0, histogram->Size() - 1);
		rangeStream << "Range: [" << rangeMin << "," << rangeMax << "]";
		this->lblRange->SetLabel(_U(rangeStream.str()));
	}
	else
	{
		rangeMin = 0.0;
		rangeMax = 1.0;
		this->lblRange->SetLabel(wxT("Range: [-]"));
	}
	// Set the histogram so as to paint it
	this->colorFunctionControl->SetHistogram(histogram);
}

/**
Sets the colour for histogram to be painted in the canvas
*/
void wxMitkScalarColorWidget::SetHistogramColor(const wxColour& colour)
{
	this->colorFunctionControl->SetHistogramColor(colour);
}

//!
const wxColour& wxMitkScalarColorWidget::GetHistogramColor(void) const
{
	return this->colorFunctionControl->GetHistogramColor();
}

//!
void wxMitkScalarColorWidget::OnColorFunctionChanged(wxMitkColorFunctionChangedEvent& event)
{
	vtkColorTransferFunction* func = event.GetColorTransferFunction();
	// The transfer function has been changed from the wxMitkColorFunctionControl
	// We update the other controls
	if(func != NULL)
	{
		this->colorGradientControl->Refresh();
	}
	event.Skip();
}

/**
Displays the color function values under the mouse position when it is inside the histogram ara (the client
area of the wxMitkColorFunctionControl).
\sa wxMitkColorFunctionControl
*/
void wxMitkScalarColorWidget::OnMousePositionChangedOverHistogram(wxMitkMouseOverHistogramEvent& event)
{
	char valueString[128];
	double rgb[3];
	
	if(event.ValuesFromColorFunction())
	{
		// Assign the scalar and color to the label
		event.GetRGBValue(rgb);
		sprintf(valueString, "Scalar: %.4f  R(%.2f)G(%.2f)B(%.2f)", event.GetFunctionPoint(), rgb[0], rgb[1], rgb[2]);
		this->lblScalar->SetLabel(_U(valueString));
	}
	event.Skip();
}


/**
Enables or disables the Interactive apply. When interactive apply is on, any mouse interaction updates the
transferfunction and the rendering inmediately. Otherwise, it only is updated when the user releases the mouse
(finishes the interaction)
*/
void wxMitkScalarColorWidget::SetInteractiveApply(bool enable)
{
	this->colorFunctionControl->SetInteractiveApply(enable);
}



