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

#include "wxMitkPaintableInSetCanvas.h"


using namespace mitk;

//!
wxMitkPaintableInSetCanvas::wxMitkPaintableInSetCanvas(int inSetX, int inSetY, int lineWidth)
: inSetBorderX(inSetX), inSetBorderY(inSetY), borderLineWidth(lineWidth)
{
	this->frameFgColor = wxColour( 0, 0, 0 );
	this->frameBgColor = wxColour( 192, 192, 192 );
}

//!
wxMitkPaintableInSetCanvas::~wxMitkPaintableInSetCanvas(void)
{
}


/**
Sets the background colour for the canvas
*/
void wxMitkPaintableInSetCanvas::SetFrameBackgroundColor(const wxColour& colour)
{
	this->frameBgColor = colour;
}

//!
const wxColour& wxMitkPaintableInSetCanvas::GetFrameBackgroundColor(void) const
{
	return this->frameBgColor;
}

/**
Sets the colour for the border of the frame
*/
void wxMitkPaintableInSetCanvas::SetFrameForegroundColor(const wxColour& colour)
{
	this->frameFgColor = colour;
}

//!
const wxColour& wxMitkPaintableInSetCanvas::GetFrameForegroundColor(void) const
{
	return this->frameFgColor;
}

//!
void wxMitkPaintableInSetCanvas::GetInSetMargin(int& inSetX, int& inSetY) const
{
	inSetX = this->inSetBorderX;
	inSetY = this->inSetBorderY;
}

/**
It is used to find out the paintable area WITH borders included
\return inSetX, inSetY by reference as the horizontal and vertical inset borders
\return width, height by reference being the width and height of the rectangle area, starting from the point (inSetX, inSetY)
*/
void wxMitkPaintableInSetCanvas::GetInSetFrameArea(int& inSetX, int& inSetY, int& width, int& height) const
{
	int clientWidth;
	int clientHeight;
	this->GetClientSize(&clientWidth, &clientHeight);
	inSetX = this->inSetBorderX;
	inSetY =  this->inSetBorderY;
	width = clientWidth - this->inSetBorderX * 2;
	height = clientHeight - this->inSetBorderY * 2;
}

/**
It is used to find out the paintable area WITH borders included
*/
const wxRect wxMitkPaintableInSetCanvas::GetInSetFrameArea(void) const
{
	int inSetX, inSetY, width, height;
	this->GetInSetFrameArea(inSetX, inSetY, width, height);
	return wxRect(inSetX, inSetY, width, height);
}

/**
It is used to find out the paintable area WITHOUT borders
\return inSetX, inSetY by reference as the horizontal and vertical inset borders
\return width, height by reference being the width and height of the rectangle area, starting from the point (inSetX, inSetY)
*/
void wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder(int& inSetX, int& inSetY, int& width, int& height) const
{
	int clientWidth;
	int clientHeight;
	this->GetClientSize(&clientWidth, &clientHeight);
	inSetX = this->inSetBorderX + this->borderLineWidth;
	inSetY =  this->inSetBorderY + this->borderLineWidth;
	width = clientWidth - this->inSetBorderX * 2 - this->borderLineWidth;
	height = clientHeight - this->inSetBorderY * 2 - this->borderLineWidth;
}

/**
It is used to find out the paintable area WITHOUT borders 
*/
const wxRect wxMitkPaintableInSetCanvas::GetInSetFrameAreaWithoutBorder(void) const
{
	int inSetX, inSetY, width, height;
	this->GetInSetFrameAreaWithoutBorder(inSetX, inSetY, width, height);
	return wxRect(inSetX, inSetY, width, height);
}

//!
void wxMitkPaintableInSetCanvas::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	int x0, y0, inSetWidth, inSetHeight;
	int width, height;

	// Get current widget area
    this->GetClientSize(&width, &height);

	// Draw the frame background
	this->GetInSetFrameArea(x0, y0, inSetWidth, inSetHeight);

	// Draw the frame
	if(!foreGroundOnly)
	{
		Dc.SetPen(wxPen(this->GetBackgroundColour(), 0, wxSOLID));
		Dc.SetBrush(wxBrush(this->GetBackgroundColour(), wxSOLID));
		Dc.DrawRectangle(wxRect(0, 0, width, height));
		Dc.SetPen(wxNullPen);
	}
	wxPoint points[5] = {wxPoint(0, 0), wxPoint(0, inSetHeight), wxPoint(inSetWidth, inSetHeight), wxPoint(inSetWidth, 0), wxPoint(0, 0)};
	Dc.SetPen(wxPen(this->frameFgColor, this->borderLineWidth, wxSOLID));
	Dc.SetBrush(wxBrush(this->frameBgColor, wxSOLID));
    Dc.DrawLines(5, points, x0 + this->borderLineWidth / 2, y0 + this->borderLineWidth / 2);
    Dc.SetBrush(wxNullBrush);
	Dc.SetPen(wxNullPen);
}



