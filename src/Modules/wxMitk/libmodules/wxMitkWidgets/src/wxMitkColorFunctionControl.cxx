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


#include "wxMitkColorFunctionControl.h"
#include <wx/colordlg.h>
#include <mitkRenderingManager.h>
#include <vtkColorTransferFunction.h>

using namespace mitk;


// Declare events to process
BEGIN_EVENT_TABLE(wxMitkColorFunctionControl, wxControl)
    EVT_PAINT				(wxMitkColorFunctionControl::OnPaint)
	EVT_MOUSE_EVENTS		(wxMitkColorFunctionControl::OnMouseEvent)
	EVT_MOUSE_CAPTURE_LOST	(wxMitkColorFunctionControl::OnMouseCaptureLost)
	EVT_ERASE_BACKGROUND	(wxMitkColorFunctionControl::OnEraseBackground)
END_EVENT_TABLE()

//!
wxMitkColorFunctionControl::wxMitkColorFunctionControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxControl(parent, id, pos, size, style, validator, name), wxMitkPaintableInSetCanvas(), wxMitkHistogramCanvas(), wxMitkVtkColorTransferFunctionCanvas(), buttonPressed(false)
{
}

//!
wxMitkColorFunctionControl::~wxMitkColorFunctionControl(void)
{
}


/**
Handles mouse events from wxControl for grabbing handles, moving, adding and removing points.
*/
void wxMitkColorFunctionControl::OnMouseEvent(wxMouseEvent& event)
{
	if(this->GetColorTransferFunction() == NULL)
		return;

	// Capture mouse so as to avoid dragging outside the client area
	if(event.ButtonDown())
		this->CaptureMouse();

	double fpX;
	double selectedX;
	double rgb[3];
	
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
			if(!this->HandleIsSelected() && this->GetColorTransferFunction() != NULL && !event.LeftIsDown())
			{
				// No handle selected, so create a new handle at that point
				this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, rgb);
				this->GetColorTransferFunction()->AddRGBPoint(fpX, rgb[0], rgb[1], rgb[2]);
				// Repaint the control, because it was modified
				this->Refresh();
				this->UpdateObserversOfTransferFunction();
			}
			// Select a new handle
			selectedHandle = this->GetFunctionPointIndexNearCanvasPoint(mousePos.x, mousePos.y);
			this->SelectHandle(selectedHandle);

			// If double-clicked, open the color picker dialog
			if(event.LeftDClick())
			{
				// Get the current colour so as to select it by default at the dialog
				this->GetFunctionPointFromPointIndex(selectedHandle, fpX, rgb);
	
				wxColourData data;
				wxColour selectedColour((unsigned int)(rgb[0] * 255), (unsigned int)(rgb[1] * 255), (unsigned int)(rgb[2] * 255));
				data.SetColour(selectedColour);

				// --- Giving custom colours only works under windows
				data.SetChooseFull(true);
				for (int i = 0; i < 16; i++)
				{
					wxColour colour(i*16, i*16, i*16);
					data.SetCustomColour(i, colour);
				}
				// --- .
				
				// Show the dialog
				wxColourDialog dialog((wxControl*)this, &data);
				if (dialog.ShowModal() == wxID_OK)
				{
					wxColourData colorData = dialog.GetColourData();
					wxColour selectedColor = colorData.GetColour();
					
					// Set the chosen color to the current function point
					rgb[0] = (double)selectedColor.Red() / 255.0;
					rgb[1] = (double)selectedColor.Green() / 255.0;
					rgb[2] = (double)selectedColor.Blue() / 255.0;
					this->GetColorTransferFunction()->RemovePoint(fpX);
					this->GetColorTransferFunction()->AddRGBPoint(fpX, rgb[0], rgb[1], rgb[2]);

					// Update
					this->Refresh();
					this->UpdateObserversOfTransferFunction();
				}
			}
			break;


	case wxMOUSE_BTN_RIGHT:
			// If a handle is selected and within the mouse boundary, and right click is pressed, then delete
			if(this->GetColorTransferFunction() != NULL && !event.RightIsDown()
				&& selectedHandle > 0 && selectedHandle < this->GetColorTransferFunction()->GetSize()-1
				&& selectedHandle == this->GetFunctionPointIndexNearCanvasPoint(mousePos.x, mousePos.y))
			{
				this->GetFunctionPointFromPointIndex(selectedHandle, selectedX, rgb);
				this->GetColorTransferFunction()->RemovePoint(selectedX);
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
	if(this->HandleIsSelected() && this->GetColorTransferFunction() != NULL && event.LeftIsDown())
	{
		// Compute previous and new positions for that handle
		this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, rgb); 
		this->GetFunctionPointFromPointIndex(selectedHandle, selectedX, rgb); // prev rgb value is saved

		// Check that the mouse is moving to a new valid function point, and block that movement by modifying fpX
		if(MovingFunctionPointIsAllowed(fpX))
		{
			// Move that point to the new location
			this->GetColorTransferFunction()->RemovePoint(selectedX);
			this->GetColorTransferFunction()->AddRGBPoint(fpX, rgb[0], rgb[1], rgb[2]);
		}

		// Repaint the control, because it was modified
		this->Refresh();
		if(this->interactiveApply)
			this->UpdateObserversOfTransferFunction();
	}

	// Update observers, if interactive apply was not selected and we released the mouse button
	if(!this->interactiveApply && this->GetColorTransferFunction() != NULL && this->buttonPressed && !event.LeftIsDown())
		this->UpdateObserversOfTransferFunction();
	// update the variable holding the "user hold the button"
	this->buttonPressed = event.LeftIsDown();

	if(this->HasCapture())
		this->ReleaseMouse();

	// Update the widgets that may be displaying the color function values under mouse position
	this->CanvasToFunctionPoint(mousePos.x, mousePos.y, fpX, rgb); 
	this->UpdateObserversOfMousePosition(fpX, rgb);

	// Skip must be called in order to allow the default processing to take place as otherwise the window
	// under mouse wouldn't get the focus.
	event.Skip();
}

void wxMitkColorFunctionControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	//if(this->HasCapture())
	//	this->ReleaseMouse();
	//event.Skip();
}

//!
void wxMitkColorFunctionControl::OnPaint(wxPaintEvent& WXUNUSED(event))
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
void wxMitkColorFunctionControl::OnEraseBackground(wxEraseEvent& event)
{
  	event.Skip(false);
}


//!
void wxMitkColorFunctionControl::DrawControl(wxDC& Dc, bool foreGroundOnly)
{
	if(this->wxControl::IsEnabled())
	{
		// Draw the parent control on the temporary canvas, as background for this child control
		this->wxMitkHistogramCanvas::DrawControl(Dc, foreGroundOnly);
		this->wxMitkVtkColorTransferFunctionCanvas::DrawControl(Dc, true);
	}
	else
	{
		this->wxMitkPaintableInSetCanvas::DrawControl(Dc, foreGroundOnly);
	}
}

//!
void wxMitkColorFunctionControl::GetClientSize(int* width, int* height) const
{
	this->wxControl::GetClientSize(width, height);
}

//!
wxColour wxMitkColorFunctionControl::GetBackgroundColour() const
{
	return this->wxControl::GetBackgroundColour();
}

/**
Convenience function for deciding if the function values for the current selected handle are valid and
do not cross the position of other function points
\returns true if allows to move to the new position. If it allows the movement if also modifies directly the desired function point fpX to make it valid
*/
bool wxMitkColorFunctionControl::MovingFunctionPointIsAllowed(double& fpX) const
{
	int selectedHandle = this->GetSelectedHandle();
	if(this->GetColorTransferFunction() == NULL)
		return false;

	double* pointData = this->GetColorTransferFunction()->GetDataPointer();

	// If is the first or last handle, you may not move it
	if(selectedHandle == 0 || selectedHandle == this->GetColorTransferFunction()->GetSize() - 1)
		return false;
	else
	{
		// No handle may be moved further than the next or prev handle
		if(selectedHandle > 0 && pointData[(selectedHandle - 1) * 4] >= fpX)
			return false;
		if(selectedHandle < this->GetColorTransferFunction()->GetSize() - 1 && pointData[(selectedHandle + 1) * 4] <= fpX)
			return false;
	}

	return true;
}

/**
Updates the 3D render windows that are rendering the volume image propietary of the transfer function 
handled by this widget. A wxMitkColorFunctionChangedEvent is cast for signalling this event. 
Catch it by using EVT_COLORFUNCTION_CHANGED in your event table.
\sa wxMitkColorFunctionChangedEvent
*/
void wxMitkColorFunctionControl::UpdateObserversOfTransferFunction(void)
{
	mitk::RenderingManager::GetInstance()->SetNextLOD(0);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll(
		mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
	
	// Create a Color Function changed event and configure it
	wxMitkColorFunctionChangedEvent event(this->GetId());
    event.SetEventObject(this);
	event.colorFunc = this->GetColorTransferFunction();

	// Cast the event
	this->GetEventHandler()->ProcessEvent(event);
}

/**
Updates the widgets that might be displaying the values of the color function under the mouse position.
This is done by casting a wxMitkMouseOverHistogramEvent. Catch it by using EVT_MOUSEOVERHISTOGRAM in your
event table.
\sa wxMitkMouseOverHistogramEvent
*/
void wxMitkColorFunctionControl::UpdateObserversOfMousePosition(const double fpX, const double rgb[3])
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


/**
Enables or disables the Interactive apply. When interactive apply is on, any mouse interaction updates the
transferfunction and the rendering inmediately. Otherwise, it only is updated when the user releases the mouse
(finishes the interaction)
*/
void wxMitkColorFunctionControl::SetInteractiveApply(bool enable)
{
	this->interactiveApply = enable;
}

//!
bool wxMitkColorFunctionControl::Enable(bool enable)
{
	// Forces repaint, because it does not repaint when disabling / enabling
	bool b = this->wxControl::Enable(enable);
	this->wxControl::Refresh();
	return b;
}



//////////////////////////////////////////////////////////////////////////////////
//
// Code for the wxMitkColorFunctionChangedEvent
//
//////////////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(mitk::wxEVT_COLORFUNCTION_CHANGED)
IMPLEMENT_DYNAMIC_CLASS(wxMitkColorFunctionChangedEvent, wxCommandEvent)

//!
wxMitkColorFunctionChangedEvent::wxMitkColorFunctionChangedEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid), colorFunc(NULL)
{
}

//!
wxMitkColorFunctionChangedEvent::~wxMitkColorFunctionChangedEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxMitkColorFunctionChangedEvent::Clone()
{
	return new wxMitkColorFunctionChangedEvent(*this);
}

//!
vtkColorTransferFunction* wxMitkColorFunctionChangedEvent::GetColorTransferFunction(void)
{
	return this->colorFunc;
}



