/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkGradientOpacityWidget_H
#define wxMitkGradientOpacityWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include "wx/panel.h"
#include <mitkHistogramGenerator.h>

class vtkPiecewiseFunction;
class wxStaticText;

namespace mitk
{
class wxMitkGradientOpacityControl;
class wxMitkMouseOverHistogramEvent;

/*
The wxMitkGradientOpacityWidget is a custom widget for editing transfer 
functions for volume rendering.
It enables the user to modify the gradient opacity function.
\sa wxMitkGradientOpacityControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 11 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkGradientOpacityWidget : public wxPanel
{
public:

	wxMitkGradientOpacityWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkGradientOpacityWidget"));
	virtual ~wxMitkGradientOpacityWidget(void);

	void SetPiecewiseFunction(vtkPiecewiseFunction* func);
	vtkPiecewiseFunction* GetPiecewiseFunction(void) const;

	void SetHistogram(
		const mitk::HistogramGenerator::HistogramType* histogram);
	void SetHistogramColor(const wxColour& colour);
	
	const wxColour& GetHistogramColor(void) const;

	void SetInteractiveApply(bool enable);
	void ResetWidget(void);

private:

	void OnMousePositionChangedOverHistogram(
		wxMitkMouseOverHistogramEvent& event);

	wxMitkGradientOpacityControl* opacityFunctionControl;

	wxStaticText* lblRange;
	wxStaticText* lblScalar;
	
	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkGradientOpacityWidget_H
