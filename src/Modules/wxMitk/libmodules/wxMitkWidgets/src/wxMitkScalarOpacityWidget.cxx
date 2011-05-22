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

#include "wxMitkScalarOpacityWidget.h"
#include "wxMitkScalarOpacityControl.h"
#include <vtkPiecewiseFunction.h>
#include <sstream>
#include <wxUnicode.h>

using namespace mitk;

#define ID_wxMitkScalarOpacityControl (001 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxMitkScalarOpacityWidget, wxPanel)
	EVT_MOUSEOVERHISTOGRAM		(ID_wxMitkScalarOpacityControl, wxMitkScalarOpacityWidget::OnMousePositionChangedOverHistogram)
END_EVENT_TABLE()

//!
wxMitkScalarOpacityWidget::wxMitkScalarOpacityWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
: wxPanel(parent, id, pos, size, style, name)
{
	this->SetId(wxID_ANY);

	wxStaticText* lblTitle = new wxStaticText(this, wxID_ANY, wxT("Scalar opacity mapping"));
	this->lblRange = new wxStaticText(this, wxID_ANY, wxT("Range: [-]"));
	this->lblScalar = new wxStaticText(this, wxID_ANY, wxT("Scalar: -  Opacity(-)"));
	
	this->opacityFunctionControl = new wxMitkScalarOpacityControl(this, ID_wxMitkScalarOpacityControl);
	
	// Fix their sizes
	this->opacityFunctionControl->SetMaxSize(wxSize(0, 64));
	this->opacityFunctionControl->SetMinSize(wxSize(0, 64));


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
	layout->Add(this->opacityFunctionControl, 0, wxEXPAND);
	layout->Add(labelLayout, 0, wxEXPAND);
	this->SetSizer(layout);
}

//!
wxMitkScalarOpacityWidget::~wxMitkScalarOpacityWidget(void)
{
}

//!
void wxMitkScalarOpacityWidget::ResetWidget(void)
{
	this->SetPiecewiseFunction(NULL);
	this->SetHistogram(NULL);
}

//!
void wxMitkScalarOpacityWidget::SetPiecewiseFunction(vtkPiecewiseFunction* func)
{
	this->opacityFunctionControl->SetPiecewiseFunction(func);
}

//!
vtkPiecewiseFunction* wxMitkScalarOpacityWidget::GetPiecewiseFunction(void) const
{
	return this->opacityFunctionControl->GetPiecewiseFunction();
}


//!
void wxMitkScalarOpacityWidget::SetHistogram(const mitk::HistogramGenerator::HistogramType* histogram)
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
	this->opacityFunctionControl->SetHistogram(histogram);
}

/**
Sets the colour for histogram to be painted in the canvas
*/
void wxMitkScalarOpacityWidget::SetHistogramColor(const wxColour& colour)
{
	this->opacityFunctionControl->SetHistogramColor(colour);
}

//!
const wxColour& wxMitkScalarOpacityWidget::GetHistogramColor(void) const
{
	return this->opacityFunctionControl->GetHistogramColor();
}


/**
Displays the color function values under the mouse position when it is inside the histogram ara (the client
area of the wxMitkScalarOpacityControl).
\sa wxMitkScalarOpacityControl
*/
void wxMitkScalarOpacityWidget::OnMousePositionChangedOverHistogram(wxMitkMouseOverHistogramEvent& event)
{
	char valueString[128];

	if(event.ValuesFromOpacityFunction())
	{
		// Assign the scalar and color to the label
		sprintf(valueString, "Scalar: %.4f  Opacity(%.2f)", event.GetFunctionPoint(), event.GetOpacityValue());
		this->lblScalar->SetLabel(_U(valueString));
	}

	event.Skip();
}


/**
Enables or disables the Interactive apply. When interactive apply is on, any mouse interaction updates the
transferfunction and the rendering inmediately. Otherwise, it only is updated when the user releases the mouse
(finishes the interaction)
*/
void wxMitkScalarOpacityWidget::SetInteractiveApply(bool enable)
{
	this->opacityFunctionControl->SetInteractiveApply(enable);
}


