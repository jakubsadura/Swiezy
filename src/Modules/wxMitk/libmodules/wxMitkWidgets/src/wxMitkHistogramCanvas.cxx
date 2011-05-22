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

#include "wxMitkHistogramCanvas.h"

using namespace mitk;


//!
wxMitkHistogramCanvas::wxMitkHistogramCanvas(void) : wxMitkPaintableInSetCanvas(), histogram(NULL)
{
	this->histogramFgColor = wxColour( 47, 47, 47 );
}

//!
wxMitkHistogramCanvas::~wxMitkHistogramCanvas(void)
{
}

//!
void wxMitkHistogramCanvas::SetHistogram(const mitk::HistogramGenerator::HistogramType* histogram)
{
	this->histogram = histogram;
}

/**
Sets the colour for histogram to be painted in the canvas
*/
void wxMitkHistogramCanvas::SetHistogramColor(const wxColour& colour)
{
	this->histogramFgColor = colour;
}

//!
const wxColour& wxMitkHistogramCanvas::GetHistogramColor(void) const
{
	return this->histogramFgColor;
}

/**
\return inSetX, inSetY by reference as the horizontal and vertical inset borders
\return width, height by reference being the width and height of the rectangle area, starting from the point (inSetX, inSetY)
*/
void wxMitkHistogramCanvas::GetHistogramArea(int& inSetX, int& inSetY, int& width, int& height) const
{
	this->wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder(inSetX, inSetY, width, height);
}

/**
\return a rectangle holding the area where the histogram is drawn
*/
const wxRect wxMitkHistogramCanvas::GetHistogramArea(void) const
{
	return this->wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder();
}


//!
void wxMitkHistogramCanvas::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	// Draw the parent control on the temporary canvas, as background for this child control
	this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);

	// Draw the histogram
	this->DrawHistogram(Dc);
}

//!
void wxMitkHistogramCanvas::DrawHistogram(wxDC& Dc)
{
	float scaleFactor;
	float maxFreqLog;
	int tfIndex;
	float freq;
	int top;

	int x0, y0, width, height;
	this->GetHistogramArea(x0, y0, width, height);

	if(this->histogram.IsNotNull())
	{
		Dc.SetPen(wxPen(this->histogramFgColor, 1, wxSOLID));

		scaleFactor = (float)(this->histogram->GetSize()[0]) / width;
		maxFreqLog = vcl_log(mitk::HistogramGenerator::CalculateMaximumFrequency(this->histogram));

		// For each frequency band, draw a line (starting from +1 not to paint over the frame)
		for (int x = x0; x < width; x++)
		{
			tfIndex = x * scaleFactor;
			freq = this->histogram->GetFrequency(tfIndex);
			
			if (freq > 0)
			{
				top = static_cast<int>((1 - vcl_log(freq) / maxFreqLog) * height);      
				Dc.DrawLine(x, height + y0, x, top + y0); // -1, and +1 are to not paint over the borders of the frame
			}
		}
		// Restore the brush
		Dc.SetPen(wxNullPen);
	}
}


