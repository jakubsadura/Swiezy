/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkColorGradientControl_H
#define wxMitkColorGradientControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkColorGradientCanvas.h"
#include "wxMitkVtkColorTransferFunctionCanvas.h"
#include "wxMitkMouseOverHistogramEvent.h"
#include <wx/control.h>


namespace mitk
{

/*
The wxMitkColorGradientControl is a widget component of 
wxMitkScalarColorWidget. It enables the user to display a gradient of the 
colors picked for the transfer function, giving information of scalar color
mapping. Color points are applied on volume rendering in order to provide 
additional information or either realistic rendering. This control is 
originally designed to provide info to the user, but not interaction.
Interaction on the color function is done through the 
wxMitkColorFunctionControl component.

\sa wxMitkScalarColorWidget, wxMitkColorFunctionControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkColorGradientControl 
	: public wxControl, 
	  public virtual wxMitkColorGradientCanvas, 
	  private virtual wxMitkVtkColorTransferFunctionCanvas
{
public:
	wxMitkColorGradientControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkColorGradientControl"));
	virtual ~wxMitkColorGradientControl(void);

	void SetColorTransferFunction (vtkColorTransferFunction* func);
	vtkColorTransferFunction* GetColorTransferFunction(void) const;
	
	virtual bool Enable(bool enable = true);
	
protected:
	virtual void GetClientSize(int* width, int* height) const;
	virtual wxColour GetBackgroundColour() const;

	virtual void UpdateObserversOfMousePosition(
		const double fpX, 
		const double rgb[3]);

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

private:
	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnEraseBackground(wxEraseEvent& event);
	void OnMouseEvent(wxMouseEvent& event);


	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkColorGradientCanvas_H

