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

#include "wxMitkRangeSliderControl.h"
#include "wxMitkLevelWindowHelper.h"

using namespace mitk;

// Declare events to process
BEGIN_EVENT_TABLE(wxMitkRangeSliderControl, wxControl)
    EVT_PAINT				(wxMitkRangeSliderControl::OnPaint)
	EVT_ERASE_BACKGROUND	(wxMitkRangeSliderControl::OnEraseBackground)
    EVT_SIZE				(wxMitkRangeSliderControl::OnSize)
	EVT_MOUSEWHEEL			(wxMitkRangeSliderControl::OnMouseWheel)
	EVT_LEFT_DOWN			(wxMitkRangeSliderControl::OnMousePress)
	EVT_LEFT_UP				(wxMitkRangeSliderControl::OnMouseRelease)
	EVT_MOTION				(wxMitkRangeSliderControl::OnMouseDrag)
	EVT_MOUSE_CAPTURE_LOST	(wxMitkRangeSliderControl::OnMouseCaptureLost)
END_EVENT_TABLE()


//!
wxMitkRangeSliderControl::wxMitkRangeSliderControl(wxWindow* parent, wxWindowID id, int value, unsigned int range, int minValue, int maxValue, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxControl(parent, id, pos, size, style), higherEdgeCoordinate(0), lowerEdgeCoordinate(0), draggingEdges(false), draggingHandle(false)
{
	isVertical = (style & wxSL_VERTICAL);
	if(isVertical)
	{
		this->SetMinSize(wxSize(24,32));
		this->SetMaxSize(wxSize(24,65335));
	}
	else
	{
		this->SetMinSize(wxSize(32,24));
		this->SetMaxSize(wxSize(65335,24));
	}
	this->ComputeValidRange(value, range, minValue, maxValue);

	this->centerLineColour = this->GetForegroundColour();
	this->handleBgColour = wxColour( 47, 47, 47 );
}

//!
wxMitkRangeSliderControl::~wxMitkRangeSliderControl(void)
{
}

//!
mitk::ScalarType wxMitkRangeSliderControl::GetValue(void)
{
	return this->value;
}

//!
mitk::ScalarType wxMitkRangeSliderControl::GetRange(void)
{
	return this->range;
}

//!
mitk::ScalarType wxMitkRangeSliderControl::GetMinValue(void)
{
	return this->minValue;
}

//!
mitk::ScalarType wxMitkRangeSliderControl::GetMaxValue(void)
{
	return this->maxValue;
}

//!
void wxMitkRangeSliderControl::SetValue(mitk::ScalarType value)
{
	this->ComputeValidRange(value, this->range, this->minValue, this->maxValue);
}

//!
void wxMitkRangeSliderControl::SetRange(mitk::ScalarType range)
{
	this->ComputeValidRange(this->value, range, this->minValue, this->maxValue);
}

//!
void wxMitkRangeSliderControl::SetMinValue(mitk::ScalarType minValue)
{
	this->ComputeValidRange(this->value, this->range, minValue, this->maxValue);
}

//!
void wxMitkRangeSliderControl::SetAll(int value, unsigned int range, int minValue, int maxValue)
{
	this->ComputeValidRange(value, range, minValue, maxValue);
}

//!
void wxMitkRangeSliderControl::SetMaxValue(mitk::ScalarType maxValue)
{
	this->ComputeValidRange(this->value, this->range, this->minValue, maxValue);
}

/**
Computes a valid range for the given value, range, minimum and maximum. It assigns the new values to the
internal variables, repaints the control in order to display the changes and casts the EVT_RANGE_CHANGED event.
*/
void wxMitkRangeSliderControl::ComputeValidRange(
	mitk::ScalarType value, 
	mitk::ScalarType range, 
	mitk::ScalarType minValue, 
	mitk::ScalarType maxValue)
{
	// Working variables
	mitk::ScalarType newValue = value;
	mitk::ScalarType newRange = range;
	mitk::ScalarType newMinValue = minValue;
	mitk::ScalarType newMaxValue = maxValue;

	// Create a Range changed event and configure it
	wxMitkRangeChangedEvent event(this->GetId());
    event.SetEventObject(this);

	//
	// Compute the new scalars
	//
	bool valid = mitk::wxMitkLevelWindowHelper::ComputeValidRange(newValue, newRange, newMinValue, newMaxValue);

	if(valid)
	{
		// End configuring the event
		event.valueChanged = this->value != newValue;
		event.rangeChanged = this->range != newRange;
		event.value = newValue;
		event.range = newRange;

		// Assign the computed values
		this->value = newValue;
		this->range = newRange;
		this->minValue = newMinValue;
		this->maxValue = newMaxValue;

		// Cast the event
		this->GetEventHandler()->ProcessEvent(event);
	}
}


//!
void wxMitkRangeSliderControl::OnPaint(wxPaintEvent& WXUNUSED(event))
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

    // Draw background
    MemDc.SetBrush(wxBrush(this->GetBackgroundColour(), wxSOLID));
    MemDc.DrawRectangle(wxRect(0, 0, width, height));
    MemDc.SetBrush(wxNullBrush);

	// Draw the handle
	this->DrawHandle(width, height, MemDc);

    // Blit the memory device context to screen and release memory
    Dc.Blit(0, 0, width, height, &MemDc, 0, 0, wxCOPY);
	delete bitmapInMemory;
}

//!
void wxMitkRangeSliderControl::OnSize(wxSizeEvent& event)
{
	this->Refresh();
	event.Skip();
}

//!
void wxMitkRangeSliderControl::OnMousePress(wxMouseEvent& event)
{
	wxPoint mousePos = event.GetPosition();
	if(isVertical)
		this->clickedCoordinate = mousePos.y;
	else
		this->clickedCoordinate = mousePos.x;
	this->draggingEdges = IsMouseWithinHandleEdges(this->clickedCoordinate);
	this->draggingHandle = IsMouseInsideTheHandle(this->clickedCoordinate);
	this->CaptureMouse();
	event.Skip();
}

//!
void wxMitkRangeSliderControl::OnMouseRelease(wxMouseEvent& event)
{
	this->draggingEdges = false;
	this->draggingHandle = false;
	if(this->HasCapture())
		this->ReleaseMouse();
	event.Skip();
}

//!
void wxMitkRangeSliderControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	// Restore initial state and don't skip the event
	// If we skip the event, we should process it by the parent window
	this->draggingEdges = false;
	this->draggingHandle = false;
}

//!
void wxMitkRangeSliderControl::OnMouseDrag(wxMouseEvent& event)
{
	// Get the mouse hit
	wxPoint mousePos = event.GetPosition();
	int mouseCoord;
	if(isVertical)
		mouseCoord = mousePos.y;
	else
		mouseCoord = mousePos.x;

	int scalar = this->MapCanvasToScalar(mouseCoord);

	int newScalar;
	// Get the increment of position (in scalar space)
	int increment = this->MapCanvasToScalar(this->clickedCoordinate) - scalar;

	// Set the cursor to improve the user experience
	if(!this->draggingHandle && (this->draggingEdges || IsMouseWithinHandleEdges(mouseCoord)))
		if(isVertical)
			this->SetCursor(wxCursor(wxCURSOR_SIZENS));
		else
			this->SetCursor(wxCursor(wxCURSOR_SIZEWE));
	else
		this->SetCursor(wxCursor(wxCURSOR_ARROW));

	// Check if user is dragging the handle or the edge (dragging value or range)
	if(this->draggingEdges)
	{
		if(scalar > this->value)
			newScalar = this->range - (increment * 2);
		else
			newScalar = this->range + (increment * 2);
		
		if(newScalar < 0) 
			newScalar = -1 * newScalar;

		//FIX: for some reason automatic rescaling (in wxMitkLevelWindowHelper::ComputeValidRange) 
		//doesn't work correctly especially when the handle is dragged toward the minValue; 
		//so simply forbid min/max value exceeding
		
		if((newScalar)>(maxValue-minValue))
			return;

		// Change the handle width to fit the new window
		this->SetRange(newScalar); 
		// update last coordinate
		this->clickedCoordinate = mouseCoord;
		// Repaint the widget
		this->Refresh();
	}
	else
	{
		// If we are dragging the handle
		if(this->draggingHandle)
		{
			newScalar = this->value - increment;
			
			//FIX: for some reason automatic rescaling (in wxMitkLevelWindowHelper::ComputeValidRange) 
			//doesn't work correctly expecially when the handle is dragged toward the minValue; 
			//so simply forbid min/max value exceeding
			if(((newScalar-range/2)<minValue) || ((newScalar+range/2)>maxValue))
				return;

			// 	Move the slider handle to the new position
			this->SetValue(newScalar);
			// update last coordinate
			this->clickedCoordinate = mouseCoord;
			// Repaint the widget
			this->Refresh();
		}
	}

	//continue processing the event
	event.Skip();
}

//!
void wxMitkRangeSliderControl::OnMouseWheel(wxMouseEvent& event)
{
	// Get current widget area
    int width, height;
    this->GetClientSize(&width, &height);

	// Scale the increment in steps in order to require always 10 wheel events for the full range of the slider
	int wheelStep = 0.1F * (this->maxValue - this->minValue);

	// If shift key is pressed we increment the range, otherwise we increment the value
	if(!event.ShiftDown())
	{
		// Increment the value
		if (event.GetWheelRotation() * event.GetWheelDelta() > 0) 
			this->SetValue(this->value - wheelStep);
		else
			this->SetValue(this->value + wheelStep);
	}
	else
	{
		// Increment the range
		if (event.GetWheelRotation() * event.GetWheelDelta() > 0) 
			this->SetRange(this->range + wheelStep);
		else
			this->SetRange(this->range - wheelStep);
	}

	// Repaint the widget and continue processing the event
	this->Refresh();
	event.Skip();
}

/**
Draws the handle in the position computed by the <i>Value</i> and with size computed by the <i>Range</i>
*/
void wxMitkRangeSliderControl::DrawHandle(int width, int height, wxMemoryDC& MemDc)
{
	int pos = ComputeHandlePositionAtCanvas();
	int size = ComputeHandleWidthAtCanvas();
	int handlePos = pos - (size / 2);
	int center = pos;
	int handleWidth, handleHeight;
	if(isVertical)
	{
		handleWidth = width - (wxMitkRangeSliderControl::HandleHorizontalOffset * 2);
		handleHeight = size;
		MemDc.SetBrush(wxBrush(this->handleBgColour, wxSOLID));
		MemDc.DrawRectangle(wxRect(wxMitkRangeSliderControl::HandleHorizontalOffset, handlePos, handleWidth, handleHeight));
		this->higherEdgeCoordinate = handlePos;
		this->lowerEdgeCoordinate = handlePos + handleHeight;
		MemDc.SetBrush(wxBrush(this->centerLineColour, wxSOLID));
		MemDc.DrawLine(wxMitkRangeSliderControl::HandleHorizontalOffset, center, width - wxMitkRangeSliderControl::HandleHorizontalOffset, center);
	}
	else
	{
		handleWidth = size;
		handleHeight = height - (wxMitkRangeSliderControl::HandleVerticalOffset * 2);
		MemDc.SetBrush(wxBrush(this->handleBgColour, wxSOLID));
		MemDc.DrawRectangle(wxRect(handlePos, wxMitkRangeSliderControl::HandleVerticalOffset, handleWidth, handleHeight));
		this->higherEdgeCoordinate = handlePos;
		this->lowerEdgeCoordinate = handlePos + handleWidth;
		MemDc.SetBrush(wxBrush(this->centerLineColour, wxSOLID));
		MemDc.DrawLine(center, wxMitkRangeSliderControl::HandleVerticalOffset, center,  height - wxMitkRangeSliderControl::HandleVerticalOffset);
	}

	MemDc.SetBrush(wxNullBrush);
}

/**
Computes the position of the center of the handle at the canvas, where it will be drawn. Transforms <i>Value</i> scalar
into pixel coordinates, computed by proportional location inside the range [min - max]. The returned position is being offset.
*/
int wxMitkRangeSliderControl::ComputeHandlePositionAtCanvas(void)
{
	return MapScalarToCanvas(this->value);
}

/**
Computes the width of the handle at the canvas, where it will be drawn. Transforms <i>Range</i> scalar
into pixel coordinates, computed proportionally inside the range [min - max].
*/
int wxMitkRangeSliderControl::ComputeHandleWidthAtCanvas(void)
{
	// Compute the step for the full range
	float step = this->GetStepSizeAtCanvas();
	// Compute the range by mapping the scalar into the full range
	int size = this->range * step;
	return size;
}

/**
Computes the mapping from Scalar value inside the range [min - max] into pixel coordinates for the canvas. 
The returned position is being offset.
*/
int wxMitkRangeSliderControl::MapScalarToCanvas(int scalar)
{
	// Get current widget area
    int width, height;
    this->GetClientSize(&width, &height);

	// Compute the step for the full range
	float step = this->GetStepSizeAtCanvas();
	// Compute the position by mapping the scalar into the full range
	int position;
	if(isVertical)
		position = height - ((scalar - this->minValue) * step) - wxMitkRangeSliderControl::HandleVerticalOffset;
	else
		position = ((scalar - this->minValue) * step) + wxMitkRangeSliderControl::HandleHorizontalOffset;
	return position;
}


/**
Computes the mapping from pixel (or mouse) coordinates of the canvas into a Scalar value inside the range [min - max]
*/
int wxMitkRangeSliderControl::MapCanvasToScalar(int position)
{
	// Get current widget area
    int width, height;
    this->GetClientSize(&width, &height);

	// Compute the step for the full range
	float step = this->GetStepSizeAtCanvas();
	// Compute the position by mapping the scalar into the full range
	int scalar;
	if(isVertical)
		scalar = this->minValue + (height - position - wxMitkRangeSliderControl::HandleVerticalOffset) / step;
	else
		scalar = this->minValue + (position - wxMitkRangeSliderControl::HandleHorizontalOffset) / step;
	return scalar;
}

/**
Computes the step size in pixels (mouse coords) that represents at the canvas one single step of value.
*/
float wxMitkRangeSliderControl::GetStepSizeAtCanvas(void)
{
	// Get current widget area
    int width, height;
    this->GetClientSize(&width, &height);
	if(isVertical)
		return (float)(height - wxMitkRangeSliderControl::HandleVerticalOffset * 2) / (float)(this->maxValue - this->minValue);
	else
		return (float)(width - wxMitkRangeSliderControl::HandleHorizontalOffset * 2) / (float)(this->maxValue - this->minValue);
}

/**
Checks if the given mouse position is inside the handle. This function is used to find out whether the user is
dragging the handle, with the mouse.
*/
bool wxMitkRangeSliderControl::IsMouseInsideTheHandle(int pos)
{
	return (pos > this->higherEdgeCoordinate) && (pos < this->lowerEdgeCoordinate);
}

/**
Checks if the given mouse position is within the handle edges. This function is used to find out whether the user is
dragging the center value or the range, with the mouse.
*/
bool wxMitkRangeSliderControl::IsMouseWithinHandleEdges(int pos)
{
	return IsMouseWithinHigherHandleEdge(pos) || IsMouseWithinLowerHandleEdge(pos);
}

/**
Checks if the given mouse position is within the higher handle edge.
*/
bool wxMitkRangeSliderControl::IsMouseWithinHigherHandleEdge(int pos)
{
	return (pos > (this->higherEdgeCoordinate - wxMitkRangeSliderControl::HandleEdgeWidth)) && (pos < (this->higherEdgeCoordinate + wxMitkRangeSliderControl::HandleEdgeWidth));
}

/**
Checks if the given mouse position is within the lower handle edge.
*/
bool wxMitkRangeSliderControl::IsMouseWithinLowerHandleEdge(int pos)
{
	return (pos > (this->lowerEdgeCoordinate - wxMitkRangeSliderControl::HandleEdgeWidth)) && (pos < (this->lowerEdgeCoordinate + wxMitkRangeSliderControl::HandleEdgeWidth));
}


//!
void wxMitkRangeSliderControl::SetHandleBackgroundColor(wxColour& color)
{
	this->handleBgColour = color;
}

//!
void wxMitkRangeSliderControl::SetCenterLineColor(wxColour& color)
{
	this->centerLineColour = color;
}





//////////////////////////////////////////////////////////////////////////////////
//
// Code for the wxMitkRangeChangedEvent
//
//////////////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(mitk::wxEVT_RANGE_CHANGED)
IMPLEMENT_DYNAMIC_CLASS(wxMitkRangeChangedEvent, wxCommandEvent)

//!
wxMitkRangeChangedEvent::wxMitkRangeChangedEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid), value(0), range(0), valueChanged(false), rangeChanged(false)
{
}

//!
wxMitkRangeChangedEvent::~wxMitkRangeChangedEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxMitkRangeChangedEvent::Clone(void)
{
	return new wxMitkRangeChangedEvent(*this);
}

//!
int wxMitkRangeChangedEvent::GetValue(void)
{
	return this->value;
}

//!
unsigned int wxMitkRangeChangedEvent::GetRange(void)
{
	return this->range;
}

//!
bool wxMitkRangeChangedEvent::ValueChanged(void)
{
	return this->valueChanged;
}

//!
bool wxMitkRangeChangedEvent::RangeChanged(void)
{
	return this->rangeChanged;
}

void mitk::wxMitkRangeSliderControl::OnEraseBackground( wxEraseEvent& event )
{

}
