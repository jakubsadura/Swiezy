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

#include "wxMitkVtkPiecewiseCanvas.h"

#include <mitkRenderingManager.h>
#include <vtkPiecewiseFunction.h>

using namespace mitk;


//!
wxMitkVtkPiecewiseCanvas::wxMitkVtkPiecewiseCanvas(void) 
: wxMitkPaintableInSetCanvas(), maxSquareDistanceToGrabHandle(16), selectedHandle(-1), pieceWiseFunc(NULL)
{

}

//!
wxMitkVtkPiecewiseCanvas::~wxMitkVtkPiecewiseCanvas(void)
{
}

//!
void wxMitkVtkPiecewiseCanvas::SetPiecewiseFunction(vtkPiecewiseFunction* func)
{
	this->pieceWiseFunc = func;
}

//!
vtkPiecewiseFunction* wxMitkVtkPiecewiseCanvas::GetPiecewiseFunction(void) const
{
	return this->pieceWiseFunc;
}


//!
void wxMitkVtkPiecewiseCanvas::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	// Draw the parent control on the temporary canvas, as background for this child control
	this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);

	// Draw the handles
	this->DrawHandles(Dc);
}

//!
void wxMitkVtkPiecewiseCanvas::UnSelectHandle(void)
{
	this->selectedHandle = -1;
}

//!
void wxMitkVtkPiecewiseCanvas::SelectHandle(int index)
{
	assert(this->pieceWiseFunc != NULL);
	if(index < this->pieceWiseFunc->GetSize())
		this->selectedHandle = index;
}

//!
int wxMitkVtkPiecewiseCanvas::GetSelectedHandle(void) const
{
	return this->selectedHandle;
}

//!
bool wxMitkVtkPiecewiseCanvas::HandleIsSelected(void) const
{
	return this->selectedHandle != -1;
}

//!
void wxMitkVtkPiecewiseCanvas::DrawHandles(wxDC& Dc)
{
	double* pointData;
	int width, height;
	int currPoint[2];
	int nextPoint[2];
	wxBrush selectedBrush(wxColour( 143, 143, 188) , wxSOLID);
	wxBrush defaultBrush(wxColour( 255, 255, 255 ), wxSOLID);
	wxFont handleFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxString handleNumber = wxT("");
	wxSize textSize;

	// Get current widget area
    this->GetClientSize(&width, &height);

	if (this->pieceWiseFunc != NULL) 
	{
		Dc.SetFont(handleFont);

		pointData = this->pieceWiseFunc->GetDataPointer();
		if(pointData == NULL)
			return;

		for (int i=0; i < this->pieceWiseFunc->GetSize(); i++)
		{
			// converts values (X/Y) of the TF-point to pixel locations on the canvas-point
			this->FunctionPointToCanvas(pointData[i*2], pointData[i*2+1], currPoint[0], currPoint[1]);
			if (i + 1 < this->pieceWiseFunc->GetSize())
			{
				// If there is next point, draw a line connecting both
				this->FunctionPointToCanvas(pointData[(i+1)*2], pointData[(i+1)*2+1], nextPoint[0], nextPoint[1]);
				Dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxSOLID));
				Dc.DrawLine(currPoint[0], currPoint[1], nextPoint[0], nextPoint[1]);
			}
			// Draw the handle point
			Dc.SetPen(wxPen(wxColour( 0, 0, 0 ), 1, wxSOLID));
			if(this->selectedHandle == i)
			{
				// Paint it bigger, highligted and with the handle index
				Dc.SetBrush(selectedBrush);
				Dc.DrawCircle(currPoint[0], currPoint[1], 7);
				handleNumber.sprintf(wxT("%d"), i+1);
				textSize = Dc.GetTextExtent(handleNumber);
				Dc.DrawText(handleNumber, currPoint[0] - (textSize.x / 2), currPoint[1] - (textSize.y / 2));
			}
			else
			{
				// Paint it smaller, not highlighted and with no text
				Dc.SetBrush(defaultBrush);
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
void wxMitkVtkPiecewiseCanvas::FunctionPointToCanvas(double fpX, double fpY, int& canvasX , int& canvasY) const
{
	assert(this->pieceWiseFunc != NULL);

	int x0, y0, width, height;
	this->GetInSetFrameAreaWithoutBorder(x0, y0, width, height);

	int fMin = int(this->pieceWiseFunc->GetRange()[0]);
	int fMax = int(this->pieceWiseFunc->GetRange()[1]);

	canvasX = (int)(width * (fpX - fMin) / (fMax - fMin)) + x0;
	canvasY = (int)(height * (1 - fpY)) + y0;
}

/**
Transforms a canvas coord to the function point of the piecewise function
\param canvasX, canvasY are the canvas coords 
\return fpX, fpY is the function point, returned its value by reference
*/
void wxMitkVtkPiecewiseCanvas::CanvasToFunctionPoint(int canvasX , int canvasY, double& fpX, double& fpY) const
{
	assert(this->pieceWiseFunc != NULL);

	int x0, y0, width, height;
	this->GetInSetFrameAreaWithoutBorder(x0, y0, width, height);

	int fMin = int(this->pieceWiseFunc->GetRange()[0]);
	int fMax = int(this->pieceWiseFunc->GetRange()[1]);

	fpX = ((double)(canvasX - x0) * (double)(fMax - fMin) / (double)width) + (double)fMin;
	fpY = 1.0 + (double)(y0 - canvasY) / (double)height;
	if(fpY > 1.0) fpY = 1.0;
}

/**
Returns the nearest handle (the piecewise function index) to the desired input canvas coordinate,
if it was placed within the maximum distance set for grabbing a handle (defined by maxSquareDistanceToGrabHandle).
Return -1 either if an error was found or no handle was in range.
*/
int wxMitkVtkPiecewiseCanvas::GetFunctionPointIndexNearCanvasPoint(int canvasX, int canvasY) const
{
	int x0, y0;
	if(this->pieceWiseFunc == NULL)
		return -1;	

	double* pointData = this->pieceWiseFunc->GetDataPointer();
	if(pointData == NULL)
		return -1;

	// For each function point, get the nearest handle to the desired input canvas coordinate 
	for(int i=0; i < this->pieceWiseFunc->GetSize(); i++)
	{
		FunctionPointToCanvas(pointData[i*2], pointData[i*2+1], x0, y0);
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
void wxMitkVtkPiecewiseCanvas::GetFunctionPointFromPointIndex(int selectedPointIndex, double& fpX, double& fpY) const
{
	assert(this->pieceWiseFunc != NULL);
	assert(selectedPointIndex < this->pieceWiseFunc->GetSize());

	double* pointData = this->pieceWiseFunc->GetDataPointer();
	fpX = pointData[selectedPointIndex*2];
	fpY = pointData[selectedPointIndex*2+1];
}



