/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkColorGradientCanvas_H
#define wxMitkColorGradientCanvas_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkPaintableInSetCanvas.h"

class vtkColorTransferFunction;

namespace mitk
{

/*
The wxMitkColorGradientCanvas is a widget component of 
wxMitkColorGradientControl. It enables the user to display a gradient of 
the colors picked for the transfer function, giving information of scalar 
color mapping.

\sa wxMitkColorGradientControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkColorGradientCanvas 
	: public virtual wxMitkPaintableInSetCanvas
{
public:
	void SetColorTransferFunction (vtkColorTransferFunction* func);
	vtkColorTransferFunction* GetColorTransferFunction(void) const;

protected:
	wxMitkColorGradientCanvas(void);
	virtual ~wxMitkColorGradientCanvas(void);

	void GetGradientColourArea(
		int& inSetX, 
		int& inSetY, 
		int& width, 
		int& height) const;
	const wxRect GetGradientColourArea(void) const;

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

private:
	void DrawLinearGradient(wxDC& Dc);

	vtkColorTransferFunction* colorFunc;
};
 
} // namespace mitk


#endif // wxMitkColorGradientCanvas_H

