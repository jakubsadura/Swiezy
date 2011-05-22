/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkVtkPiecewiseCanvas_H
#define wxMitkVtkPiecewiseCanvas_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkPaintableInSetCanvas.h"

class vtkPiecewiseFunction;
class wxDC;

namespace mitk
{

/*
The wxMitkVtkPiecewiseCanvas is a widget component of 
wxMitkTransferFunctionWidget. It eases the creation of controls that 
enable to interact with a vtkPiecewiseFunction. An example of this kind 
of controls would be wxMitkScalarOpacityControl and wxMitkColorFunctionControl

\sa wxMitkScalarOpacityControl, wxMitkColorFunctionControl
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 03 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkVtkPiecewiseCanvas 
	: public virtual wxMitkPaintableInSetCanvas
{
public:
	void SetPiecewiseFunction(vtkPiecewiseFunction* func);
	vtkPiecewiseFunction* GetPiecewiseFunction(void) const;

protected:
	wxMitkVtkPiecewiseCanvas(void);
	virtual ~wxMitkVtkPiecewiseCanvas(void);

	void FunctionPointToCanvas(double fpX, double fpY, int& canvasX , int& canvasY) const;
	void CanvasToFunctionPoint(int canvasX, int canvasY, double& fpX, double& fpY) const;
	int GetFunctionPointIndexNearCanvasPoint(int canvasX, int canvasY) const;
	void GetFunctionPointFromPointIndex(int selectedPointIndex, double& fpX, double& fpY) const;
	
	void UnSelectHandle(void);
	void SelectHandle(int index);
	int GetSelectedHandle(void) const;
	bool HandleIsSelected(void) const;

	virtual void DrawControl(wxDC& Dc, bool foreGroundOnly = false);

	const int maxSquareDistanceToGrabHandle;

private:
	void DrawHandles(wxDC& Dc);

	vtkPiecewiseFunction* pieceWiseFunc;

	int selectedHandle;
};
 
} // namespace mitk


#endif // wxMitkVtkPiecewiseCanvas_H

