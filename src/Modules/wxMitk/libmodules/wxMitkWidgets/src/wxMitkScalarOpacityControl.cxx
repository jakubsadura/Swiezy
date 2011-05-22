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

#include "wxMitkScalarOpacityControl.h"

#include <mitkRenderingManager.h>
#include <vtkPiecewiseFunction.h>

using namespace mitk;


// Declare events to process
BEGIN_EVENT_TABLE(wxMitkScalarOpacityControl, wxControl)
    EVT_PAINT				(wxMitkScalarOpacityControl::OnPaint)
	EVT_MOUSE_EVENTS		(wxMitkScalarOpacityControl::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST	(wxMitkScalarOpacityControl::OnMouseCaptureLost)
	EVT_ERASE_BACKGROUND	(wxMitkScalarOpacityControl::OnEraseBackground)
END_EVENT_TABLE()

//!
wxMitkScalarOpacityControl::wxMitkScalarOpacityControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxControl(parent, id, pos, size, style, validator, name), wxMitkPaintableInSetCanvas(), wxMitkHistogramCanvas(), wxMitkVtkPiecewiseCanvas(), buttonPressed(false)
{
}

//!
wxMitkScalarOpacityControl::~wxMitkScalarOpacityControl(void)
{
}


/**
Handles mouse events from wxControl for grabbing handles, moving, adding and removing points.
*/
void wxMitkScalarOpacityControl::OnMouseEvent(wxMouseEvent& event)
{
	if(this->GetPiecewiseFunction() == NULL)
		return;

	// Capture mouse so as to avoid dragging outside the client area
	if(event.ButtonDown())
		this->CaptureMouse();
	
	double fpX, fpY;
	double selectedX, selectedY;

	int clientWidth;
	int clientHeight;
	this->GetClientSize(&clientWidth, &clientHeight);
	int x0, y0;
	this->GetInSetMargin(x0, y0);
	int selectedHandle = this->GetSelectedHandle();

	wxPoint mousePos = event.GetPosition();
	// Fit mousePos inside the drawable and interactable area
	mousePos.x = (mousePos.x < x0) ? x0 : mousePos.x;
	mousePos.y = (mousePos.y < y0) ? y0 : mousePos.y;
	mousePos.x = (mousePos.x > (clientWidth - x0)) ? (clientWidth - x0) : mousePos.x;
	mousePos.y = (mousePos.y > (clientHeight - y0)) ? (clientHeight - y0) : mousePos.y;

	switch(event.GetButton())
	{
	case wxMOUSE_BTN_LEFT:
			if(!this->HandleIsSelected() && this->GetPiecewiseFunction() != NULL && !event.LeftIsDown())
			{
				// No handle selected, so create a new handle at that point
				this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, fpY);
				this->GetPiecewiseFunction()->AddPoint(fpX, fpY);
				// Repaint the control and update observers, because it was modified
				this->Refresh();
				this->UpdateObserversOfTransferFunction();
			}
			// Select a new handle
			selectedHandle = this->GetFunctionPointIndexNearCanvasPoint(mousePos.x, mousePos.y);
			this->SelectHandle(selectedHandle);
			break;


	case wxMOUSE_BTN_RIGHT:
			// If a handle is selected and within the mouse boundary, and right click is pressed, then delete
			if(this->GetPiecewiseFunction() != NULL && !event.RightIsDown()
				&& selectedHandle > 0 && selectedHandle < this->GetPiecewiseFunction()->GetSize()-1
				&& selectedHandle == this->GetFunctionPointIndexNearCanvasPoint(mousePos.x, mousePos.y))
			{
				this->GetFunctionPointFromPointIndex(selectedHandle, selectedX, selectedY);
				this->GetPiecewiseFunction()->RemovePoint(selectedX);
				this->UnSelectHandle();
				// Repaint the control, because it was modified
				this->Refresh();
				this->UpdateObserversOfTransferFunction();
			}
			break;

	case wxMOUSE_BTN_MIDDLE:
	default:
		break;
	}


	// A handle was selected, check dragging and moving point inside the boundaries
	if(this->HandleIsSelected() && this->GetPiecewiseFunction() != NULL && event.LeftIsDown())
	{
		// Compute previous and new positions for that handle
		this->GetFunctionPointFromPointIndex(selectedHandle, selectedX, selectedY);
		this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, fpY);

		// Check that the mouse is moving to a new valid function point, and block that movement by modifying fpX
		if(MovingFunctionPointIsAllowed(fpX))
		{
			// Move that point to the new location
			this->GetPiecewiseFunction()->RemovePoint(selectedX);
			this->GetPiecewiseFunction()->AddPoint(fpX, fpY);
		}

		// Repaint the control, because it was modified
		this->Refresh();
		if(this->interactiveApply)
			this->UpdateObserversOfTransferFunction();
	}

	// Update observers, if interactive apply was not selected and we released the mouse button
	if(!this->interactiveApply && this->GetPiecewiseFunction() != NULL && this->buttonPressed && !event.LeftIsDown())
		this->UpdateObserversOfTransferFunction();
	// update the variable holding the "user hold the button"
	this->buttonPressed = event.LeftIsDown();

	if(this->HasCapture())
		this->ReleaseMouse();

	// Update the widgets that may be displaying the opacity function values under mouse position
	this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, fpY); 
	this->UpdateObserversOfMousePosition(fpX, fpY);

	// Skip must be called in order to allow the default processing to take place as otherwise the window
	// under mouse wouldn't get the focus.
	event.Skip();
}

//!
void wxMitkScalarOpacityControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	//if(this->HasCapture())
	//	this->ReleaseMouse();
	//event.Skip();
}

//!
void wxMitkScalarOpacityControl::OnPaint(wxPaintEvent& WXUNUSED(event))
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
void wxMitkScalarOpacityControl::OnEraseBackground(wxEraseEvent& event)
{
  	event.Skip(false);
}


//!
void wxMitkScalarOpacityControl::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	if(this->wxControl::IsEnabled())
	{
		// Draw the parent control on the temporary canvas, as background for this child control
		this->wxMitkHistogramCanvas::DrawControl(Dc, foreGroundOnly);
		this->wxMitkVtkPiecewiseCanvas::DrawControl(Dc, true);
	}
	else
	{
		this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);
	}
}

//!
void wxMitkScalarOpacityControl::GetClientSize(int* width, int* height) const
{
	this->wxControl::GetClientSize(width, height);
}

//!
wxColour wxMitkScalarOpacityControl::GetBackgroundColour() const
{
	return this->wxControl::GetBackgroundColour();
}

/**
Convenience function for deciding if the function values for the current selected handle are valid and
do not cross the position of other function points
\returns true if allows to move to the new position. If it allows the movement if also modifies directly the desired function point fpX to make it valid
*/
bool wxMitkScalarOpacityControl::MovingFunctionPointIsAllowed(double& fpX)
{
	int selectedHandle = this->GetSelectedHandle();
	if(this->GetPiecewiseFunction() == NULL)
		return false;

	double* pointData = this->GetPiecewiseFunction()->GetDataPointer();

	// If is the first or last handle, you may move it only up and down
	if(selectedHandle == 0 || selectedHandle == this->GetPiecewiseFunction()->GetSize() - 1)
		fpX = pointData[selectedHandle * 2];
	else
	{
		// No handle may be moved further than the next or prev handle
		if(selectedHandle > 0 && pointData[(selectedHandle - 1) * 2] >= fpX)
			return false;
		if(selectedHandle < this->GetPiecewiseFunction()->GetSize() - 1 && pointData[(selectedHandle + 1) * 2] <= fpX)
			return false;
	}

	return true;
}

/**
Updates the 3D render windows that are rendering the volume image propietary of the transfer function 
handled by this widget
*/
void wxMitkScalarOpacityControl::UpdateObserversOfTransferFunction(void)
{
	mitk::RenderingManager::GetInstance()->SetNextLOD(0);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
}

/**
Updates the widgets that might be displaying the values of the opacity function under the mouse position.
This is done by casting a wxMitkMouseOverHistogramEvent. Catch it by using EVT_MOUSEOVERHISTOGRAM in your
event table.
\sa wxMitkMouseOverHistogramEvent
*/
void wxMitkScalarOpacityControl::UpdateObserversOfMousePosition(const double fpX, const double opacity)
{
	wxMitkMouseOverHistogramEvent event(this->GetId());
	event.SetEventObject(this);
	event.functionPoint = fpX;
	event.opacityValue = opacity;
	event.isColorFunction = false;

	// Cast the event
	this->GetEventHandler()->ProcessEvent(event);
}

/**
Enables or disables the Interactive apply. When interactive apply is on, any mouse interaction updates the
transferfunction and the rendering inmediately. Otherwise, it only is updated when the user releases the mouse
(finishes the interaction)
*/
void wxMitkScalarOpacityControl::SetInteractiveApply(bool enable)
{
	this->interactiveApply = enable;
}

//!
bool wxMitkScalarOpacityControl::Enable(bool enable)
{
	// Forces repaint, because it does not repaint when disabling / enabling
	bool b = this->wxControl::Enable(enable);
	this->wxControl::Refresh();
	return b;
}


	

