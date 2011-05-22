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

#include "wxMitkColorGradientCanvas.h"
#include "vtkColorTransferFunction.h"

using namespace mitk;

//!
wxMitkColorGradientCanvas::wxMitkColorGradientCanvas() : wxMitkPaintableInSetCanvas(), colorFunc(NULL)
{
}

//!
wxMitkColorGradientCanvas::~wxMitkColorGradientCanvas(void)
{
}

/**
\return inSetX, inSetY by reference as the horizontal and vertical inset borders
\return width, height by reference being the width and height of the rectangle area, starting from the point (inSetX, inSetY)
*/
void wxMitkColorGradientCanvas::GetGradientColourArea(int& inSetX, int& inSetY, int& width, int& height) const
{
	this->wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder(inSetX, inSetY, width, height);
}

/**
\return a rectangle holding the area where the gradient is drawn
*/
const wxRect wxMitkColorGradientCanvas::GetGradientColourArea(void) const
{
	return this->wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder();
}

//!
void wxMitkColorGradientCanvas::SetColorTransferFunction(vtkColorTransferFunction* func)
{
	this->colorFunc = func;
}

//!
vtkColorTransferFunction* wxMitkColorGradientCanvas::GetColorTransferFunction(void) const
{
	return this->colorFunc;
}


//!
void wxMitkColorGradientCanvas::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	// Draw the parent control on the temporary canvas, as background for this child control
	this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);

	// Draw the histogram
	this->DrawLinearGradient(Dc);
}

//!
void wxMitkColorGradientCanvas::DrawLinearGradient(wxDC& Dc)
{
	int x0, y0, width, height;
	double min, max;
	double funcPoint;
	this->GetGradientColourArea(x0, y0, width, height);

	if (this->colorFunc != NULL) 
	{
		min = this->colorFunc->GetRange()[0];
		max = this->colorFunc->GetRange()[1];

		for (int canvasPoint = 0; canvasPoint < width; canvasPoint++)
		{
			// Linear interpolation (mapping) of the function range over the canvas width   
			funcPoint = min + ((double)canvasPoint) * (max - min) / width; 
			// Map the function color to the canvasPoint
			wxColor color(
				(int)(this->colorFunc->GetRedValue(funcPoint) * 255.0), 
				(int)(this->colorFunc->GetGreenValue(funcPoint) * 255.0),
				(int)(this->colorFunc->GetBlueValue(funcPoint) * 255.0)
				);
			// Draw a vertical line with that colour
			Dc.SetPen(wxPen(color, 1, wxSOLID));
			Dc.DrawLine(x0 + canvasPoint, y0, x0 + canvasPoint, height + y0);
		}
		Dc.SetPen(wxNullPen);
	}
}


