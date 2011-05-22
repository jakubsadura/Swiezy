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

#include "wxMitkVtkColorTransferFunctionCanvas.h"

#include <mitkRenderingManager.h>
#include <vtkColorTransferFunction.h>

using namespace mitk;


//!
wxMitkVtkColorTransferFunctionCanvas::wxMitkVtkColorTransferFunctionCanvas(void) 
: wxMitkPaintableInSetCanvas(), maxSquareDistanceToGrabHandle(16), selectedHandle(-1), colorFunc(NULL)
{

}

//!
wxMitkVtkColorTransferFunctionCanvas::~wxMitkVtkColorTransferFunctionCanvas(void)
{
}

//!
void wxMitkVtkColorTransferFunctionCanvas::SetColorTransferFunction(vtkColorTransferFunction* func)
{
	this->colorFunc = func;
}

//!
vtkColorTransferFunction* wxMitkVtkColorTransferFunctionCanvas::GetColorTransferFunction(void) const
{
	return this->colorFunc;
}


//!
void wxMitkVtkColorTransferFunctionCanvas::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	// Draw the parent control on the temporary canvas, as background for this child control
	this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);

	// Draw the handles
	this->DrawHandles(Dc);
}

//!
void wxMitkVtkColorTransferFunctionCanvas::UnSelectHandle(void)
{
	this->selectedHandle = -1;
}

//!
void wxMitkVtkColorTransferFunctionCanvas::SelectHandle(int index)
{
	assert(this->colorFunc != NULL);
	if(index < this->colorFunc->GetSize())
		this->selectedHandle = index;
}

//!
int wxMitkVtkColorTransferFunctionCanvas::GetSelectedHandle(void) const
{
	return this->selectedHandle;
}

//!
bool wxMitkVtkColorTransferFunctionCanvas::HandleIsSelected(void) const
{
	return this->selectedHandle != -1;
}

//!
void wxMitkVtkColorTransferFunctionCanvas::DrawHandles(wxDC& Dc)
{
	double* pointData;
	int width, height;
	int currPoint[2];
	int nextPoint[2];
	unsigned int r, g, b;
	
	wxFont handleFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxString handleNumber = wxT("");
	wxSize textSize;

	// Get current widget area
    this->GetClientSize(&width, &height);

	if (this->colorFunc != NULL) 
	{
		Dc.SetFont(handleFont);

		pointData = this->colorFunc->GetDataPointer();
		if(pointData == NULL)
			return;

		for (int i=0; i < this->colorFunc->GetSize(); i++)
		{
			// converts values (X/Y) of the TF-point to pixel locations on the canvas-point
			this->FunctionPointToCanvas(pointData[i*4], currPoint[0], currPoint[1]);
			// Get the current handle colour
			r = (unsigned int)(pointData[i*4+1] * 255);
			g = (unsigned int)(pointData[i*4+2] * 255);
			b = (unsigned int)(pointData[i*4+3] * 255);

			if (i + 1 < this->colorFunc->GetSize())
			{
				// If there is next point, draw a line connecting both
				this->FunctionPointToCanvas(pointData[(i+1)*4], nextPoint[0], nextPoint[1]);
				Dc.SetPen(wxPen(wxColour( 0, 0, 0 ), 2, wxSOLID));
				Dc.DrawLine(currPoint[0], currPoint[1], nextPoint[0], nextPoint[1]);
			}

			// Draw the handle point
			Dc.SetPen(wxPen( wxColour( 0, 0, 0 ), 1, wxSOLID));
			
			if(this->selectedHandle == i)
			{
				// Paint it bigger, highligted and with the handle index and current colour
				Dc.SetBrush(wxBrush(wxColour(r, g, b), wxSOLID));
				Dc.DrawCircle(currPoint[0], currPoint[1], 7);
				handleNumber.sprintf(wxT("%d"), i+1);
				textSize = Dc.GetTextExtent(handleNumber);
				Dc.DrawText(handleNumber, currPoint[0] - (textSize.x / 2), currPoint[1] - (textSize.y / 2));
			}
			else
			{
				// Paint it smaller, not highlighted and with no text
				Dc.SetBrush(wxBrush(wxColour(r, g, b), wxSOLID));
				Dc.DrawCircle(currPoint[0], currPoint[1], 4);
			}
			
		}
		// Restore the brush
		Dc.SetPen(wxNullPen);
		Dc.SetBrush(wxNullBrush);
		Dc.SetFont(wxNullFont);
	}
}

/**
Transforms a function point of the piecewise function to its canvas coordinates
\return fpX, fpY is the function point
\return canvasX, canvasY are the canvas coords, returned its value by reference
*/
void wxMitkVtkColorTransferFunctionCanvas::FunctionPointToCanvas(double fpX, int& canvasX, int& canvasY) const
{
	assert(this->colorFunc != NULL);

	int x0, y0, width, height;
	this->GetInSetFrameAreaWithoutBorder(x0, y0, width, height);

	int fMin = this->colorFunc->GetRange()[0];
	int fMax = this->colorFunc->GetRange()[1];

	canvasX = (int)(width * (fpX - fMin) / (fMax - fMin)) + x0;
	canvasY = (int)(height / 2) + y0;
}

/**
Transforms a canvas coord to the function point of the piecewise function
\param canvasX, canvasY are the canvas coords 
\return fpX, fpY is the function point, returned its value by reference
*/
void wxMitkVtkColorTransferFunctionCanvas::CanvasToFunctionPoint(int canvasX, int canvasY, double& fpX, double rgb[3]) const
{
	assert(this->colorFunc != NULL);
	int x0, y0, width, height;
	this->GetInSetFrameAreaWithoutBorder(x0, y0, width, height);

	int fMin = this->colorFunc->GetRange()[0];
	int fMax = this->colorFunc->GetRange()[1];

	fpX = ((double)(canvasX - x0) * (double)(fMax - fMin) / (double)width) + (double)fMin;
	this->colorFunc->GetColor(fpX, rgb);
}

/**
Returns the nearest handle (the piecewise function index) to the desired input canvas coordinate,
if it was placed within the maximum distance set for grabbing a handle (defined by maxSquareDistanceToGrabHandle).
Return -1 either if an error was found or no handle was in range.
*/
int wxMitkVtkColorTransferFunctionCanvas::GetFunctionPointIndexNearCanvasPoint(int canvasX, int canvasY) const
{
	int x0, y0;
	if(this->colorFunc == NULL)
		return -1;	

	double* pointData = this->colorFunc->GetDataPointer();
	if(pointData == NULL)
		return -1;

	// For each function point, get the nearest handle to the desired input canvas coordinate 
	for(int i=0; i < this->colorFunc->GetSize(); i++)
	{
		FunctionPointToCanvas(pointData[i*4], x0, y0);
		if((x0-canvasX)*(x0-canvasX) + (y0-canvasY)*(y0-canvasY) <= this->maxSquareDistanceToGrabHandle)
			return i;
	}
	return -1;
}

/**
Returns the function point that is holding the handle at a given index position.
\param selectedPointIndex is the index position (or handle). Make sure you set a valid index
\return fpX,fpY is the function point, returned its value by reference
*/
void wxMitkVtkColorTransferFunctionCanvas::GetFunctionPointFromPointIndex(int selectedPointIndex, double& fpX, double rgb[3]) const
{
	assert(this->colorFunc != NULL);
	assert(selectedPointIndex < this->colorFunc->GetSize());

	double* pointData = this->colorFunc->GetDataPointer();
	fpX = pointData[selectedPointIndex*4];
	rgb[0] = pointData[selectedPointIndex*4+1];
	rgb[1] = pointData[selectedPointIndex*4+2];
	rgb[2] = pointData[selectedPointIndex*4+3];
}



