/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkScalarColorWidget_H
#define wxMitkScalarColorWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include "wx/panel.h"
#include <mitkHistogramGenerator.h>

class vtkColorTransferFunction;
class wxStaticText;

namespace mitk
{
class wxMitkColorFunctionControl;
class wxMitkColorGradientControl;
class wxMitkColorFunctionChangedEvent;
class wxMitkMouseOverHistogramEvent;

/*
The wxMitkScalarColorWidget is a custom widget for editing transfer 
functions for volume rendering.

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkScalarColorWidget : public wxPanel
{
public:

	wxMitkScalarColorWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkTransferFunctionWidget"));
	virtual ~wxMitkScalarColorWidget(void);

	void SetColorTransferFunction(vtkColorTransferFunction* func);
	vtkColorTransferFunction* GetColorTransferFunction(void) const;

	void SetHistogram(
		const mitk::HistogramGenerator::HistogramType* histogram);
	void SetHistogramColor(const wxColour& colour);
	
	const wxColour& GetHistogramColor(void) const;

	void SetInteractiveApply(bool enable);
	void ResetWidget(void);

private:
	
	void OnColorFunctionChanged(wxMitkColorFunctionChangedEvent& event);
	void OnMousePositionChangedOverHistogram(wxMitkMouseOverHistogramEvent& event);

	wxMitkColorFunctionControl* colorFunctionControl;
	wxMitkColorGradientControl* colorGradientControl;
	wxStaticText* lblRange;
	wxStaticText* lblScalar;
	
	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkScalarColorWidget_H
