/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkHistogramCanvas_H
#define wxMitkHistogramCanvas_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkPaintableInSetCanvas.h"
#include <mitkHistogramGenerator.h>

class wxColour;
class wxDC;
class wxRect;

namespace mitk
{

/*
The wxMitkHistogramCanvas is a base widget for displaying Histograms. 
Widgets may inherit this class so as
to provide a rendering of an histogram at the background

\sa wxMitkScalarOpacityControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 28 Nov 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkHistogramCanvas 
	: public virtual wxMitkPaintableInSetCanvas
{
public:

	void SetHistogram(
		const mitk::HistogramGenerator::HistogramType* histogram);
	void SetHistogramColor(const wxColour& colour);
	
	const wxColour& GetHistogramColor(void) const;

protected:
	wxMitkHistogramCanvas(void);
	virtual ~wxMitkHistogramCanvas(void);

	void GetHistogramArea(int& inSetX, int& inSetY, int& width, int& height) const;
	const wxRect GetHistogramArea(void) const;

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

private:
	void DrawHistogram(wxDC& Dc);

	mitk::HistogramGenerator::HistogramType::ConstPointer histogram;
	wxColour histogramFgColor;

};
 
} // namespace mitk


#endif // wxMitkScalarOpacityControl_H

