/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkVtkColorTransferFunctionCanvas_H
#define wxMitkVtkColorTransferFunctionCanvas_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkPaintableInSetCanvas.h"

class vtkColorTransferFunction;
class wxDC;

namespace mitk
{

/*
The wxMitkVtkColorTransferFunctionCanvas is a widget component of 
wxMitkColorFunctionControl. It eases the creation of controls that enable 
to interact with a vtkColorTransferFunction. An example of this kind of 
controls would be wxMitkScalarOpacityControl and wxMitkColorFunctionControl

\sa wxMitkScalarOpacityControl, wxMitkColorFunctionControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 05 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkVtkColorTransferFunctionCanvas 
	: public virtual wxMitkPaintableInSetCanvas
{
public:
	void SetColorTransferFunction (vtkColorTransferFunction* func);
	vtkColorTransferFunction* GetColorTransferFunction(void) const;

protected:
	wxMitkVtkColorTransferFunctionCanvas(void);
	virtual ~wxMitkVtkColorTransferFunctionCanvas(void);

	void FunctionPointToCanvas(double fpX, int& canvasX, int& canvasY) const;
	void CanvasToFunctionPoint(int canvasX, int canvasY, double& fpX, double rgb[3]) const;
	int GetFunctionPointIndexNearCanvasPoint(int canvasX, int canvasY) const;
	void GetFunctionPointFromPointIndex(int selectedPointIndex, double& fpX, double rgb[3]) const;
	
	void UnSelectHandle(void);
	void SelectHandle(int index);
	int GetSelectedHandle(void) const;
	bool HandleIsSelected(void) const;

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

	const int maxSquareDistanceToGrabHandle;

private:
	void DrawHandles(wxDC& Dc);

	vtkColorTransferFunction* colorFunc;

	int selectedHandle;
};
 
} // namespace mitk


#endif // wxMitkVtkColorTransferFunctionCanvas_H

