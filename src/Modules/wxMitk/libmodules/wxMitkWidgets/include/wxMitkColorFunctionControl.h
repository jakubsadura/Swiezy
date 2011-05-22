/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkColorFunctionControl_H
#define wxMitkColorFunctionControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkHistogramCanvas.h"
#include "wxMitkVtkColorTransferFunctionCanvas.h"
#include "wxMitkMouseOverHistogramEvent.h"
#include <wx/control.h>
#include <wx/event.h>

class wxDC;
class vtkColorTransferFunction;


namespace mitk
{
// Definitions for enabling the custom wxMitkColorFunctionChangedEvent 
// event in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(WXMITKWIDGETS_EXPORT, wxEVT_COLORFUNCTION_CHANGED, -1)

/**
\brief An event class responsible for signaling that the value of the 
TransferFunction has changed within wxMitkColorFunctionControl control
\sa wxMitkColorFunctionControl

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkColorFunctionChangedEvent 
	: public wxCommandEvent
{

friend class wxMitkColorFunctionControl;

public:
	wxMitkColorFunctionChangedEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_COLORFUNCTION_CHANGED);
	virtual ~wxMitkColorFunctionChangedEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	vtkColorTransferFunction* GetColorTransferFunction(void);

private:
	vtkColorTransferFunction* colorFunc;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkColorFunctionChangedEvent)
};



// Definitions for enabling the custom wxMitkColorFunctionChangedEvent 
// event in wxWidgets ///////////
typedef void (wxEvtHandler::*wxMitkColorFunctionChangedEventFunction)
	(mitk::wxMitkColorFunctionChangedEvent&);

#define wxMitkColorFunctionChangedEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxMitkColorFunctionChangedEventFunction, & func )
#define EVT_COLORFUNCTION_CHANGED(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		mitk::wxEVT_COLORFUNCTION_CHANGED, \
		id, wxID_ANY, wxMitkColorFunctionChangedEventHandler(fn), NULL), 



// ------------------------------------------------------------------------


/*
The wxMitkColorFunctionControl is a widget component of 
wxMitkScalarColorWidget. It enables the user to tweak the opacity points 
in a transferfunction of a given image. Opacity points are applied on volume 
rendering in order to clip certain scalar ranges of an image, so as to 
allow a deeper exploration.

\sa wxMitkScalarColorWidget
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkColorFunctionControl 
	: public wxControl, 
	  public wxMitkHistogramCanvas, 
	  public wxMitkVtkColorTransferFunctionCanvas
{
public:
	wxMitkColorFunctionControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkColorFunctionControl"));
	virtual ~wxMitkColorFunctionControl(void);

	virtual bool Enable(bool enable = true);
	void SetInteractiveApply(bool enable);

protected:
	bool MovingFunctionPointIsAllowed(double& fpX) const;
	virtual void UpdateObserversOfTransferFunction(void);
	virtual void UpdateObserversOfMousePosition(
		const double fpX, 
		const double rgb[3]);

	void GetClientSize(int* width, int* height) const;
	wxColour GetBackgroundColour() const;

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

private:

	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnMouseEvent(wxMouseEvent& event);

	/**
	\brief This handler will never execute because ReleaseMouse( )
	is always called at OnMouseEvent( )
	*/
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	
	bool interactiveApply;
	bool buttonPressed;

	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkColorFunctionControl_H

