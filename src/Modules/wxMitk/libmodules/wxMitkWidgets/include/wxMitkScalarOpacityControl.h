/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkScalarOpacityControl_H
#define wxMitkScalarOpacityControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkHistogramCanvas.h"
#include "wxMitkVtkPiecewiseCanvas.h"
#include "wxMitkMouseOverHistogramEvent.h"

#include <wx/control.h>

class wxDC;

namespace mitk
{

/*
The wxMitkScalarOpacityControl is a widget component of 
wxMitkTransferFunctionWidget. It enables the user to tweak the opacity 
points in a transferfunction of a given image. Opacity points are applied 
on volume rendering in order to clip certain scalar ranges of an image, 
so as to allow a deeper exploration.

\sa wxMitkTransferFunctionWidget
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 26 Nov 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkScalarOpacityControl 
	: public wxControl, 
	  public wxMitkHistogramCanvas, 
	  public wxMitkVtkPiecewiseCanvas
{
public:
	wxMitkScalarOpacityControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkScalarOpacityControl"));
	virtual ~wxMitkScalarOpacityControl(void);

	virtual bool Enable(bool enable = true);
	void SetInteractiveApply(bool enable);

protected:
	bool MovingFunctionPointIsAllowed(double& fpX);
	virtual void UpdateObserversOfTransferFunction(void);
	virtual void UpdateObserversOfMousePosition(
		const double fpX, 
		const double opacity);

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


#endif // wxMitkScalarOpacityControl_H

