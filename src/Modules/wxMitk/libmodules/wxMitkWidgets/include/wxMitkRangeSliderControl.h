/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkRangeSliderControl_H
#define wxMitkRangeSliderControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "mitkVector.h"
#include <wx/event.h>
#include <wx/control.h>

class wxMemoryDC;

namespace mitk
{

// Definitions for enabling the custom wxMitkRangeChangedEvent event 
// in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(WXMITKWIDGETS_EXPORT, wxEVT_RANGE_CHANGED, -1)

/**
\brief An event class responsible for signaling that the Value or Range 
has changed in a wxMitkRangeSliderControl control
\sa wxMitkRangeSliderControl

\ingroup wxMitkWidgets
\author Juan Antonio Moya, revised by Luigi Carotenuto
\date 22 Oct 2007 /rev. 03 Oct 2010
*/
class WXMITKWIDGETS_EXPORT wxMitkRangeChangedEvent : public wxCommandEvent
{
friend class wxMitkRangeSliderControl;

public:
	wxMitkRangeChangedEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_RANGE_CHANGED);
	virtual ~wxMitkRangeChangedEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	int GetValue(void);
	unsigned int GetRange(void);
	bool ValueChanged(void);
	bool RangeChanged(void);
	
private:
	int value;
	unsigned int range;
	bool valueChanged;
	bool rangeChanged;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkRangeChangedEvent)
};


// Definitions for enabling the custom wxMitkRangeChangedEvent event 
// in wxWidgets ///////////
typedef void (wxEvtHandler::*wxMitkRangeChangedEventFunction)
	(mitk::wxMitkRangeChangedEvent&);
#define wxMitkRangeChangedEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxMitkRangeChangedEventFunction, & func )
#define EVT_RANGE_CHANGED(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		mitk::wxEVT_RANGE_CHANGED, \
		id, wxID_ANY, wxMitkRangeChangedEventHandler(fn), NULL), 


///////////////////////////////////////////////////////////////////////////



/**
The Range slider provides a slider for controlling Range size and center 
position, in a way user-friendly. It can handle and validate a given range 
inside boundaries, and it is specially useful for working with any 
mitk::LevelWindow.

Makes use of a wxMitkLevelWindowHelper class, created for centralizing 
the validation of range values.

\sa wxMitkLevelWindowHelper

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 19 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkRangeSliderControl : public wxControl
{

public:
	wxMitkRangeSliderControl(
		wxWindow* parent, 
		wxWindowID id, 
		int value = 0, 
		unsigned int range = 5, 
		int minValue = -10, 
		int maxValue = 10, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxSL_VERTICAL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkRangeSliderControl"));
	virtual ~wxMitkRangeSliderControl(void);
	
	mitk::ScalarType GetValue(void);
	mitk::ScalarType GetRange(void);
	mitk::ScalarType GetMinValue(void);
	mitk::ScalarType GetMaxValue(void);

	void SetValue(mitk::ScalarType value);
	void SetRange(mitk::ScalarType range);
	void SetMinValue(mitk::ScalarType min);
	void SetMaxValue(mitk::ScalarType max);
	void SetAll(int value, unsigned int range, int minValue, int maxValue);

	void SetHandleBackgroundColor(wxColour& color);
	void SetCenterLineColor(wxColour& color);

protected:
	void ComputeValidRange(
		mitk::ScalarType value, 
		mitk::ScalarType range, 
		mitk::ScalarType minValue, 
		mitk::ScalarType maxValue);
	float GetStepSizeAtCanvas(void);

	bool IsMouseWithinHandleEdges(int yPos);
	bool IsMouseWithinHigherHandleEdge(int yPos);
	bool IsMouseWithinLowerHandleEdge(int yPos);
	bool IsMouseInsideTheHandle(int yPos);

	int MapScalarToCanvas(int scalar);
	int MapCanvasToScalar(int position);

	int ComputeHandlePositionAtCanvas(void);
	int ComputeHandleWidthAtCanvas(void);

private:

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseDrag(wxMouseEvent& event);
	void OnMousePress(wxMouseEvent& event);
	void OnMouseRelease(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
	void OnEraseBackground (wxEraseEvent& event);
	
	void DrawHandle(int width, int height, wxMemoryDC& MemDc);

	static const int HandleHorizontalOffset = 2;
	static const int HandleVerticalOffset = 2;
	static const int HandleEdgeWidth = 6;

	mitk::ScalarType value;
	mitk::ScalarType range;
	mitk::ScalarType minValue;
	mitk::ScalarType maxValue;

	bool draggingEdges;
	bool draggingHandle;
	int clickedCoordinate;

	int higherEdgeCoordinate;
	int lowerEdgeCoordinate;

	wxColour handleBgColour;
	wxColour centerLineColour;

	bool isVertical;

	DECLARE_EVENT_TABLE();
};
 
}

#endif // wxMitkRangeSliderControl_H
