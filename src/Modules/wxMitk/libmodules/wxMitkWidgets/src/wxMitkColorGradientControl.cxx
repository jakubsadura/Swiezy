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

#include "wxMitkColorGradientControl.h"

using namespace mitk;

// Declare events to process
BEGIN_EVENT_TABLE(wxMitkColorGradientControl, wxControl)
    EVT_PAINT				(wxMitkColorGradientControl::OnPaint)
	EVT_ERASE_BACKGROUND	(wxMitkColorGradientControl::OnEraseBackground)
	EVT_MOTION				(wxMitkColorGradientControl::OnMouseEvent)
END_EVENT_TABLE()

//!
wxMitkColorGradientControl::wxMitkColorGradientControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxControl(parent, id, pos, size, style, validator, name), wxMitkPaintableInSetCanvas(), wxMitkColorGradientCanvas(), wxMitkVtkColorTransferFunctionCanvas()
{
}

//!
wxMitkColorGradientControl::~wxMitkColorGradientControl(void)
{
}

//!
void wxMitkColorGradientControl::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	// Set the rendering context to this widget
    wxPaintDC Dc(this);
	wxMemoryDC MemDc;
	
	// Get current widget area
    int width, height;
	this->GetClientSize(&width, &height);
	
	// Create a bitmap for blitting to the screen
    wxBitmap *bitmapInMemory = new wxBitmap(width, height);
    MemDc.SelectObject(*bitmapInMemory);

	// Draw the control
	this->DrawControl(MemDc);

	// Blit the memory device context to screen and release memory
    Dc.Blit(0, 0, width, height, &MemDc, 0, 0, wxCOPY);
	delete bitmapInMemory;
}

/**
\brief Handles erase background events, turn off background erase to reduce flickering (mainly happening on Windows O.S) 
*/
void wxMitkColorGradientControl::OnEraseBackground(wxEraseEvent& event)
{
  	event.Skip(false);
}

/**
Handles mouse events from wxControl for capturing mouse position.
*/
void wxMitkColorGradientControl::OnMouseEvent(wxMouseEvent& event)
{
	if(this->GetColorTransferFunction() == NULL)
		return;

	double fpX;
	double rgb[3];
	
	int clientWidth;
	int clientHeight;
	this->GetClientSize(&clientWidth, &clientHeight);
	int x0, y0;
	this->GetInSetMargin(x0, y0);

	wxPoint mousePos = event.GetPosition();
	// Fit mousePos inside the drawable and interactable area
	mousePos.x = (mousePos.x < x0) ? x0 : mousePos.x;
	mousePos.y = (mousePos.y < y0) ? y0 : mousePos.y;
	mousePos.x = (mousePos.x > (clientWidth - x0)) ? (clientWidth - x0) : mousePos.x;
	mousePos.y = (mousePos.y > (clientHeight - y0)) ? (clientHeight - y0) : mousePos.y;

	// Update the widgets that may be displaying the color function values under mouse position
	this->wxMitkVtkColorTransferFunctionCanvas::CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, rgb); 
	this->UpdateObserversOfMousePosition(fpX, rgb);

	// Skip must be called in order to allow the default processing to take place as otherwise the window
	// under mouse wouldn't get the focus.
	event.Skip();
}


//!
void wxMitkColorGradientControl::GetClientSize(int* width, int* height) const
{
	this->wxControl::GetClientSize(width, height);
}

//!
wxColour wxMitkColorGradientControl::GetBackgroundColour() const
{
	return this->wxControl::GetBackgroundColour();
}

/**
Updates the widgets that might be displaying the values of the color function under the mouse position.
This is done by casting a wxMitkMouseOverHistogramEvent. Catch it by using EVT_MOUSEOVERHISTOGRAM in your
event table.
*/
void wxMitkColorGradientControl::UpdateObserversOfMousePosition(const double fpX, const double rgb[3])
{
	wxMitkMouseOverHistogramEvent event(this->GetId());
	event.SetEventObject(this);
	event.functionPoint = fpX;
	event.rgb[0] = rgb[0];
	event.rgb[1] = rgb[1];
	event.rgb[2] = rgb[2];
	event.isColorFunction = true;

	// Cast the event
	this->GetEventHandler()->ProcessEvent(event);
}

//!
void wxMitkColorGradientControl::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	if(this->wxControl::IsEnabled())
	{
		// Draw the parent control on the temporary canvas, as background for this child control
		this->wxMitkColorGradientCanvas::DrawControl(Dc, foreGroundOnly);
	}
	else
	{
		this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);
	}
}

//!
void wxMitkColorGradientControl::SetColorTransferFunction(vtkColorTransferFunction* func)
{
	this->wxMitkColorGradientCanvas::SetColorTransferFunction(func);
	this->wxMitkVtkColorTransferFunctionCanvas::SetColorTransferFunction(func);
}

//!
vtkColorTransferFunction* wxMitkColorGradientControl::GetColorTransferFunction(void) const
{
	return this->wxMitkColorGradientCanvas::GetColorTransferFunction();
}

//!
bool wxMitkColorGradientControl::Enable(bool enable)
{
	// Forces repaint, because it does not repaint when disabling / enabling
	bool b = this->wxControl::Enable(enable);
	this->wxControl::Refresh();
	return b;
}


