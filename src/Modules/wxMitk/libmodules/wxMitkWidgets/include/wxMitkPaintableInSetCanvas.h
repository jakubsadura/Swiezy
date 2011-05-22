/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkPaintableInSetCanvas_H
#define wxMitkPaintableInSetCanvas_H

#include "wxMitkWidgetsWin32Header.h"

class wxColour;
class wxDC;
class wxRect;

namespace mitk
{
/*
The wxMitkPaintableInSetCanvas is a base widget for displaying widgets 
with a painted frame beveled in 2D inside the widget client area. Widgets 
may inherit this class so as to create controls with inset frames.
An example of those would be the wxMitkScalarOpacityControl and 
wxMitkColorFunctionControl, as they inherit from wxMitkHistogramCanvas 
and wxMitkVtkPiecewiseCanvas.

\sa wxMitkHistogramCanvas, wxMitkVtkPiecewiseCanvas, 
wxMitkScalarOpacityControl, wxMitkColorFunctionControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 03 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkPaintableInSetCanvas
{
public:

	void SetFrameBackgroundColor(const wxColour& colour);
	const wxColour& GetFrameBackgroundColor(void) const;
	void SetFrameForegroundColor(const wxColour& colour);
	const wxColour& GetFrameForegroundColor(void) const;

protected:
	wxMitkPaintableInSetCanvas(int inSetX = 8, int inSetY = 8, int lineWidth = 1);
	virtual ~wxMitkPaintableInSetCanvas(void);

	void GetInSetMargin(int& inSetX, int& inSetY) const;
	void GetInSetFrameArea(int& inSetX, int& inSetY, int& width, int& height) const;
	const wxRect GetInSetFrameArea(void) const;
	void GetInSetFrameAreaWithoutBorder(int& inSetX, int& inSetY, int& width, int& height) const;
	const wxRect GetInSetFrameAreaWithoutBorder(void) const;


	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

	virtual void GetClientSize(int* width, int* height) const = 0;
	virtual wxColour GetBackgroundColour() const = 0;

private:

	wxColour frameBgColor;
	wxColour frameFgColor;
	const int inSetBorderX;
	const int inSetBorderY;
	const int borderLineWidth;

};
 
} // namespace mitk


#endif // wxMitkScalarOpacityControl_H

